#ifndef PARSTOOLS_PARSER_BASE_HPP
#define PARSTOOLS_PARSER_BASE_HPP

#include "utils.hpp"

// TODO: some methods can be inherited from common BaseParser base class
//  when parsing results, delete parsed token from .rest

template<class P>
concept Parser = (sizeof(P) == 1) and
        std::is_default_constructible_v<P> and
        requires (str input, const MaybeItem result, MaybeItems results) {
            {1 + 2} -> std::convertible_to<int>;
//    { P::run(input) }   -> std::same_as<MaybeItem>;
//    { P::run(result) }  -> std::same_as<MaybeItem>;
//    { P::run(results) } -> std::same_as<MaybeItems&>;
//    { P::get_token(input) } -> std::same_as<MaybeStr>;
};

template<Token<char_cmp> T>
struct ConditionalCharParser {
    static constexpr Token_t token_type = T.type;
    static constexpr char_cmp eq        = T.value;

    consteval ConditionalCharParser() = default;

    static constexpr MaybeItem run (const std::ranges::input_range auto & value) {
        if (value.size() != 0 and eq(value[0])) {
            return Item({{value[0]}, token_type}, {value.begin() + 1, value.end()});
        }
        return std::nullopt;
    }

    static constexpr MaybeToken get_token (const std::ranges::input_range auto &value) {
        if (value.size() != 0 and eq(value[0])) {
            return {{{value[0]}, token_type}};
        }
        return std::nullopt;
    }

    static constexpr MaybeItem run (const MaybeItem& result) {
        if(not result.has_value())
            return std::nullopt;
        return run(result.value().rest);
    }

    static constexpr MaybeItems& run(MaybeItems & result) {
        if (not (result.has_value() and get_token(result.value().rest)))  // de-morgan magic
            return result;
        result.value().found.push_back(get_token(result.value().rest).value());
        result.value().rest.erase(0, 1);

        return result;
    }
};

template<char C>
struct CharParser : ConditionalCharParser<Token<char_cmp>{is_same_char<C>, to_Token_t(C)}>{};


// function shall return 0 if comparison failed, else length of compared string
template<Token<str_cmp> T>
struct ConditionalStrParser {
    static constexpr Token_t token_type = T.type;
    static constexpr str_cmp eq         = T.value;

    consteval ConditionalStrParser() = default;

    static constexpr MaybeItem run (const std::ranges::input_range auto &value) {
        if (not eq(value))
            return std::nullopt;
        return Item({{value.begin(), value.begin() + eq(value)}, token_type},
                    {value.begin() + eq(value), value.end()});
    }

    static constexpr MaybeToken get_token (const std::ranges::input_range auto &value) {
        if (not eq(value))
            return std::nullopt;
        return {{{value.begin(), value.begin() + eq(value)}, token_type}};
    }

    static constexpr MaybeItem run (const MaybeItem& result) {
        if(not result.has_value())
            return std::nullopt;
        return run(result.value().rest);
    }

    static constexpr MaybeItems& run (MaybeItems& result) {     // FIXME: Maybe is useless here
        if (not (result.has_value() and get_token(result.value().rest)))
            return result;
        auto temp = get_token(result.value().rest).value();
        auto temp_size = temp.value.size();

        result.value().found.push_back(temp);
        result.value().rest.erase(0, temp_size);

        return result;
    }
};

template<str_literal target>
struct StrParser : public ConditionalStrParser<Token<str_cmp>{is_same_str<target>, to_Token_t(target)}> {};
//static_assert(Parser<StrParser<Token{"str", Token_t::ws}>>);

template<Parser FirstParser, Parser... Parsers>
struct MultiParser {
    consteval MultiParser() = default;

    static constexpr MaybeItem run (const std::ranges::input_range auto & value) {
        if (FirstParser::run(value).has_value())
            return FirstParser::run(value);
        return MultiParser<Parsers...>::run(value);
    }

    static constexpr MaybeItem run (const MaybeItem& result) {
        if(not result.has_value())
            return std::nullopt;
        return run(result.value().rest);
    }

    static constexpr MaybeItems & run (MaybeItems& results) {
        return MultiParser<Parsers...>::run(FirstParser::run(results));
    }

    static constexpr MaybeItems & run_until_fail (MaybeItems& results) {   // FIXME: simplify
        if(not results.has_value())
            return results;
        const size_t prev_s = results.value().found.size();
        MultiParser<Parsers...>::run_until_fail(FirstParser::run(results));
        if (not results.has_value() or prev_s == results.value().found.size())
            return results;
        return run(results);
    }

    static constexpr MaybeItems & parse_str (const str& value) {
        auto *r = new MaybeItems{{{}, value}};
        return run(*r);
    }

    static constexpr MaybeItems & parse_str_until_fail (const str& value) {
        auto *r = new MaybeItems{{{}, value}};
        return run_until_fail(*r);
    }
};

template<Parser TheParser>
struct MultiParser<TheParser> {
    consteval MultiParser() = default;

    static constexpr MaybeItem run (const std::ranges::input_range auto &value) {
        return TheParser(value);
    }

    static constexpr MaybeItem run (const MaybeItem& result) {
        if(not result.has_value())
            return std::nullopt;
        return run(result.value().rest);
    }

    static constexpr MaybeItems & run (MaybeItems& results) {
        return TheParser::run(results);
    }

    static constexpr MaybeItems & run_until_fail (MaybeItems& results) {
        return TheParser::run(results);
    }

    static constexpr MaybeItems & parse_str (const str& value) {    // just for compatibility
        auto *r = new MaybeItems{{{}, value}};
        return run(*r);
    }

    static constexpr MaybeItems & parse_str_until_fail (const str& value) {    // just for compatibility
        auto *r = new MaybeItems{{{}, value}};
        return run_until_fail(*r);
    }
};
static_assert(Parser<MultiParser<StrParser<"NULL">,
                                 CharParser<' '>,
                                 MultiParser<StrParser<"NULL">,
                                             CharParser<' '>
                                             >
                                 >
                     >);



#endif //PARSTOOLS_PARSER_BASE_HPP



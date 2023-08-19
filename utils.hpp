#ifndef PARSTOOLS_UTILS_HPP
#define PARSTOOLS_UTILS_HPP

#include <concepts>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <iostream>
#include <ostream>
#include <ranges>
#include <functional>
#include <memory>

// TODO: watch a shitload of everything on string and string_view
//  replace type of found with token and refactor

using str = std::string;
using std::cout;

typedef bool   (*char_cmp) (char);
typedef size_t (*str_cmp)  (const str &);

#define CHAR_CMP(func_name, var_name) constexpr bool   func_name(char       var_name)
#define STR_CMP( func_name, var_name) constexpr size_t func_name(const str& var_name)

template<size_t N>
struct str_literal;

enum class Token_t {
    ws,
    variable,
    ch,
    string,
};

std::ostream & operator << (std::ostream &out, Token_t t);

constexpr Token_t to_Token_t(char c) {  // will have a giant switch-stmt
    using enum Token_t;
    return ch;
}

template<size_t N>
constexpr Token_t to_Token_t(str_literal<N> strng){  // will have a giant switch-stmt
    using enum Token_t;
    return string;
}

template<class T>
        concept Show = requires(std::ostream & out, T self){
            { out << self } -> std::same_as<std::ostream &>;
};

template<class T, size_t N = 0>
        concept Cmp_or_Str = std::same_as<T, char_cmp>   ||
                             std::same_as<T, str_cmp>    ||
                             std::convertible_to<T, str> ||
                             std::same_as<T, char>;

template<Cmp_or_Str T>
struct Token {
    T             value;
    const Token_t type;

    constexpr Token(T val, Token_t typ)
        : value{val}, type(typ) {}
};

std::ostream & operator << (std::ostream & out, const Token<char_cmp>&);
std::ostream & operator << (std::ostream & out, const Token<str_cmp>&);
std::ostream & operator << (std::ostream & out, const Token<char>&);
template<class Str>
        requires std::convertible_to<Str, str>
std::ostream & operator << (std::ostream & out, const Token<Str>& tok) {
    out << "Token(value = " << tok.value << ", type = " << tok.type << ")\n";
    return out;
}

using MaybeToken = std::optional<Token<str>>;

struct Items {
    std::vector<Token<str>> found;    // std::vector<Token<str>> found;
    str rest;
};

struct Item {
    const Token<str> found;    // const Token found
    const str rest;

    operator Items() const {    // must be implicit
        return {{found}, rest};
    }
};

using MaybeItem  = std::optional<Item>;
using MaybeItems = std::optional<Items>;

using MaybeStr  = std::optional<str>;
//using MaybeStrs = std::optional<std::vector<str>>;

std::ostream & operator << (std::ostream & out, const Item & obj);
std::ostream & operator << (std::ostream & out, const Items & obj);

template<Show T>
std::ostream & operator << (std::ostream & out, const std::optional<T>& obj) {
    if(obj.has_value())
        out << "Maybe(" << obj.value() << ")\n";
    else
        out << "Maybe(nothing)\n";
    return out;
}


template<size_t N>
struct str_literal {
    static_assert(N != 0, "C-strings can't be of length zero\n");
    static constexpr size_t length = N - 1;
    char value[N];

    constexpr str_literal(const char (&string)[N]) {    // must be implicit
        std::copy_n(string, N, value);
    }

    static consteval size_t size() noexcept {   // for compatibility
        return length;
    }

    constexpr auto begin() const noexcept {
        return value;
    }

    constexpr auto end() const noexcept {
        return value + length;
    }

    operator str() const {  // must be implicit
        return str(value);
    }
};
static_assert(std::ranges::input_range<str_literal<2>>);
static_assert(std::input_iterator<decltype(str_literal("hello").begin())>);


template<char C>
CHAR_CMP(is_same_char, p) {
    return C == p;
}


template<str_literal S>
STR_CMP(is_same_str, string) {
    if (S.size() > string.size() or not std::equal(S.begin(), S.end(), string.begin()))
        return 0;
    return S.size();
}

STR_CMP(is_word, string) {
    size_t count = 0;
    char c = string[count];
    while(c != ' ' && c != '\0')
        c = string[++count];
    return count;
}

#endif //PARSTOOLS_UTILS_HPP

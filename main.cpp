#include "parsers.hpp"

bool is_same(std::input_iterator auto It1,
             std::input_iterator auto It1_end,
             std::input_iterator auto It2) {
    if(It1 != It1_end)
        return (*It1 == *It2) && (is_same(++It1, It1_end, ++It2));
    return true;
}

int main() {
    str string1 = "NULL bitch + else";
    str string2 = "  bitch  hello ";

    cout << word_parser::parse_str_until_fail(string2);

//    cout << big_parser::parse_str_until_fail(string);
}

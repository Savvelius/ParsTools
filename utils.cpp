#include "utils.hpp"

std::ostream &operator<<(std::ostream &out, const Item &obj) {
    out << "Item(found = " << obj.found << ", rest = " << obj.rest;
    return out;
}

std::ostream &operator<<(std::ostream &out, const Items &obj) {
    out << "Items(";
    for (size_t i = 0; i < obj.found.size(); ++i)
        out << "found_" << i << " = " << obj.found[i] << ", ";
    out << '\n' << "rest = " << obj.rest << '\n';
    return out;
}

std::ostream &operator<<(std::ostream &out, Token_t t) {
    using enum Token_t;
    switch (t) {
        case ws:
            out << "ws";
            break;
        case variable:
            out << "variable";
            break;
        case ch:
            out << "ch";
            break;
        case string:
            out << "string";
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const Token <char_cmp> & tok) {
    out << "Token(value = " << "char_cmp, " << "type = " << tok.type << ")\n";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Token <str_cmp> & tok) {
    out << "Token(value = " << "str_cmp, " << "type = " << tok.type << ")\n";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Token<char> & tok) {
    out << "Token(value = " << tok.value << ", type = " << tok.type << ")\n";
    return out;
}


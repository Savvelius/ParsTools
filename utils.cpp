#include "utils.hpp"

std::ostream &operator<<(std::ostream &out, const Item &obj) {
    out << "Item(found = " << obj.found << ", rest = " << obj.rest;
    return out;
}

std::ostream &operator<<(std::ostream &out, const MaybeItem &obj) {
    if(obj.has_value())
        out << "MaybeItem(" << obj.value() << ")\n";
    else
        out << "MaybeItem(nothing)\n";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Items &obj) {
    out << "Items(";
    for (size_t i = 0; i < obj.found.size(); ++i)
        out << "found_" << i << " = " << obj.found[i] << ", ";
    out << '\n' << "rest = " << obj.rest << '\n';
    return out;
}

std::ostream &operator<<(std::ostream &out, const MaybeItems &obj) {
    if(obj.has_value())
        out << "MaybeItems(" << obj.value() << ")\n";
    else
        out << "MaybeItems(nothing)\n";
    return out;
}

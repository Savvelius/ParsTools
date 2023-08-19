#include "parsers.hpp"

bool is_same(std::input_iterator auto It1,
             std::input_iterator auto It1_end,
             std::input_iterator auto It2) {
    if(It1 != It1_end)
        return (*It1 == *It2) && (is_same(++It1, It1_end, ++It2));
    return true;
}

// TODO:
//  write a shitload of ostream operators

int main() {

}

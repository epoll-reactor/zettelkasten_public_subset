// Feb 18, 2022
#include <string>
#include <iostream>

bool contain(std::string baseString, std::string subString) {
    auto base_begin = baseString.begin();
    auto base_end = baseString.end();
    auto sub_begin = subString.begin();
    auto sub_end = subString.end();
    auto sub_first = sub_begin;

    while (base_begin != base_end && sub_begin != sub_end) {
        if (*base_begin == *sub_begin) {
            while (*base_begin == *sub_begin) {
                if (base_begin == base_end)
                    break;
                if (sub_begin == sub_end)
                    break;
                ++base_begin;
                ++sub_begin;
            }
            if (sub_begin == sub_end) {
                return true;
            }
        }
        sub_begin = sub_first;
        ++base_begin;
    }
    
    return false;
}

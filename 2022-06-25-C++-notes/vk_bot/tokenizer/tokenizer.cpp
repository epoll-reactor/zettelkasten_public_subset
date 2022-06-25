#include "tokenizer.hpp"

string_vector Tokenizer::tokenize_line(const std::string& line) {
    std::istringstream stream(line);
    string_vector tokens {
        std::istream_iterator<std::string>(stream), { }
    };
    return tokens;
}

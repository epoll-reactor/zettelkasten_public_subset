#include "command_handler/command_handler.hpp"

#ifdef DOC_COM
void Command_handler::document_command(const string_vector& text) {
    if (_message == "+документ") {
        _if_empty_query();
    } else {
        _media_search(text, "docs.search");
    }
}
#endif //DOC_COM

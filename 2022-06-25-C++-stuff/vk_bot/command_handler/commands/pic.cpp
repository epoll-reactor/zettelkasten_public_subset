#include "command_handler/command_handler.hpp"

#ifdef PIC_COM
void Command_handler::picture_command(const string_vector& text) {
    if (_message == "+пикча") {
        _if_empty_query();
    } else {
        _media_search(text, "photos.search");
    }
}
#endif //PIC_COM

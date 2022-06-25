#include "command_handler/command_handler.hpp"

#ifdef VIDEO_COM
void Command_handler::video_command(const string_vector& text) {
    if (_message == "+видео") {
        _if_empty_query();
    } else {
        _media_search(text, "video.search");
    }
}
#endif //VIDEO_COM

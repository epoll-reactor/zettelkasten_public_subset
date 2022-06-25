#include "command_handler/command_handler.hpp"

#ifdef HELP_COM
void Command_handler::_help_autogen() {
    #ifdef CRC32_COM
        _help_info_vector.push_back("⚠+crc32 {...} - сгенерить CRC-32 хеш-сумму строки\n");
    #endif
    #ifdef SHA256_COM
        _help_info_vector.push_back("⚠+sha256 {...} - сгенерить SHA-256 хеш-сумму строки\n");
    #endif
    #ifdef PIC_COM
        _help_info_vector.push_back("⚠+пикча {...} - найти картинку среди просторов ВК\n");
    #endif
    #ifdef VIDEO_COM
        _help_info_vector.push_back("⚠+видео {...} - поиск видеозаписей\n");
    #endif
    #ifdef DOC_COM
        _help_info_vector.push_back("⚠+документ {...} - поиск документов\n");
    #endif
    #ifdef BAN_COM
        _help_info_vector.push_back("⚠+кик {...} - кикнуть юзера из беседы\n");
    #endif
    #ifdef WEATHER_COM
        _help_info_vector.push_back("⚠+погода {...} - показать погоду\n");
    #endif
    #ifdef NICKNAME_COM
        _help_info_vector.push_back("⚠+никнейм {...} - установить обращение бота к тебе\n"
                                    "⚠+никнейм удалить - удалить свой никнейм\n");
    #endif
}

void Command_handler::help_command() {
    string help_info = "Список команд:\n";
    _help_autogen();
    for (auto element : _help_info_vector) {
        help_info += element;
    }
    pair_vector answer_query = {
        { "message", help_info },
        { "peer_id", _peer_id  }
    };
    string query = cURL::generate_vk_query("messages.send", answer_query);
    cURL::request(query);
}
#endif //HELP_COM

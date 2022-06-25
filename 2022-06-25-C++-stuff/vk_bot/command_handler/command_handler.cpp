#include "command_handler.hpp"

Command_handler::Command_handler(const string& message, const string& peer_id, const string& from_id)
    : _message(message)
    , _peer_id(peer_id)
    , _from_id(from_id)
{ }

void Command_handler::_if_empty_query() {
    pair_vector answer_query = {
        { "message", "Задана пустая строка." },
        { "peer_id", _peer_id }
    };
    string query = cURL::generate_vk_query("messages.send", answer_query);
    cURL::request(query);
}

void Command_handler::init_commands() {
    const string_vector tokenized_message = Tokenizer::tokenize_line(_message);

#ifdef HELP_COM
    if (tokenized_message[0] == "+помощь") {
        help_command();
    }
#endif
#ifdef CRC32_COM
    if (tokenized_message[0] == "+crc32") {
        crc32_command(tokenized_message);
    }
#endif

#ifdef SHA256_COM
    if (tokenized_message[0] == "+sha256") {
        sha256_command(tokenized_message);
    }
#endif

#ifdef PIC_COM
    if (tokenized_message[0] == "+пикча") {
        picture_command(tokenized_message);
    }
#endif

#ifdef VIDEO_COM
    if (tokenized_message[0] == "+видео") {
        video_command(tokenized_message);
    }
#endif

#ifdef DOC_COM
    if (tokenized_message[0] == "+документ") {
        document_command(tokenized_message);
    }
#endif

#ifdef WEATHER_COM
    if (tokenized_message[0] == "+погода") {
        weather_command(tokenized_message);
    }
#endif

#ifdef TRANSLATE_COM
    if (tokenized_message[0] == "+переводчик") {
        translate_command(tokenized_message);
    }
#endif

#ifdef NICKNAME_COM
    if (tokenized_message[0] == "+никнейм") {
        if (_message == "+никнейм") {
            _if_empty_query();
        } else if (tokenized_message[1] == "удалить") {
            remove_nickname_command(_from_id);
        } else {
            add_nickname_command(_from_id, tokenized_message[1]);
        }
    }
#endif

#ifdef BAN_COM
    if (tokenized_message[0] == "+бан") {
        ban_command(tokenized_message);
    }
#endif
}



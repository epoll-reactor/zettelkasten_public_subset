#include "command_handler/command_handler.hpp"

#ifdef NICKNAME_COM
void Command_handler::add_nickname_command(const string& user_id, const string& prefix) {
    database.insert_nickname(user_id, prefix);
    pair_vector answer = {
        {"message", "Никнейм успешно установлен\n"},
        {"peer_id", _peer_id}
    };
    cURL::request(cURL::generate_vk_query("messages.send", answer));
}

void Command_handler::remove_nickname_command(const string& user_id) {
    database.insert_nickname(user_id, "");
    pair_vector answer;
    if (_nickname != "") {
        answer = {
            { "message", "Никнейм успешно удалён\n" },
            { "peer_id", _peer_id }
        };
    } else {
        answer = {
            { "message", "Никнейм не установлен\n" },
            { "peer_id", _peer_id }
        };
    }
    cURL::request(cURL::generate_vk_query("messages.send", answer));
}
#endif //NICKNAME_COM

#include "command_handler/command_handler.hpp"

#ifdef BAN_COM
void Command_handler::ban_command(const string_vector& text) {
    string_vector tokenized_message = Tokenizer::tokenize_line(_message);
    if (_message == "+кик") {
        pair_vector answer_query = {
            std::make_pair("message", "Некого кикать."),
            std::make_pair("peer_id", _peer_id)
        };
        string query = cURL::generate_vk_query("messages.send"
            , answer_query);
       cURL::request(query);
    } else {
        long peer_id = std::stol(_peer_id);
        peer_id     -= 2000000000;
        string to_kick = text[1];
        string resolved_name = _resolve_screen_name(to_kick);

        pair_vector answer_query = {
            std::make_pair("chat_id", std::to_string(peer_id)),
            std::make_pair("user_id", resolved_name)
        };
        string query = cURL::generate_vk_query("messages.removeChatUser"
            , answer_query);
        cURL::request(query);
        std::cout << query << std::endl;
    }
}
#endif //BAN_COM

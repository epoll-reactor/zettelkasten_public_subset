#include "command_handler/command_handler.hpp"
#include "src/Lightning/Hash/sha256.hpp"

using lightning::SHA256;

#ifdef SHA256_COM
void Command_handler::sha256_command(const string_vector& text) {
    if (_message == "+sha256") {
        _if_empty_query();
    } else {
        SHA256 sha(_message.substr(text[0].size() + 1, _message.size() - 1).c_str());
        pair_vector answer_query;
        if (_nickname != "") {
            answer_query = {
                { "message", _formatted_nickname + sha.hash() },
                { "peer_id", _peer_id }
            };
        } else {
            answer_query = {
                { "message", sha.hash() },
                { "peer_id", _peer_id   }
            };
        }
        string query
      = cURL::generate_vk_query("messages.send", answer_query);
        cURL::request(query);
    }
}
#endif //SHA256_COM

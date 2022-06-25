#include "command_handler/command_handler.hpp"
#include "src/Lightning/Hash/crc32.hpp"

using lightning::CRC32;

#ifdef CRC32_COM
void Command_handler::crc32_command(const string_vector& text) {
    if (_message == "+crc32") {
        _if_empty_query();
    } else {
        unsigned long crc32 = CRC32::crc32gen(_message.substr(text[0].size() + 1,
            _message.size() - 1).c_str());
        std::stringstream stream;
        stream << "0x" << std::hex << crc32;
        string crc32_str(stream.str());
        pair_vector answer_query;
        if (_nickname != "") {
            answer_query = {
                { "message", _formatted_nickname + crc32_str },
                { "peer_id", _peer_id }
            };
        } else {
            answer_query = {
                { "message", crc32_str },
                { "peer_id", _peer_id  }
            };
        }
        string query
      = cURL::generate_vk_query("messages.send", answer_query);
        cURL::request(query);
    }
}
#endif //CRC32_COM

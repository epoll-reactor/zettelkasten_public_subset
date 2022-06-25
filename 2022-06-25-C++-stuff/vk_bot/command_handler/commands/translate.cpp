#include "command_handler/command_handler.hpp"

#ifdef TRANSLATE_COM
void Command_handler::translate_command(const string_vector &text) {
    string yandex_token = "trnsl.1.1.20190508T201810Z.385ebfa1e596baa0.90672cf8655555b1b51ced31b03c2e8bb9bde46c";
    if (_message == "+переводчик") {
        _if_empty_query();
    } else {
        string to_translate = _message.substr(text[0].size() + 1,
                                        _message.size() - 1);
        string translate_answer =
                cURL::request("https://translate.yandex.net/api/v1.5/tr.json/translate?key="
                              + yandex_token
                              + "&lang=ru&text="
                              + to_translate);
        std::cout << translate_answer << std::endl;
        auto parsed = nlohmann::json::parse(translate_answer);
        string translated = parsed["text"][1];
        pair_vector message = {
            { "message", translated },
            { "peer_id", _peer_id   }
        };
        string query
      = cURL::generate_vk_query("messages.send", message);
        cURL::request(query);
    }
}
//https://translate.yandex.net/api/v1.5/tr.json/translate?key=trnsl.1.1.20190508T201810Z.385ebfa1e596baa0.90672cf8655555b1b51ced31b03c2e8bb9bde46c&lang=ru&text=shit
//["text"][1]
#endif //TRANSLATE_COM

#include "command_handler/command_handler.hpp"

#ifdef WEATHER_COM
void Command_handler::weather_command(const string_vector &text) {
    if (_message == "+погода") {
        _if_empty_query();
    } else {
        string appid = "ef23e5397af13d705cfb244b33d04561";
        pair_vector query = {
            { "lang",  "ru"     },
            { "units", "metric" },
            { "APPID", appid    },
            { "q",     text[1]  }
        };
        string answer = cURL::request(cURL::generate_query(
                                      "http://api.openweathermap.org/data/2.5/weather?", query));
        auto parsed = nlohmann::json::parse(answer);
        if (!parsed["weather"].is_null()) {
        string description =
            parsed["weather"]
                  [0]
                  ["description"];
        int temp =
            parsed["main"]
                  ["temp"];

        int feels_like =
            parsed["main"]
                  ["feels_like"];

        int humidity =
            parsed["main"]
                  ["humidity"];

        string city_name =
            parsed["name"];

        string weather =
            "Сейчас в " + city_name + " " +
            std::to_string(temp) + "°C, " + description + "\nОщущается как " +
            std::to_string(feels_like)    + "°C\nВлажность: " +
            std::to_string(humidity)      + "%";
        pair_vector query = {
            { "message", weather  },
            { "peer_id", _peer_id }
        };
        cURL::request(cURL::generate_vk_query("messages.send", query));
        } else {
            pair_vector query;
            query = {
                { "message", "Нет такого города." },
                { "peer_id", _peer_id }
            };
            cURL::request(cURL::generate_vk_query("messages.send", query));
        }
    }
}
#endif //WEATHER_COM

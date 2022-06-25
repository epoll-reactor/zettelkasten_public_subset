#include "command_handler/command_handler.hpp"

void init_bot() {
//    раскомментируй это при первом запуске, чтобы создать таблицу
//    Database db;
//    db.init_table();
    pair_vector lp_parameters = {
        { "group_id", group_id }
    };
    while (true) {
        string lp_query =
            cURL::generate_vk_query("groups.getLongPollServer", lp_parameters);
        string get_lp   = cURL::request(lp_query);
        auto lp_parsed  = nlohmann::json::parse(get_lp);

        string key      = lp_parsed["response"]["key"];
        string server   = lp_parsed["response"]["server"];
        long ts_digit   = lp_parsed["response"]["ts"];
        string ts       = std::to_string(ts_digit);

        pair_vector query = {
            { "act",  "a_check" },
            { "key",   key      },
            { "ts",    ts       },
            { "wait", "60"      }
        };
        string lp = cURL::request(cURL::generate_query(server + "?", query));
        auto parsed = nlohmann::json::parse(lp);
        if (!parsed["updates"][0].is_null()) {
            if (parsed["updates"]
                      [0]
                      ["object"]
                      ["message"]
                      ["text"] != "")
            {
                string received_message =
                    parsed["updates"]
                          [0]
                          ["object"]
                          ["message"]
                          ["text"];
                long peer_id_digit =
                    parsed["updates"]
                          [0]
                          ["object"]
                          ["message"]
                          ["peer_id"];
                long from_id_digit =
                    parsed["updates"]
                          [0]
                          ["object"]
                          ["message"]
                          ["from_id"];
                string from_id = std::to_string(from_id_digit);
                string peer_id = std::to_string(peer_id_digit);
                Command_handler handler(received_message, peer_id, from_id);
                handler.init_commands();
            }
            ts_digit++;
        }
    }
}

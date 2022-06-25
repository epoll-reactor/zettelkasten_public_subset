#include "command_handler/command_handler.hpp"

string Command_handler::_resolve_screen_name(const string& screen_name) {
    pair_vector answer_query = {
        std::make_pair("screen_name", screen_name)
    };
    string query = cURL::generate_vk_query("utils.resolveScreenName", answer_query);
    string answer = cURL::request(query);
    auto parsed = nlohmann::json::parse(answer);
    long object_id = parsed["response"]["object_id"];
    return std::to_string(object_id);
}

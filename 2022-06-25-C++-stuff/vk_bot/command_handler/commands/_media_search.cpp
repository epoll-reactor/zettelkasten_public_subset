#include "command_handler/command_handler.hpp"

string Command_handler::_return_attachment_type(const string& method) {
    std::string type;
    if (method == "photos.search") {
        type = "photo";
    } else if (method == "video.search") {
        type = "video";
    } else if (method == "docs.search") {
        type = "doc";
    }
    return type;
}

pair_vector Command_handler::_if_media_not_found(const string& type) {
    pair_vector message;
    if (type == "photo") {
        message = {
            { "message", "Не найдено фотографий!" },
            { "peer_id", _peer_id }
        };
    }
    else if (type == "video") {
        message = {
            { "message", "Не найдено видеозаписей!" },
            { "peer_id", _peer_id }
        };
    }
    else if (type == "doc") {
        message = {
            { "message", "Не найдено такого документа!" },
            { "peer_id", _peer_id }
        };
    }
    return message;
}

void Command_handler::_media_search(string_vector tokenized_message, const string& method)
{
    std::string type = _return_attachment_type(method);

    string message_to_search =
        _message.substr(tokenized_message[0].size() + 1,
                       _message.size() - 1);

    pair_vector query = {
        { "q", message_to_search     },
        { "access_token", user_token },
        { "count", "100"             },
        { "v", api_version           }
    };
    string answer =
        cURL::request(cURL::generate_query(api_url + method + "?", query));
    auto parsed = nlohmann::json::parse(answer);
    long size = parsed["response"]["items"].size();
    long random;
    random = rand() % size - 1;
    if (size == 1) {
        random = 0;
    }
    if (size == 0) {
        pair_vector if_not_found = _if_media_not_found(type);
        string query = cURL::generate_vk_query("messages.send", if_not_found);
        cURL::request(query);
    }
    else {
        long owner_id_digit =
            parsed["response"]
                  ["items"]
                  [random]
                  ["owner_id"];
        long id_digit =
            parsed["response"]
                  ["items"]
                  [random]
                  ["id"];
         if (owner_id_digit != 0 && id_digit != 0) {
            string owner_id = std::to_string(owner_id_digit);
            string id = std::to_string(id_digit);

            pair_vector media_query;
            if (_nickname != "") {
                media_query = {
                    { "attachment", type + owner_id + "_" + id },
                    { "peer_id", _peer_id },
                    { "message", _formatted_nickname }
                };
            }
            else {
                media_query = {
                    { "attachment", type + owner_id + "_" + id },
                    { "peer_id", _peer_id }
                };
            }
            string query = cURL::generate_vk_query("messages.send", media_query);
            cURL::request(query);
        }
    }
}

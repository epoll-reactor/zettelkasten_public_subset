#ifndef CURL_HPP
#define CURL_HPP

#include <string>
#include <vector>
#include <curl/curl.h>
#include <json.hpp>
#include "../long_poll/token.hpp"

using std::string;
using pair_vector = std::vector<std::pair<string, string>>;

class cURL {
public:
    static string generate_vk_query(string server, pair_vector& query);
    static string generate_query(string server, pair_vector& query);
    static string urlencode(string& url);
    static string request(string url);
private:
    static size_t write_callback(void* contents,
                                 size_t size,
                                 size_t nmemb,
                                 void* userp);
};

#endif // CURL_HPP

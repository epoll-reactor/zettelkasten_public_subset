#include "curl.hpp"

std::string cURL::urlencode(std::string& url) { //костыль
    std::string urlen;
    for (char c : url) {
        if (c == ' ') {
            urlen += "%20";
        } else if (c == '\n') {
            urlen += "%0A";
        } else if (c == '+') {
            urlen += "%2B";
        } else {
            urlen += c;
        }
    }
    return urlen;
}

size_t cURL::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size* nmemb);
    return size * nmemb;
}

std::string cURL::request(std::string url) {
  std::cout << url << std::endl;
    CURL* curl;
    CURLcode res;
    std::string buffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cURL::urlencode(url).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
    }
    return buffer;
}

std::string cURL::generate_vk_query(std::string method, pair_vector &query) {
    std::string result = api_url;
    result = result + method + "?";
    result = result + "access_token" + "=" + access_token + "&";
    result = result + "v" + "=" + api_version + "&";
    for (auto element : query) {
        result = result + element.first + "=" + element.second + "&";
    }
    return result;
}

std::string cURL::generate_query(std::string url, pair_vector &query) {
    std::string result = url;
    for (auto element : query) {
        result = result + element.first + "=" + element.second + "&";
    }
    return result;
}

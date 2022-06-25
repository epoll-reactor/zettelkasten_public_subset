#pragma once

#include <map>
#include <string_view>

namespace curlpp
{
class Cleanup;
class Easy;
}

class client
{
public:
    std::string request(std::string_view body, const std::map<std::string_view, std::string_view>& params) const;
    std::size_t download(std::string_view filename, std::string_view server) const;
    std::string upload(std::string_view field_name, std::string_view filename, std::string_view server) const;
    std::string request_data(std::string_view body, std::string_view data) const;

    static std::string unescape(std::string_view text);
};

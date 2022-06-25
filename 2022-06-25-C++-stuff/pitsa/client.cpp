//#include <curlpp/cURLpp.hpp>
//#include <curlpp/Easy.hpp>
//#include <curlpp/Options.hpp>
//#include <sstream>

//#include "client.hpp"


//static std::string escape(std::string_view url)
//{
//    return curlpp::escape(url.data());
//}

//static size_t file_write(FILE* file, char* ptr, size_t size, size_t nmemb)
//{
//    return fwrite(ptr, size, nmemb, file);
//}

//static std::string genparams(const std::map<std::string_view, std::string_view>& body)
//{
//    std::string result;
//    for (const auto&[key, value] : body)
//    {
//        result += std::string(key) + '=' + escape(value) + '&';
//    }
//    return result;
//}

//std::string client::request(std::string_view method, const std::map<std::string_view, std::string_view>& params) const
//{
//    std::ostringstream response;
//    curlpp::Easy curl_easy;

//    std::cout << method.data() + genparams(params) << std::endl;

//    curl_easy.setOpt(new curlpp::options::Url(method.data() + genparams(params)));
//    curl_easy.setOpt(new curlpp::options::WriteStream(&response));
//    curl_easy.perform();

//    return response.str();
//}

//std::string client::request_data(std::string_view method, std::string_view data) const
//{
//    std::ostringstream response;

//    curlpp::Easy curl_easy;
//    curl_easy.setOpt(new curlpp::options::Url(method.data()));
//    curl_easy.setOpt(new curlpp::options::PostFields(data.data()));
//    curl_easy.setOpt(new curlpp::options::PostFieldSize(data.size()));
//    curl_easy.setOpt(new curlpp::options::WriteStream(&response));
//    curl_easy.perform();

//    return response.str();
//}

//std::string client::unescape(std::string_view text)
//{
//    return curlpp::unescape(text.data());
//}

//std::size_t client::download(std::string_view filename, std::string_view server) const
//{
//    FILE* fp = fopen(filename.data(), "w");
//    if (not fp)
//    {
//        return -1;
//    }

//    curlpp::Easy curl_easy;
//    curlpp::options::WriteFunction* writef =
//        new curlpp::options::WriteFunction(std::bind(
//            &file_write, fp,
//            std::placeholders::_1,
//            std::placeholders::_2,
//            std::placeholders::_3
//    ));
//    curl_easy.setOpt(writef);
//    curl_easy.setOpt(new curlpp::options::Url(server.data()));
//    curl_easy.perform();
//    fclose(fp);
//    return 0;
//}

//std::string client::upload(std::string_view field_name, std::string_view filename, std::string_view server) const
//{
//    std::ostringstream response;
//    curlpp::Forms formParts;
//    formParts.push_back(new curlpp::FormParts::File(field_name.data(), filename.data()));

//    curlpp::Easy curl_easy;
//    curl_easy.setOpt(new curlpp::options::Url(server.data()));
//    curl_easy.setOpt(new curlpp::options::HttpPost(formParts));
//    curl_easy.setOpt(new curlpp::options::WriteStream(&response));
//    curl_easy.perform();

//    return response.str();
//}

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "../network/curl.hpp"
#include "../tokenizer/tokenizer.hpp"
#include "../sqlite/sqlite.hpp"

using std::string;

class Command_handler {
public:
#define CRC32_COM
#define SHA256_COM
#define PIC_COM
#define VIDEO_COM
#define DOC_COM
#define HELP_COM
#define BAN_COM
#define WEATHER_COM
#define TRANSLATE_COM
#define NICKNAME_COM

/* undef if you want to disable command */
//#undef CRC32_COM
//#undef SHA256_COM
//#undef PIC_COM
//#undef VIDEO_COM
//#undef DOC_COM
#undef BAN_COM
//#undef WEATHER_COM
//#undef HELP_COM
#undef TRANSLATE_COM
//#undef NICKNAME_COM

    Command_handler() = delete;
    Command_handler& operator=(Command_handler&) = delete;
    Command_handler(const string& message, const string& peer_id, const string& from_id);

#ifdef CRC32_COM
    void crc32_command(const string_vector& text);
#endif

#ifdef SHA256_COM
    void sha256_command(const string_vector& text);
#endif

#ifdef PIC_COM
    void picture_command(const string_vector& text);
#endif

#ifdef VIDEO_COM
    void video_command(const string_vector& text);
#endif

#ifdef DOC_COM
    void document_command(const string_vector& text);
#endif

#ifdef WEATHER_COM
    void weather_command(const string_vector& text);
#endif

#ifdef TRANSLATE_COM
    void translate_command(const string_vector& text);
#endif

#ifdef HELP_COM
    void help_command();
#endif

#ifdef BAN_COM
    void ban_command(const string_vector& text);
#endif

#ifdef NICKNAME_COM
    void add_nickname_command(const string& user_id, const string& prefix);
    void remove_nickname_command(const string& user_id);
#endif

    void init_commands();

private:
    void _help_autogen();
    void _if_empty_query();
    string _return_attachment_type(const string& method);
    pair_vector _if_media_not_found(const string& type);
    void _media_search(string_vector tokenized_message,
                       const string& method);
    Database database;
    string_vector _help_info_vector;
    string _resolve_screen_name(const string& screen_name);
    string _message;
    string _peer_id;
    string _from_id;
    string _nickname = database.return_nickname(_from_id);
    string _formatted_nickname = "@id" + _from_id + "(" + _nickname + "), ";
};

void init_bot();

#endif // COMMAND_HANDLER_HPP

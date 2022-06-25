#ifndef SQLITE_HPP
#define SQLITE_HPP
#define EMPTY 0

#include <sqlite3.h>
#include <string>
#include <stdexcept>
#include <vector>

using std::string;

class Database {
public:
    Database& operator=(Database&) = delete;
    void init_table();
    void insert_nickname(const string& user_id, const string& prefix);
    string return_nickname(const string& user_id);
private:
    sqlite3* database;
    int rc = 0;
    char* err = nullptr;
    static int callback(void* not_used, int argc, char* argv[], char** col_name);
};

#endif //SQLITE_HPP

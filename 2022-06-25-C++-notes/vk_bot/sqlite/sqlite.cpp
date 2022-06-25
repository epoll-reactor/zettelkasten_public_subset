#include "sqlite.hpp"
#include <iostream>

int Database::callback(void* not_used,
                       int argc,
                       char** argv,
                       char** col_name)
{
    return 0;
}

void Database::init_table() {
    rc = sqlite3_open("users.db", &database);
    if (rc) {
        throw std::invalid_argument(sqlite3_errmsg(database));
    }
    string sql =
        "CREATE TABLE USERS ("
        "USER_ID TEXT NOT NULL UNIQUE,"
        "PREFIX  TEXT NOT NULL);";
    rc = sqlite3_exec( database, sql.c_str(), callback, 0, &err);
}

void Database::insert_nickname(const string& user_id, const string& prefix) {
    sqlite3_open("users.db", &database);
    string query =
        "REPLACE INTO USERS"
        "('USER_ID', 'PREFIX')"
        "VALUES ('" + user_id + "'," +
                "'" + prefix  + "');";
    rc = sqlite3_exec(database, query.c_str(), callback, 0, &err);
}

string Database::return_nickname(const string& user_id) {
    sqlite3_open("users.db", &database);
    std::vector<string> result;
    sqlite3_stmt* stmt;
    char query[255];

    snprintf (
        query,
        sizeof query - 1,
        "SELECT PREFIX FROM USERS WHERE USER_ID = %s;",
        user_id.c_str()
    );
    rc = sqlite3_prepare(database, query, sizeof (query), &stmt, NULL);
    if (rc == 0) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            result.push_back((const char*)sqlite3_column_text(stmt, 0));
        }
    }
    if (result.size() == EMPTY) {
        return "";
    } else {
        return result[0];
    }
}




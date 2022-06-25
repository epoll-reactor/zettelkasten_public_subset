TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -lcurl -Wall -Wextra -O2
QMAKE_MAKEFILE += -j8
LIBS += -lcurl -lsqlite3

SOURCES += \
        /usr/local/include/src/Lightning/Hash/crc32.cpp \
        /usr/local/include/src/Lightning/Hash/sha256.cpp \
        command_handler/commands/nickname.cpp \
        command_handler/commands/translate.cpp \
        long_poll/init_bot.cpp \
        main.cpp \
        tokenizer/tokenizer.cpp \
        network/curl.cpp \
        command_handler/commands/ban.cpp \
        command_handler/command_handler.cpp \
        command_handler/commands/_resolve_screen_name.cpp \
        command_handler/commands/_media_search.cpp \
        command_handler/commands/crc.cpp \
        command_handler/commands/sha.cpp \
        command_handler/commands/pic.cpp \
        command_handler/commands/video.cpp \
        command_handler/commands/doc.cpp \
        command_handler/commands/help.cpp \
        command_handler/commands/weather.cpp \
        sqlite/sqlite.cpp

HEADERS += \
        command_handler/command_handler.hpp \
        network/curl.hpp \
        long_poll/token.hpp \
        tokenizer/tokenizer.hpp \
        sqlite/sqlite.hpp


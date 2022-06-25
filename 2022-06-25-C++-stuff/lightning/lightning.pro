TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -O2 -Wall -Wextra

SOURCES += \
        src/Lightning/Hash/crc16.cpp            \
        src/Lightning/Hash/crc32.cpp            \
        src/Lightning/Hash/crc8.cpp             \
        src/Lightning/Hash/sha256.cpp           \
        src/Lightning/Line/line.cpp             \
        src/Lightning/Regex/regex.cpp           \
        src/main.cpp

HEADERS += \
        src/Lightning/Allocator/allocator.hpp   \
        src/Lightning/Hash/crc16.hpp            \
        src/Lightning/Hash/crc32.hpp            \
        src/Lightning/Hash/crc8.hpp             \
        src/Lightning/Hash/sha256.hpp           \
        src/Lightning/Array/array.hpp           \
        src/Lightning/Array/array.tcc           \
        src/Lightning/Math/math.hpp             \
        src/Lightning/Math/math.tcc             \
        src/Lightning/Complex/complex.hpp       \
        src/Lightning/Complex/complex.tcc       \
        src/defines.hpp                         \
        src/Lightning/namespace/lightning.hpp   \
        src/Lightning/Line/line.hpp             \
        src/Lightning/Regex/regex.hpp

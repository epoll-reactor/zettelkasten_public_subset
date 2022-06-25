TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = clang++

QMAKE_CXXFLAGS += \
#      -march=native -mtune=native -Wextra -Wall
#      -ffast-math -mavx512vl -m64 -ffast-math -fno-protect-parens -fstack-arrays -ftree-vectorize
#      -rdynamic

LIBS += -lpthread

SOURCES += \
#    client.cpp \
  list.cpp \
    logger.cpp \
    main.cpp \
#    sha256.cpp \
#    simd.cpp \
 \#    thread_pool.cpp
#  memory_pool.cpp

HEADERS += \
#    backtrace.hpp \
#    client.hpp \
#    crc32.hpp \
#    exception.hpp \
#    format.hpp \
    logger.hpp \
    matrix.hpp \
    matrix_test.hpp \
#    parser.hpp \
  list.hpp \
#  logger2.hpp \
#  my_logger.hpp \
#    root_certificates.hpp \
#    sha256.hpp \
#    simd.hpp \
#  memory_pool.hpp \
#    testSpeed.hpp \
#    thread_pool.hpp \
 \#    vector.hpp
  sub_not_pattern.hpp

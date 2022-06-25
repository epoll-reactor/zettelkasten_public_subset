// Вам дан класс Writer, который умеет писать данные на экран, в файл или в память –
// в общем, неважно куда. У этого класса есть виртуальная функция Write, принимающая
// на вход указатель на байты памяти и их количество. Функция пытается записать эти
// байты в нужное место. Выглядит этот класс примерно так: https://clck.ru/CbtZu.

// Однако операция вывода имеет накладные расходы, и частые вызовы Write с маленькими
// размерами данных могут замедлять программу. Вам надо написать класс-наследник
// BufferedWriter, который осуществлял бы буферизованный вывод. Конструктор этого
// класса должен принимать параметр типа size_t – размер буфера. Переопределенная
// в этом классе функция Write должна сначала дописывать данные в этот внутренний
// буфер, и только когда он заполнится – вызывать Write базового класса с данными
// этого буфера. В конце использования класса оставшиеся в буфере данные также должны
// быть записаны через вызов функции Write базового класса. Максимально возможный
// размер буфера, переданный в конструкторе, в процессе работы не должен изменяться.

#pragma once

#include <iostream>


class Writer {
public:
  virtual void Write(const char * data, size_t len) {
    (void)len;
    std::cout << data;
  }
};


class BufferedWriter : public Writer {
public:
  explicit BufferedWriter(size_t bufsize)
    : bufsize_(bufsize), has_rest_(false) { }

  ~BufferedWriter() {
    if (has_rest_) {
      Writer::Write(buffer_.c_str(), buffer_.size());
      buffer_.clear();
    }
  }
  void Write(const char* data, size_t len) override {
    for (size_t i = 0; i < len; i++) {
      buffer_.push_back(data[i]);
      if (buffer_.size() == bufsize_) {
        Writer::Write(buffer_.c_str(), buffer_.size());
        buffer_.clear();
      }
      if (i == len - 1 && !buffer_.empty()) {
        has_rest_ = true;
      }
    }
  }
private:
  const size_t bufsize_;
  std::string buffer_;
  bool has_rest_;
};

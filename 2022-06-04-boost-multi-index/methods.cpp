#include <boost/multi_index_container.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <iostream>
#include <string>

namespace mi = boost::multi_index;

struct name_record {
public:
  name_record(std::string given_name, std::string family_name)
    : given_name(given_name), family_name(family_name) {}

  std::string name() const {
    return family_name + " " + given_name;
  }

private:
  std::string given_name, family_name;
};

size_t name_record_length(const name_record &r) {
  return r.name().size();
}

using name_record_set = mi::multi_index_container<
  name_record,
  mi::indexed_by<
    mi::ordered_unique<
      // Сортировка с помощью метода класса.
      mi::const_mem_fun<name_record, std::string, &name_record::name>
    >,
    mi::ordered_non_unique<
      // Сортировка чисел (длин строк), возвращаемых свободной функцией.
      mi::global_fun<const name_record &, size_t, name_record_length>
    >
  >
>;

int main() {
  name_record_set set;
  set.insert(name_record("Joe", "Smith"));
  set.insert(name_record("Robert", "Nightingale"));
  set.insert(name_record("Robert", "Brown"));
  set.insert(name_record("Marc", "Tuxedo"));

  std::cout << "Phonebook order:\n";
  for (auto it : set.get<0>()) // Индекс шаблонных парамеров indexed_by.
    std::cout << "  " << it.name() << '\n';

  std::cout << "Length order:\n";
  for (auto it : set.get<1>()) // Индекс шаблонных парамеров indexed_by.
    std::cout << "  " << it.name() << '\n';
}

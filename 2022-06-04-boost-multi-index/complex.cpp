//https://www.boost.org/doc/libs/1_36_0/libs/multi_index/example/complex_structs.cpp
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <string>

namespace multi_index = boost::multi_index;

template <typename _1, typename _2>
class merge_keys {
  _1 key1;
  _2 key2;

public:
  using result_type = typename _1::result_type;

  merge_keys(_1 key1 = _1(), _2 key2 = _2())
    : key1(key1), key2(key2) {}

  template <typename Arg>
  result_type operator()(Arg &arg) const {
    return key1(key2(arg));
  }
};

struct by_model {};
struct by_manufacturer {};
struct by_price {};

struct car_manufacturer {
  std::string name;

  car_manufacturer(std::string name)
    : name(std::move(name)) {}
};

struct car_model {
  std::string             model;
  const car_manufacturer *manufacturer;
  int                     price;

  car_model(std::string model, const car_manufacturer *manufacturer, int price)
    : model(model), manufacturer(manufacturer), price(price) {}

  friend std::ostream &operator<<(std::ostream &stream, const car_model &m) {
    return stream << m.manufacturer->name << " "
                  << m.model << " $"
                  << m.price << "\n";
  }
};

using car_manufacturer_table = boost::multi_index_container<
  car_manufacturer,
  multi_index::indexed_by<
    multi_index::ordered_unique<
      multi_index::member<car_manufacturer, std::string, &car_manufacturer::name>
    >
  >
>;

using car_table = boost::multi_index_container<
  car_model,
  multi_index::indexed_by<
      multi_index::ordered_unique<
      multi_index::tag<by_model>,
      multi_index::member<car_model, std::string, &car_model::model>
    >,
    multi_index::ordered_non_unique<
      multi_index::tag<by_manufacturer>,
      merge_keys<
        multi_index::member<car_manufacturer, const std::string, &car_manufacturer::name>,
        multi_index::member<car_model, const car_manufacturer *, &car_model::manufacturer>
      >
    >,
    multi_index::ordered_non_unique<
      multi_index::tag<by_price>,
      multi_index::member<car_model, int, &car_model::price>
    >
  >
>;

using car_table_price_view = boost::multi_index_container<
  const car_model *,
  multi_index::indexed_by<
    multi_index::ordered_non_unique<
      multi_index::member<car_model, const int, &car_model::price>
    >
  >
>;

using car_table_manufacturer_view = boost::multi_index_container<
  const car_model *,
  multi_index::indexed_by<
    multi_index::ordered_non_unique<
      merge_keys<
        multi_index::member<car_manufacturer, const std::string, &car_manufacturer::name>,
        multi_index::member<car_model, const car_manufacturer * const, &car_model::manufacturer>
      >
    >
  >
>;

void write_in_car_table(car_table &table, car_manufacturer_table &manufacturer_table) {
  const auto *cadillac =
      &*(manufacturer_table.insert(car_manufacturer("Cadillac")).first);
  const auto *ford =
      &*(manufacturer_table.insert(car_manufacturer("Ford")).first);
  const auto *bmw =
      &*(manufacturer_table.insert(car_manufacturer("BMW")).first);
  const auto *audi =
      &*(manufacturer_table.insert(car_manufacturer("Audi")).first);

  table.insert(car_model("XLR",cadillac,76200));
  table.insert(car_model("SRX",cadillac,38690));
  table.insert(car_model("CTS",cadillac,30695));
  table.insert(car_model("Escalade",cadillac,54770));
  table.insert(car_model("ESV",cadillac,57195));
  table.insert(car_model("EXT",cadillac,52045));
  table.insert(car_model("Deville",cadillac,45195));
  table.insert(car_model("Seville",cadillac,46330));

  table.insert(car_model("ZX2",ford,15355));
  table.insert(car_model("Thunderbird",ford,43995));
  table.insert(car_model("Windstar",ford,35510));
  table.insert(car_model("Focus",ford,19630));
  table.insert(car_model("Taurus",ford,24290));
  table.insert(car_model("Mustang",ford,39900));
  table.insert(car_model("Crown Victoria",ford,30370));

  table.insert(car_model("325i",bmw,27800));
  table.insert(car_model("545i",bmw,54300));
  table.insert(car_model("745i",bmw,68500));
  table.insert(car_model("M3 coupe",bmw,46500));
  table.insert(car_model("Z4 roadster 3.0i",bmw,40250));
  table.insert(car_model("X5 4.4i",bmw,49950));

  table.insert(car_model("A4 1.8T",audi,25940));
  table.insert(car_model("TT Coupe",audi,33940));
  table.insert(car_model("A6 3.0",audi,36640));
  table.insert(car_model("Allroad quattro 2.7T",audi,40640));
  table.insert(car_model("A8 L",audi,69190));
}

int main() {
  car_table car_table;
  car_manufacturer_table manufacturer_table;

  write_in_car_table(car_table, manufacturer_table);

  std::cout << "Enter a manufacturer: ";
  std::string manufacturer_name;
  std::getline(std::cin, manufacturer_name);

  auto it = manufacturer_table.find(manufacturer_name);

  if(it == manufacturer_table.end()) {
    std::cout << "no such manufacturer in the table" << std::endl;
    return 0;
  }

  int min_price, max_price;
  std::cout << "Enter a minimum price: " << std::endl;
  std::cin >> min_price;
  std::cout << "Enter a maximum price: " << std::endl;
  std::cin >> max_price;

  {
    // Method 1.
    multi_index::index_iterator<::car_table, by_manufacturer>::type begin, end;
    // Get a pair of iterators with cars of given brand.
    boost::tuples::tie(begin, end) =
        get<by_manufacturer>(car_table).equal_range(manufacturer_name);

    // Push all to view, sorted by price.
    car_table_price_view view;
    while (begin != end)
      view.insert(&*begin++);

    // Selected by member<car_model, const int, &car_model::price>
    // in car_table_price_view.
    auto view_begin = view.lower_bound(min_price);
    auto view_end = view.upper_bound(max_price);
    if (view_begin == view_end) {
      std::cout << "No cars in range given.\n";
      return 0;
    }

    std::cout << "Listing by method 1\n";
    while (view_begin != view_end) {
      std::cout << **view_begin;
      ++view_begin;
    }
    std::cout << std::endl;
  }

  {
    // Method 2.
    // Get range between min and max prices.
    auto begin = get<by_price>(car_table).lower_bound(min_price);
    auto end = get<by_price>(car_table).upper_bound(max_price);

    car_table_manufacturer_view view;
    while (begin != end)
      view.insert(&*begin++);

    // Get range of cars of given brand.
    car_table_manufacturer_view::iterator view_begin, view_end;
    boost::tuples::tie(view_begin, view_end) = view.equal_range(manufacturer_name);

    if (view_begin == view_end) {
      std::cout << "No cars in range given.\n";
      return 0;
    }

    std::cout << "Listing by method 2:\n";
    while (view_begin != view_end) {
      std::cout << **view_begin;
      ++view_begin;
    }
    std::cout << std::endl;
  }
}

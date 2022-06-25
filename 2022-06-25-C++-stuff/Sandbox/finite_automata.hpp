// Oct 8, 2021
#ifndef FINITE_AUTOMATA_HPP
#define FINITE_AUTOMATA_HPP

#include <map>
#include <iostream>

template<typename State, typename Transition = State>
class finite_automata {
public:
  finite_automata() {
    set(State{});
  }

  void set(const State& state) {
    current_state_ = state;
  }

  const State& get() const {
    return current_state_;
  }

  void clear() {
    state_pool_.clear();
  }

  void add(const State& state, const Transition& transition, const State& next) {
    state_pool_[state][transition] = next;
  }

  bool process(const Transition& transition) {
    const auto& transitions = state_pool_[current_state_];
    const auto& found = transitions.find(transition);
    if (found == transitions.end()) {
      return false;
    }
    const auto& next = found->second;
    set(next);
    return true;
  }

  bool process() {
    return process(get());
  }

  template<typename Container>
  bool get_valid_transitions(const State& state, Container& container) {
    container.clear();
    const auto& found = state_pool_.find(state);
    if (found == state_pool_.end()) {
      return false;
    }
    const auto& transitions = found->second;
    if (transitions.empty()) {
      return false;
    }
    for (const auto& iterator : transitions) {
      const auto& transition = iterator.first;
      container.push_back(transition);
    }
    return true;
  }

  template<typename Container>
  bool get_valid_transitions(Container& container) {
    return get_valid_transitions(get(), container);
  }

private:
  State current_state_;
  std::map<State, std::map<Transition, State>> state_pool_;
};

#include <set>
#include <list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <random>

template<typename Delimiter>
std::vector<std::string_view> split(std::string_view data, Delimiter&& delim) {
  std::vector<std::string_view> splitted;
  splitted.reserve(data.length() / 2);
  size_t pos = 0;
  while (pos != std::string_view::npos) {
    pos = data.find_first_not_of(delim);
    if (pos == std::string_view::npos) {
      return splitted;
    }
    data = data.substr(pos);
    pos = data.find_first_of(delim);
    splitted.emplace_back(data.substr(0, pos));
    data = data.substr(pos == std::string_view::npos ? 0 : pos);
  }
  splitted.shrink_to_fit();
  return splitted;
}

template<typename Iterator, typename Execution>
void for_each(Iterator begin, Iterator end, Execution exec) {
  for (; begin != end; ++begin) {
    exec(*begin);
  }
}

template<typename FiniteAutomata>
static void set_dialog_states(FiniteAutomata& fa) {
  fa.add("инит", "выход", "выход");
  fa.add("инит", "говори", "говори");
  fa.add("говори", "выход", "выход");
  fa.add("говори", "вежливо", "вежливо");
  fa.add("говори", "агрессивно", "агрессивно");
  fa.add("агрессивно", "говори", "говори");
  fa.add("вежливо", "говори", "говори");
  fa.set("инит");
}

template<typename FiniteAutomata>
static void print_current_state(std::string_view current_state, FiniteAutomata& fa) {
  std::list<std::string> transitions;
  fa.get_valid_transitions(transitions);
  std::string options;
  for (const auto& t : transitions) {
    if (!options.empty()) {
      options += ", ";
    }
    options += t;
  }
  std::cout << "[" << current_state << "] Waiting for input (" << options << "): " << std::endl;
}

void run_finite_automata_user_input() {
  finite_automata<std::string> automata;
  automata.add("старт", "выход", "выход");
  automata.add("старт", "меню", "меню");
  automata.add("меню", "1", "1");
  automata.add("меню", "2", "2");
  automata.add("меню", "3", "3");
  automata.add("1", "меню", "меню"); /// Should be implicit
  automata.add("2", "меню", "меню"); /// Should be implicit
  automata.add("3", "меню", "меню"); /// Should be implicit
  automata.add("меню", "выход", "старт");
  automata.set("старт");
  std::multiset<std::string> selected;
  while (true) {
    std::string current_state = automata.get();
    if (current_state == "выход") {
      break;
    } else if (current_state == "старт") {

    } else if (current_state == "меню") {

    } else {
      int selected_item = 0;
      try {
        selected_item = std::stoi(current_state);
      } catch (std::exception&) {
        std::cout << "Wrong item: " << current_state << std::endl;
        break;
      }
      selected.emplace(std::to_string(selected_item));
      automata.process("меню");
    }
    print_current_state(current_state, automata);
    std::string transition;
    std::cout << " > ";
    std::cin >> transition;
    if (!automata.process(transition)) {
      std::cout << "Error: invalid transition: " << transition << std::endl;
    }
  }
  std::string items;
  for (const auto& s : selected) {
    if (!items.empty()) {
      items += ", ";
    }
    items += s;
  }
  if (!items.empty()) {
    std::cout << "You выбор: " << items << std::endl;
  }
}

void run_finite_automata_test() {
  finite_automata<std::string> automata;
  automata.add("ready", "выход", "выход");
  automata.add("ready", "старт", "старт");
  automata.add("старт", "выход", "выход");
  automata.add("старт", "select", "select");
  automata.add("select", "1", "1");
  automata.add("select", "2", "2");
  automata.add("select", "3", "3");
  automata.add("1", "select", "select"); /// Should be implicit
  automata.add("2", "select", "select"); /// Should be implicit
  automata.add("3", "select", "select"); /// Should be implicit
  automata.add("select", "выход", "старт");
  automata.set("ready");
  std::multiset<std::string> selected;
  while (true) {
    std::string current_state = automata.get();
    if (current_state == "выход") {
      break;
    } else if (current_state == "старт") {

    } else if (current_state == "ready") {

    } else if (current_state == "select") {

    } else {
      int selected_item = 0;
      try {
        selected_item = std::stoi(current_state);
      } catch (std::exception&) {
        std::cout << "Wrong item: " << current_state << std::endl;
        break;
      }
      selected.emplace(std::to_string(selected_item));
      automata.process("select");
    }
    print_current_state(current_state, automata);
    std::string transition;
    std::cout << " > ";
    std::cin >> transition;
    if (!automata.process(transition)) {
      std::cout << "Error: invalid transition: " << transition << std::endl;
    }
  }
  std::string items;
  for (const auto& s : selected) {
    if (!items.empty()) {
      items += ", ";
    }
    items += s;
  }
  if (!items.empty()) {
    std::cout << "You select: " << items << std::endl;
  }
}

#endif // FINITE_AUTOMATA_HPP

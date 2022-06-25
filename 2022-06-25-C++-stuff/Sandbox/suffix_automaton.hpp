// Feb 13, 2022
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <stack>
#include <cassert>
#include <set>
#include <codecvt>
#include <locale>

class suffix_automaton {
private:
  class state {
  private:
    int                  length_;
    int                  link_;
    int                  first_;
    int                  index_;
    std::vector<int>     suffix_references_;
    std::map<char16_t, int> transitions_;

    friend class suffix_automaton;

    void add_transition(char16_t symbol, int new_idx) {
      transitions_.emplace(symbol, new_idx);
    }

    int get_transition(char16_t new_symbol) {
      if (auto it = transitions_.find(new_symbol); it != transitions_.end()) {
        return it->second;
      }
      return -1;
    }

    void update_transition(char16_t new_symbol, int new_idx) {
      if (auto it = transitions_.find(new_symbol); it != transitions_.end()) {
        it->second = new_idx;
      }
    }
  };

  bool suffix_references_;
  std::vector<state> states_;

public:
  suffix_automaton() = delete;
  suffix_automaton(const std::u16string& input)
    : suffix_references_(false) {
    build_automaton(input);
  }

  bool contains(const std::u16string& input) {
    int transition = 0;
    for (char16_t symbol : input) {
      transition = states_[transition].get_transition(symbol);
      if (transition == -1) {
        return false;
      }
    }
    return true;
  }

  int first(const std::u16string& input) {
    int transition = 0;
    for (char16_t symbol : input) {
      transition = states_[transition].get_transition(symbol);
      if (transition == -1) {
        return -1;
      }
    }
    return normalized_string_position(transition, input);
  }

  std::set<int> positions(const std::u16string& input) {
    if (!suffix_references_) {
      compute_suffix_references();
    }
    int transition = 0;
    for (char16_t symbol : input) {
      transition = states_[transition].get_transition(symbol);
      if (transition == -1) {
        return {};
      }
    }
    // Traverse link tree down from first occurrence to find all others
    std::stack<int> stack{{transition}};
    std::set<int> positions;
    while (stack.size() > 0) {
      transition = stack.top();
      stack.pop();
      positions.emplace(normalized_string_position(transition, input));
      for (int reference : states_[transition].suffix_references_) {
        stack.push(reference);
      }
    }
    return positions;
  }

private:
  void build_automaton(const std::u16string& input) {
    state st;
    st.link_   = -1;
    st.length_ =  0;
    st.index_  =  0;
    int last   =  0;
    states_.push_back(st);

    for (char16_t symbol : input) {
      // Create a new state for a new equivalence class
      int current_state = add_state(states_[last].length_ + 1);
      // Mark the ending position of the first occurrence of this state
      states_[current_state].first_ = states_[last].length_;
      // Keep following links until we find a transition through current_state
      int linked = last;
      int transition = states_[linked].get_transition(symbol);
      bool done = false;
      while (transition == -1) {
        states_[linked].add_transition(symbol, states_[current_state].index_);
        if (states_[linked].link_ != -1) {
          linked = states_[linked].link_;
          transition = states_[linked].get_transition(symbol);
        } else {
          // We have climbed the link tree to the root.
          // Add current as a child of the root in the link tree and
          // process the next character
          states_[current_state].link_ = 0;
          last = current_state;
          done = true;
          break;
        }
      }
      if (done) {
        continue;
      }
      int state_1 = linked;
      int state_2 = transition;
      if (states_[state_2].length_ == states_[state_1].length_ + 1) {
        // Current is a child of state_2 in the link tree, process next character
        states_[current_state].link_ = states_[state_2].index_;
        last = current_state;
        continue;
      }
      // Current is not a child of state_2 in the link tree, we must create a new
      // state that will be the parent of both state_1 and current in the link tree
      int clone = add_state(states_[state_1].length_ + 1);
      states_[clone].link_          = states_[state_2].link_;
      states_[clone].transitions_   = states_[state_2].transitions_;
      states_[clone].first_         = states_[state_2].first_;
      states_[current_state].link_  = states_[clone].index_;
      states_[state_2].link_        = states_[clone].index_;

      linked = state_1;
      while (transition == state_2 && linked != -1) {
        states_[linked].update_transition(symbol, clone);
        linked = states_[linked].link_;
        if (linked != -1) {
          transition = states_[linked].get_transition(symbol);
        }
      }
      // We are finished, advance last to the new state and continue
      last = current_state;
    }

    // We now want to mark every terminal state. We start with last, as
    // it is obviously a terminal state. By climbing the suffix links, we
    // find the state that corresponds to the next largest suffix that
    // is of a different equivalence class. This will be a terminal state
    // as well. So on and so forth until we hit the root of the link tree.
    int link = states_[last].link_;
    while (link != -1) {
      int linked = link;
      link = states_[linked].link_;
    }
  }

  int add_state(int length) {
    state new_state;
    new_state.length_ = length;
    new_state.index_ = states_.back().index_ + 1;
    states_.push_back(new_state);
    return new_state.index_;
  }

  void compute_suffix_references() {
    for (int i = 1; i < states_.size(); i++) {
      states_[states_[i].link_].suffix_references_.push_back(i);
    }
    suffix_references_ = true;
  }

  int normalized_string_position(int transition, const std::u16string& input) {
    return states_[transition].first_ - input.size() + 1;
  }
};

#include "function_benchmarking.hpp"

void test() {
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
  {
    suffix_automaton sa(u"text");
    assert(sa.contains(u""));
    assert(sa.contains(u"t"));
    assert(sa.contains(u"ex"));
    assert(sa.contains(u"tex"));
    assert(sa.contains(u"text"));
    assert(!sa.contains(u"text_"));
    assert(!sa.contains(u"other text"));
    assert(!sa.contains(u"\xFF"));
  } {
    suffix_automaton sa(u"text");
    assert(sa.positions(u"t") == std::set<int>({0, 3}));
    assert(sa.positions(u"__") == std::set<int>({}));
  } {
    std::u16string input = u"текст";
    std::u16string to_find = u"т";
    suffix_automaton automaton(input);
    std::cout << "For string: " << converter.to_bytes(input) << std::endl;
    auto occurrences = automaton.positions(to_find);
    if (occurrences.empty()) {
      std::cout << "No occurrences was found for " << converter.to_bytes(to_find) << std::endl;
      return;
    }
    std::cout << converter.to_bytes(to_find) << " occurs at positions ";
    for (size_t i : occurrences) {
      std::cout << i << ' ';
    }
    std::cout << std::endl;
    for (int occurrence : occurrences) {
      std::cout << "# " << converter.to_bytes(input.substr(0, occurrence));
      std::cout << "(" << converter.to_bytes(input.substr(occurrence, to_find.length())) << ")";
      std::cout << converter.to_bytes(input.substr(occurrence + 1)) << std::endl;
    }
  } {
    suffix_automaton sa(u"a aa b aa c aa");
    auto positions = sa.positions(u"aa");
    assert(sa.positions(u"aa") == std::set<int>({2, 7, 12}));
  } {
    suffix_automaton sa(u"text");
    assert(sa.first(u"ex") == 1);
  } {
    constexpr char16_t lorem_ipsum_text[] =
     u"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis sodales, ipsum nec molestie "
      "cursus, urna nulla pellentesque enim, sodales euismod magna augue et lacus. Donec fermentum "
      "lacus at diam dignissim, in auctor mi fermentum. Morbi congue ornare blandit. Ut sodales "
      "pulvinar purus. Cras ultricies nulla quis nibh euismod consequat. Sed malesuada ac lacus non "
      "vulputate. Etiam eget interdum purus, et tincidunt sem. Integer luctus pellentesque turpis a "
      "vulputate. Quisque cursus lorem in diam consectetur porttitor. Praesent at facilisis quam. "
      "Aenean molestie fringilla nibh, a lacinia est fermentum ac. Sed sollicitudin sed sem a volutpat. "
      "Donec eget justo eget neque semper euismod sit amet eu urna. Integer imperdiet nulla sit amet "
      "ligula laoreet aliquam. Quisque non auctor nibh. Suspendisse potenti. Cras efficitur, libero "
      "sit amet tincidunt volutpat, leo leo semper turpis, eget viverra eros libero sit amet diam. "
      "Integer non justo lacinia, volutpat sem nec, fermentum elit. Duis non venenatis nibh. Donec "
      "nec velit a felis ullamcorper scelerisque. Etiam ultrices, risus et vestibulum lacinia, leo "
      "dolor ornare urna, eu laoreet nulla purus aliquet elit. Phasellus lorem elit, malesuada non "
      "mauris vel, fermentum consequat nunc. Praesent nec justo sed urna ultrices dapibus eu sed quam. "
      "Morbi at ex ac ante venenatis accumsan. Maecenas mattis nulla dictum, placerat neque nec, cursus "
      "lorem. Fusce condimentum finibus tempor. Sed ac fringilla augue. Quisque at commodo libero, vel "
      "volutpat leo. Praesent libero tortor, ullamcorper id orci tristique, ultrices vulputate justo. "
      "Maecenas auctor tincidunt mi vitae pharetra. Nam ac felis nec purus tristique laoreet non pulvinar "
      "felis. Mauris scelerisque fringilla turpis vel blandit. Mauris in nulla justo. Aliquam erat volutpat. "
      "Phasellus at sodales nunc. Fusce eget vestibulum diam, ac rhoncus eros. Aliquam est tellus, lobortis "
      "vitae justo sit amet, ultricies iaculis sem. Donec ac molestie odio. Suspendisse potenti. Aliquam "
      "elementum venenatis turpis, non vestibulum ex auctor at. Phasellus sed eros odio. Donec lacinia sem "
      "tortor, in ullamcorper eros bibendum eu. Donec blandit lacinia lobortis. Sed ac lorem malesuada, "
      "rhoncus velit eget, gravida eros. Morbi pellentesque lectus dapibus ullamcorper venenatis.";
    suffix_automaton lorem_ipsum(lorem_ipsum_text);
    fun_benchmark(100, [&lorem_ipsum] {
      assert(lorem_ipsum.positions(u"lorem").size() == 4);
    });
    fun_benchmark(100, [&lorem_ipsum_text] {
      std::u16string lorem_ipsum(lorem_ipsum_text);
      std::vector<size_t> positions;
      size_t pos = lorem_ipsum.find(u"lorem", 0);
      while(pos != std::string::npos) {
        positions.push_back(pos);
        pos = lorem_ipsum.find(u"lorem", pos + 1);
      }
      assert(positions.size() == 4);
    });
  }
}

void interactive() {
  std::u16string input = u"aabaabaabaab";
  std::u16string to_find = u"aa";
  suffix_automaton automaton(input);
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
  std::cout << "For string: " << converter.to_bytes(input) << std::endl;
  auto occurrences = automaton.positions(to_find);
  if (occurrences.empty()) {
    std::cout << "No occurrences was found for " << converter.to_bytes(to_find) << std::endl;
    return;
  }
  std::cout << converter.to_bytes(to_find) << " occurs at positions ";
  for (size_t i : occurrences) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
  for (int occurrence : occurrences) {
    std::cout << "# " << converter.to_bytes(input.substr(0, occurrence));
    std::cout << "(" << converter.to_bytes(input.substr(occurrence, to_find.length())) << ")";
    std::cout << converter.to_bytes(input.substr(occurrence + 1)) << std::endl;
  }
}

void run_suffix_automaton_tests() {
  test();
  interactive();
//  return 0;
//
//  std::string s;
//  char a;
//  std::cout << "Enter the string to construct a suffix automaton:" << endl;
//  std::cin.get(a);
//  while (a != '\n')
//  {
//    s.push_back(a);
//    cin.get(a);
//  }
//  cout << "Constructing automaton..." << endl;
//  suffix_automaton sa = suffix_automaton(s);
//  cout << "String: \"" << s << "\" is of size " << s.size() << " and its automaton has " << sa.states.size() << " states" << endl;
//  while (true)
//  {
//    cout << "Would you like to check for the [o]ccurrence of a substring, the [f]irst position of a substring, [a]ll positions of a substring, or [q]uit?" << endl;
//    unordered_set<char> input{'a', 'o', 'f', 'q'};
//    cin.get(a);
//    while (input.count(a) < 1)
//    {
//      cin.get(a);
//    }
//    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
//    if (a == 'a')
//    {
//      cout << "Enter a substring to see its positions:" << endl;
//
//      string p;
//      cin.get(a);
//      while (a != '\n')
//      {
//        p.push_back(a);
//        cin.get(a);
//      }
//      vector<int> positions = sa.positions(p);
//      if (positions.size() != 0)
//      {
//        cout << "YES, \"" << s << "\" contains the substring " << "\"" << p << "\" at positions\n[ ";
//        for (auto& i : positions)
//        {
//          cout << i << " ";
//        }
//        cout << "]" << endl;
//        for (auto& position : positions)
//        {
//          int id = min(10, position);
//          if (position > 10) cout << "...";
//          for (int pf = position-id; pf < position; pf++)
//          {
//            cout <<s[pf];
//          }
//          cout << "(" << p << ")";
//          for (id = p.size()+position; (id < s.size() && id < p.size()+position+10); id++)
//          {
//            cout << s[id];
//          }
//          if (id < s.size()) cout << "...";
//          cout << endl;
//        }
//      }
//      else
//      {
//        cout << "NO, \"" << s << "\" does not contain the substring " << "\"" << p << "\"" << endl;
//      }
//    }
//    else if (a == 'f')
//    {
//      cout << "Enter a substring to see its first position:" << endl;
//
//      string p;
//      cin.get(a);
//      while (a != '\n')
//      {
//        p.push_back(a);
//        cin.get(a);
//      }
//      int position = sa.first(p);
//      if (position != -1)
//      {
//        cout << "YES, \"" << s << "\" contains the substring " << "\"" << p << "\" at position " << position << ":" << endl;
//        int id = min(10, position);
//        if (position > 10) cout << "...";
//        for (int pf = position-id; pf < position; pf++)
//        {
//          cout <<s[pf];
//        }
//        cout << "(" << p << ")";
//        for (id = p.size()+position; (id < s.size() && id < p.size()+position+10); id++)
//        {
//          cout << s[id];
//        }
//        if (id < s.size()) cout << "...";
//        cout << endl;
//
//      } else
//      {
//        cout << "NO, \"" << s << "\" does not contain the substring " << "\"" << p << "\"" << endl;
//      }
//    }
//    else if (a == 'o')
//    {
//      cout << "Enter a substring to see if it occurs:" << endl;
//
//      string p;
//      cin.get(a);
//      while (a != '\n')
//      {
//        p.push_back(a);
//        cin.get(a);
//      }
//      bool occurs = sa.contains(p);
//      if (occurs)
//      {
//        cout << "YES, \"" << s << "\" contains the substring " << "\"" << p << "\"" << endl;
//      } else
//      {
//        cout << "No, \"" << s << "\" does not contain the substring " << "\"" << p << "\"" << endl;
//      }
//    }
//    else if (a == 'q')
//    {
//      cout << "Quitting" << endl;
//      return 0;
//    }
//  }
}









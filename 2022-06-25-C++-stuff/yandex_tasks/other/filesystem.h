#include <map>
#include <string>
#include <vector>

struct Node {
  std::map<std::string, Node> children;
};

class Tree {
private:
  Node root;

public:
  bool Has(const std::vector<std::string>& node) const;
  void Insert(const std::vector<std::string>& node);
  void Delete(const std::vector<std::string>& node);
};

inline bool Tree::Has(const std::vector<std::string>& node) const {

}

inline void Tree::Insert(const std::vector<std::string>& node) {

}

inline void Tree::Delete(const std::vector<std::string>& node) {

}

#ifndef RADIX_TREE_DEMO_H
#define RADIX_TREE_DEMO_H

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace radix_tree_demo {

class RadixTree {
 public:
  RadixTree();

  RadixTree(const RadixTree&) = delete;
  RadixTree& operator=(const RadixTree&) = delete;

  ~RadixTree();

  bool Insert(const std::string& key, int32_t value);

  bool Search(const std::string& key, int32_t* value) const;

  bool Delete(const std::string& key);

  std::vector<std::string> StartsWith(const std::string& prefix) const;

  void Print() const;

 private:
  struct Node {
    std::string label;
    int32_t value;
    bool has_value;
    std::vector<std::unique_ptr<Node>> children;

    Node() : value(0), has_value(false) {}
  };

  std::unique_ptr<Node> root_;

  Node* FindChildWithFirstChar(const Node* parent, char first) const;

  void CollectKeys(const Node* node, std::string* current,
                   std::vector<std::string>* keys) const;

  using PathEntry = std::pair<Node*, size_t>;

  void PrintNode(const Node* node, int depth,
                 std::ostringstream* oss) const;
};

int32_t RadixTreeDemoRun();

}  // namespace radix_tree_demo

#endif  // RADIX_TREE_DEMO_H

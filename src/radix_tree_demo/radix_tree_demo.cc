#include "radix_tree_demo.h"

#include <algorithm>
#include <sstream>
#include <string>

#include "common_util.h"

namespace radix_tree_demo {

RadixTree::RadixTree() : root_(std::make_unique<Node>()) {}

RadixTree::~RadixTree() = default;

RadixTree::Node* RadixTree::FindChildWithFirstChar(const Node* parent,
                                                    char first) const {
  for (const auto& child : parent->children) {
    if (!child->label.empty() && child->label[0] == first) {
      return child.get();
    }
  }
  return nullptr;
}

bool RadixTree::Insert(const std::string& key, int32_t value) {
  Node* current = root_.get();
  size_t offset = 0;

  while (offset < key.size()) {
    Node* child = FindChildWithFirstChar(current, key[offset]);
    if (child == nullptr) {
      auto new_node = std::make_unique<Node>();
      new_node->label = key.substr(offset);
      new_node->value = value;
      new_node->has_value = true;
      current->children.push_back(std::move(new_node));
      return true;
    }

    const std::string& label = child->label;
    size_t common = 0;
    while (common < label.size() && offset + common < key.size() &&
           label[common] == key[offset + common]) {
      common++;
    }

    if (common == label.size()) {
      offset += common;
      current = child;
      continue;
    }

    auto split = std::make_unique<Node>();
    split->label = label.substr(0, common);
    child->label = label.substr(common);

    if (offset + common < key.size()) {
      auto new_node = std::make_unique<Node>();
      new_node->label = key.substr(offset + common);
      new_node->value = value;
      new_node->has_value = true;
      split->children.push_back(std::move(new_node));
    } else {
      split->value = value;
      split->has_value = true;
    }

    auto it = std::find_if(current->children.begin(), current->children.end(),
                           [child](const auto& ptr) {
                             return ptr.get() == child;
                           });
    split->children.push_back(std::move(*it));
    *it = std::move(split);
    return true;
  }

  if (current->has_value) {
    return false;
  }
  current->value = value;
  current->has_value = true;
  return true;
}

bool RadixTree::Search(const std::string& key, int32_t* value) const {
  const Node* current = root_.get();
  size_t offset = 0;

  while (offset < key.size()) {
    const Node* child = FindChildWithFirstChar(current, key[offset]);
    if (child == nullptr) {
      return false;
    }

    const std::string& label = child->label;
    if (key.compare(offset, label.size(), label) != 0) {
      return false;
    }
    offset += label.size();
    current = child;
  }

  if (!current->has_value) {
    return false;
  }
  *value = current->value;
  return true;
}

bool RadixTree::Delete(const std::string& key) {
  if (key.empty()) {
    return false;
  }

  std::vector<PathEntry> path;
  Node* current = root_.get();
  size_t offset = 0;

  while (offset < key.size()) {
    Node* child = FindChildWithFirstChar(current, key[offset]);
    if (child == nullptr) {
      return false;
    }
    if (key.compare(offset, child->label.size(), child->label) != 0) {
      return false;
    }
    auto it = std::find_if(current->children.begin(), current->children.end(),
                           [child](const auto& ptr) {
                             return ptr.get() == child;
                           });
    size_t idx = static_cast<size_t>(
        std::distance(current->children.begin(), it));
    path.push_back({current, idx});
    offset += child->label.size();
    current = child;
  }

  if (!current->has_value) {
    return false;
  }
  current->has_value = false;

  for (int64_t i = static_cast<int64_t>(path.size()) - 1; i >= 0; i--) {
    auto idx = static_cast<size_t>(i);
    Node* parent = path[idx].first;
    size_t child_idx = path[idx].second;
    Node* node = parent->children[child_idx].get();

    if (node->has_value) {
      break;
    }
    if (node->children.empty()) {
      parent->children.erase(parent->children.begin() +
          static_cast<int64_t>(child_idx));
      continue;
    }
    if (node->children.size() == 1) {
      auto sole = std::move(node->children[0]);
      sole->label = node->label + sole->label;
      parent->children[child_idx] = std::move(sole);
      break;
    }
    break;
  }

  return true;
}

void RadixTree::CollectKeys(const Node* node, std::string* current,
                             std::vector<std::string>* keys) const {
  if (node->has_value) {
    keys->push_back(*current);
  }
  for (const auto& child : node->children) {
    size_t old_len = current->size();
    *current += child->label;
    CollectKeys(child.get(), current, keys);
    current->resize(old_len);
  }
}

std::vector<std::string> RadixTree::StartsWith(
    const std::string& prefix) const {
  std::vector<std::string> keys;
  const Node* current = root_.get();
  size_t offset = 0;

  while (offset < prefix.size()) {
    const Node* child = FindChildWithFirstChar(current, prefix[offset]);
    if (child == nullptr) {
      return keys;
    }
    const std::string& label = child->label;
    if (prefix.compare(offset, label.size(), label) != 0) {
      return keys;
    }
    offset += label.size();
    current = child;
  }

  std::string current_key = prefix;
  CollectKeys(current, &current_key, &keys);
  return keys;
}

void RadixTree::PrintNode(const Node* node, int depth,
                          std::ostringstream* oss) const {
  for (const auto& child : node->children) {
    for (int i = 0; i < depth; i++) {
      *oss << "  ";
    }
    *oss << "\"" << child->label << "\"";
    if (child->has_value) {
      *oss << " [" << child->value << "]";
    }
    *oss << "\n";
    PrintNode(child.get(), depth + 1, oss);
  }
}

void RadixTree::Print() const {
  std::ostringstream oss;
  oss << "RadixTree:\n";
  PrintNode(root_.get(), 0, &oss);
  LOG(INFO) << "\n" << oss.str();
}

int32_t RadixTreeDemoRun() {
  RadixTree tree;

  LOG(INFO) << "=== RadixTree Demo ===";

  tree.Insert("hello", 1);
  tree.Insert("hell", 2);
  tree.Insert("world", 3);
  tree.Insert("hi", 4);
  tree.Insert("her", 5);
  tree.Insert("hero", 6);
  tree.Insert("helium", 7);
  tree.Print();

  int32_t val = 0;
  if (tree.Search("hello", &val)) {
    LOG(INFO) << "Search hello: " << val;
  }
  if (tree.Search("hero", &val)) {
    LOG(INFO) << "Search hero: " << val;
  }
  if (!tree.Search("xyz", &val)) {
    LOG(INFO) << "Search xyz: not found";
  }

  auto keys = tree.StartsWith("he");
  LOG(INFO) << "StartsWith he:";
  for (const auto& k : keys) {
    LOG(INFO) << "  " << k;
  }

  tree.Delete("hero");
  LOG(INFO) << "After delete hero:";
  tree.Print();

  tree.Delete("hell");
  LOG(INFO) << "After delete hell:";
  tree.Print();

  LOG(INFO) << "=== Demo Done ===";
  return 0;
}

}  // namespace radix_tree_demo

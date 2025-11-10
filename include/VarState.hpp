#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class VarState {
 public:
  VarState();
  void setValue(const std::string& name, int value);
  int getValue(const std::string& name) const;
  void clear();
  std::vector<std::pair<std::string, int>> snapshot() const;
  void enterScope();
  void exitScope();
  std::size_t scopeDepth() const noexcept;
  void resetScopes();

 private:
  std::vector<std::unordered_map<std::string, int>> scopes_;
};

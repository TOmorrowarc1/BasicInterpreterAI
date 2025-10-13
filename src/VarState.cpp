#include "VarState.hpp"
#include "utils/Error.hpp"

#include <algorithm>

void VarState::setValue(const std::string &name, int value) {
  values_[name] = value;
}

int VarState::getValue(const std::string &name) const {
  auto it = values_.find(name);
  if (it == values_.end()) {
    throw BasicError("VARIABLE NOT DEFINED");
  }
  return it->second;
}

void VarState::clear() { values_.clear(); }

std::vector<std::pair<std::string, int>> VarState::snapshot() const {
  std::vector<std::pair<std::string, int>> items;
  items.reserve(values_.size());
  for (const auto &entry : values_) {
    items.emplace_back(entry.first, entry.second);
  }
  std::sort(items.begin(), items.end(),
            [](const std::pair<std::string, int> &lhs,
               const std::pair<std::string, int> &rhs) {
              return lhs.first < rhs.first;
            });
  return items;
}

#include "VarState.hpp"

#include <algorithm>

#include "utils/Error.hpp"

VarState::VarState() : scopes_(1) {}

void VarState::setValue(const std::string& name, int value) {
  scopes_.back()[name] = value;
}

int VarState::getValue(const std::string& name) const {
  for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
    auto found = it->find(name);
    if (found != it->end()) {
      return found->second;
    }
  }
  throw BasicError("VARIABLE NOT DEFINED");
}

void VarState::clear() {
  scopes_.clear();
  scopes_.emplace_back();
}

std::vector<std::pair<std::string, int>> VarState::snapshot() const {
  std::unordered_map<std::string, int> merged;
  for (const auto& scope : scopes_) {
    for (const auto& entry : scope) {
      merged[entry.first] = entry.second;
    }
  }

  std::vector<std::pair<std::string, int>> items;
  items.reserve(merged.size());
  for (const auto& entry : merged) {
    items.emplace_back(entry.first, entry.second);
  }

  std::sort(items.begin(), items.end(),
            [](const std::pair<std::string, int>& lhs,
               const std::pair<std::string, int>& rhs) {
              return lhs.first < rhs.first;
            });
  return items;
}

void VarState::enterScope() { scopes_.emplace_back(); }

void VarState::exitScope() {
  if (scopes_.size() == 1) {
    throw BasicError("SCOPE UNDERFLOW");
  }
  scopes_.pop_back();
}

std::size_t VarState::scopeDepth() const noexcept { return scopes_.size(); }

void VarState::resetScopes() {
  while (scopes_.size() > 1) {
    scopes_.pop_back();
  }
}

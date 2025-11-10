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

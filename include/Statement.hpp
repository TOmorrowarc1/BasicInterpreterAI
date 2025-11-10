#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState &state, Program &program) const = 0;

  const std::string &text() const noexcept;

private:
  std::string source_;
};

#include "Statement.hpp"

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string &Statement::text() const noexcept { return source_; }

RemStmt::RemStmt(std::string source, std::string comment)
    : Statement(std::move(source)), comment_(std::move(comment)) {}

void RemStmt::execute(VarState &state, Program &program) const {}

const std::string &RemStmt::comment() const noexcept { return comment_; }

LetStmt::LetStmt(std::string source, std::string name,
                 std::unique_ptr<Expression> &&expression)
    : Statement(std::move(source)), name_(std::move(name)),
      expression_(std::move(expression)) {}

void LetStmt::execute(VarState &state, Program &program) const {
  int value = expression_->evaluate(state);
  state.setValue(name_, value);
}

PrintStmt::PrintStmt(std::string source,
                     std::unique_ptr<Expression> &&expression)
    : Statement(std::move(source)), expression_(std::move(expression)) {}

void PrintStmt::execute(VarState &state, Program &program) const {
  int value = expression_->evaluate(state);
  std::cout << value << '\n';
}

InputStmt::InputStmt(std::string source, std::string name)
    : Statement(std::move(source)), name_(std::move(name)) {}

void InputStmt::execute(VarState &state, Program &program) const {
  std::cout << "? " << '\n';
  std::string line;
  if (!std::getline(std::cin, line)) {
    throw BasicError("INPUT ERROR");
  }
  std::istringstream stream(line);
  int value = 0;
  char extra = 0;
  if ((stream >> value) && !(stream >> extra)) {
    state.setValue(name_, value);
  } else {
    throw BasicError("INVAILD NUMBER");
  }
}

EndStmt::EndStmt(std::string source) : Statement(std::move(source)) {}

void EndStmt::execute(VarState &state, Program &program) const {
  program.requestStop();
}

GotoStmt::GotoStmt(std::string source, int targetLine)
    : Statement(std::move(source)), targetLine_(targetLine) {}

void GotoStmt::execute(VarState &state, Program &program) const {
  if (!program.hasLine(targetLine_)) {
    throw BasicError("LINE NUMBER ERROR");
  }
  program.changePC(targetLine_);
}

int GotoStmt::target() const noexcept { return targetLine_; }

IfStmt::IfStmt(std::string source, std::unique_ptr<Expression> &&left, char op,
               std::unique_ptr<Expression> &&right, int targetLine)
    : Statement(std::move(source)), left_(std::move(left)),
      right_(std::move(right)), op_(std::move(op)), targetLine_(targetLine) {}

void IfStmt::execute(VarState &state, Program &program) const {
  int lhs = left_->evaluate(state);
  int rhs = right_->evaluate(state);
  bool condition = false;
  switch (op_) {
  case '=':
    condition = lhs == rhs;
    break;
  case '<':
    condition = lhs < rhs;
    break;
  case '>':
    condition = lhs > rhs;
    break;
  default:
    throw BasicError("INVALID OPERATOR");
  }
  if (condition) {
    if (!program.hasLine(targetLine_)) {
      throw BasicError("LINE NUMBER ERROR");
    }
    program.changePC(targetLine_);
  }
}

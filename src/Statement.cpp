#include "Statement.hpp"

#include <iostream>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

RemStmt::RemStmt(std::string source, std::string comment)
    : Statement(std::move(source)), comment_(std::move(comment)) {}

void RemStmt::execute(VarState& state, Program& program) const {}

const std::string& RemStmt::comment() const noexcept { return comment_; }

LetStmt::LetStmt(std::string source, std::string name,
                 std::unique_ptr<Expression>&& expression)
    : Statement(std::move(source)),
      name_(std::move(name)),
      expression_(std::move(expression)) {}

void LetStmt::execute(VarState& state, Program& program) const {
  int value = expression_->evaluate(state);
  state.setValue(name_, value);
}

PrintStmt::PrintStmt(std::string source,
                     std::unique_ptr<Expression>&& expression)
    : Statement(std::move(source)), expression_(std::move(expression)) {}

void PrintStmt::execute(VarState& state, Program& program) const {
  int value = expression_->evaluate(state);
  std::cout << value << '\n';
}

InputStmt::InputStmt(std::string source, std::string name)
    : Statement(std::move(source)), name_(std::move(name)) {}

void InputStmt::execute(VarState& state, Program& program) const {
  std::cout << " ? ";
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream stream(line);
    int value = 0;
    char extra = 0;
    try {
      if ((stream >> value) && !(stream >> extra)) {
        state.setValue(name_, value);
        break;
      } else {
        throw BasicError("INVALID NUMBER");
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << '\n';
      std::cout << " ? ";
      continue;
    }
  }
}

EndStmt::EndStmt(std::string source) : Statement(std::move(source)) {}

void EndStmt::execute(VarState& state, Program& program) const {
  program.programEnd();
}

IndentStmt::IndentStmt(std::string source) : Statement(std::move(source)) {}

void IndentStmt::execute(VarState& state, Program&) const {
  state.enterScope();
}

DedentStmt::DedentStmt(std::string source) : Statement(std::move(source)) {}

void DedentStmt::execute(VarState& state, Program&) const { state.exitScope(); }

GotoStmt::GotoStmt(std::string source, int targetLine)
    : Statement(std::move(source)), targetLine_(targetLine) {}

void GotoStmt::execute(VarState& state, Program& program) const {
  program.changePC(targetLine_);
}

int GotoStmt::target() const noexcept { return targetLine_; }

IfStmt::IfStmt(std::string source, std::unique_ptr<Expression>&& left, char op,
               std::unique_ptr<Expression>&& right, int targetLine)
    : Statement(std::move(source)),
      left_(std::move(left)),
      right_(std::move(right)),
      op_(std::move(op)),
      targetLine_(targetLine) {}

void IfStmt::execute(VarState& state, Program& program) const {
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
    program.changePC(targetLine_);
  }
}

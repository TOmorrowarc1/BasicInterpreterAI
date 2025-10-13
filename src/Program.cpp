#include "Program.hpp"
#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program()
    : recorder_(), vars_(), programCounter_(0), programEnd_(false) {}

void Program::addStmt(int line, std::unique_ptr<Statement> &&stmt) {
  recorder_.add(line, std::move(stmt));
}

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run() {
  int programCounter_ = recorder_.nextLine(-1);
  if (programCounter_ == -1) {
    return;
  }
  while (!programEnd_ && programCounter_ != -1) {
    const Statement *stmt = recorder_.get(programCounter_);
    programCounter_ = recorder_.nextLine(programCounter_);
    if (stmt == nullptr) {
      throw BasicError("LINE NUMBER ERROR");
    }
    stmt->execute(vars_, *this);
  }
  // resetAfterRun();
}

void Program::list() const { recorder_.printLines(); }

void Program::clear() {
  recorder_.clear();
  vars_.clear();
  programCounter_ = 0;
}

void Program::execute(std::unique_ptr<Statement> &&stmt) {
  if (!stmt) {
    throw BasicError("STATEMENT NULL");
  }
  stmt->execute(vars_, *this);
}

int Program::getPC() const noexcept { return programCounter_; }

void Program::changePC(int line) {
  if (line <= 0) {
    throw BasicError("LINE NUMBER ERROR");
  }
  if (!recorder_.hasLine(line)) {
    throw BasicError("LINE NUMBER ERROR");
  }
  programCounter_ = line;
}

void Program::programEnd() { programEnd_ = true; }

void Program::resetAfterRun() noexcept {
  vars_.clear();
  programCounter_ = 0;
  programEnd_ = false;
}

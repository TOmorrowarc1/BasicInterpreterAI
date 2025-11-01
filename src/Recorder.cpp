#include "Recorder.hpp"
#include "utils/Error.hpp"

#include <algorithm>
#include <iostream>

Recorder::~Recorder() {
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    delete it->second;
  }
}

void Recorder::add(int line, Statement *stmt) {
  if (line <= 0) {
    throw BasicError("LINE NUMBER ERROR");
  }
  if (!stmt) {
    throw BasicError("STATEMENT NULL");
  }
  auto it = lines.find(line);
  if (it != lines.end()) {
    delete it->second;
  }
  lines[line] = stmt;
}

void Recorder::remove(int line) {
  if (line <= 0) {
    throw BasicError("LINE NUMBER ERROR");
  }
  auto it = lines.find(line);
  if (it != lines.end()) {
    delete it->second;
    lines.erase(it);
  }
}

const Statement *Recorder::get(int line) const noexcept {
  auto it = lines.find(line);
  if (it == lines.end()) {
    return nullptr;
  }
  return it->second;
}

bool Recorder::hasLine(int line) const noexcept {
  return lines.find(line) != lines.end();
}

void Recorder::clear() noexcept {
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    delete it->second;
  }
  lines.clear();
}

void Recorder::printLines() const {
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    std::cout << it->second->text() << '\n';
  }
}

int Recorder::nextLine(int line) const noexcept {
  auto it = lines.upper_bound(line);
  if (it == lines.end()) {
    return -1;
  }
  return it->first;
}

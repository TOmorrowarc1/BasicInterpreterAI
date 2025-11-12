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

// TODO: Imply interfaces declared in the Statement.hpp.

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class VarState {
public:
	void setValue(const std::string &name, int value);
	int getValue(const std::string &name) const;
	void clear();
	std::vector<std::pair<std::string, int>> snapshot() const;

private:
	std::unordered_map<std::string, int> values_;
};


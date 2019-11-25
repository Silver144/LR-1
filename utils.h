#pragma once
#include "analyzer.h"

namespace utils
{
	inline std::map<char, std::set<char>> first_set;
	inline std::vector<state> dfa;

	void init();
	void _first_table();
	void _create_dfa();
	void _create_table();
}

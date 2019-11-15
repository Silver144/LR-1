#pragma once
#include <set>
#include <map>
#include "utils.h"

enum class type
{
	_SHIFT,
	_REDUCE,
	_ACCEPT,
	_GOTO,
	_ERROR
};

class action
{
public:
	action() = default;
	action(type __type, int next) : _type(__type), _next(next) {}

	type _type;
	int _next;
};

class status_index
{
public:
	status_index() = default;
	status_index(int status, char symbol) : _status(status), _symbol(symbol) {}

	int _status;
	char _symbol;
};

template <typename T>
constexpr bool _t_less(const T& _Tx, const T& _Ty)
{
	return _Tx < _Ty;
}

template <> constexpr bool _t_less<status_index>(const status_index& _Tx, const status_index& _Ty)
{
	return _Tx._status < _Ty._status ? true : _Tx._status == _Ty._status ?
		_Tx._symbol < _Ty._symbol ? true : false : false;
}
using lis = std::integral_constant<decltype(&_t_less<status_index>), &_t_less<status_index>>;

extern std::stack<int> _status;
extern std::stack<char> _symbol;

extern std::set<char> _terminator;
extern std::set<char> _non_terminator;

extern std::vector<std::pair<char, std::string>> generator;

extern std::map<status_index, action, lis> _state_table;

action _get_action(int _status, char _symbol);
void analyze(const std::string& _sentense);

void _init_table();

#pragma once
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <assert.h>


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

inline std::stack<int> _status;
inline std::stack<char> _symbol;

inline std::set<char> _terminator;
inline std::set<char> _non_terminator;

inline std::vector<std::pair<char, std::string>> generator;

inline std::map<status_index, action, lis> _state_table;

class state_val
{
public:
	state_val() = default;
	state_val(int generator_id, int ip, std::set<char> ahead) : _generator_id(generator_id), _ip(ip), _ahead(ahead)
	{
		gene = generator[_generator_id];
	}

	int _generator_id;
	int _ip;
	std::set<char> _ahead;
	std::pair<char, std::string> gene;

	bool operator==(const state_val& b) const
	{
		return _generator_id == b._generator_id &&
			_ip == b._ip &&
			_ahead == b._ahead;
	}

	bool operator<(const state_val& b) const
	{
		if (_generator_id < b._generator_id)
			return true;
		if (_generator_id > b._generator_id)
			return false;
		if (_ip < b._ip)
			return true;
		if (_ip > b._ip)
			return false;
		if (_ahead < b._ahead)
			return true;
		return false;
	}
};

class state
{
public:
	std::set<state_val> _val;
	std::map<char, int> _direct;
	std::set<state_val> _kernel;
};

action _get_action(int _status, char _symbol);
void analyze(const std::string& _sentense);
void _init_generator();

using msal = std::map<status_index, action, lis>::value_type;

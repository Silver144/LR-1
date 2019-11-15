#include "analyzer.h"

std::stack<int> _status;
std::stack<char> _symbol;

std::set<char> _terminator;
std::set<char> _non_terminator;

std::vector<std::pair<char, std::string>> generator;

std::map<status_index, action, lis> _state_table;

action _get_action(int _status, char _symbol)
{
	auto _T_index = status_index(_status, _symbol);
	return _state_table[_T_index];
}

void analyze(const std::string& _sentense)
{
	_status.push(0);
	int _ip = 0;

	auto _action = _get_action(_status.top(), _sentense[_ip]);
	while (_ip != _sentense.size() && _action._type != type::_ACCEPT && _action._type != type::_ERROR)
	{
		if (_action._type == type::_SHIFT)
		{
			_symbol.push(_sentense[_ip]);
			_status.push(_action._next);
			_action = _get_action(_status.top(), _sentense[++_ip]);
			continue;
		}
		else if (_action._type == type::_REDUCE)
		{
			int _generate_len = static_cast<int>(generator[_action._next].second.size());
			while (_generate_len--)
				_status.pop(), _symbol.pop();
			_symbol.push(generator[_action._next].first);
			_action = _get_action(_status.top(), _symbol.top());
		}
		else
		{
			_status.push(_action._next);
			_action = _get_action(_status.top(), _sentense[_ip]);
		}
	}
}

using msal = std::map<status_index, action, lis>::value_type;

void _init_table()
{
	_state_table.insert(msal(status_index(0, '('), action(type::_SHIFT, 4)));
	_state_table.insert(msal(status_index(0, 'n'), action(type::_SHIFT, 5)));
	_state_table.insert(msal(status_index(0, 'E'), action(type::_GOTO, 1)));
	_state_table.insert(msal(status_index(0, 'T'), action(type::_GOTO, 2)));
	_state_table.insert(msal(status_index(0, 'F'), action(type::_GOTO, 3)));

	_state_table.insert(msal(status_index(1, '+'), action(type::_SHIFT, 6)));
	_state_table.insert(msal(status_index(1, '-'), action(type::_SHIFT, 7)));
	_state_table.insert(msal(status_index(1, '$'), action(type::_ACCEPT, 0)));

	_state_table.insert(msal(status_index(2, '*'), action(type::_SHIFT, 8)));
	_state_table.insert(msal(status_index(2, '/'), action(type::_SHIFT, 9)));
	_state_table.insert(msal(status_index(2, '+'), action(type::_REDUCE, 3)));
	_state_table.insert(msal(status_index(2, '-'), action(type::_REDUCE, 3)));
	_state_table.insert(msal(status_index(2, '$'), action(type::_REDUCE, 3)));

	_state_table.insert(msal(status_index(3, '+'), action(type::_REDUCE, 6)));
	_state_table.insert(msal(status_index(3, '-'), action(type::_REDUCE, 6)));
	_state_table.insert(msal(status_index(3, '*'), action(type::_REDUCE, 6)));
	_state_table.insert(msal(status_index(3, '/'), action(type::_REDUCE, 6)));
	_state_table.insert(msal(status_index(3, '$'), action(type::_REDUCE, 6)));

	_state_table.insert(msal(status_index(4, '('), action(type::_SHIFT, 13)));
	_state_table.insert(msal(status_index(4, 'n'), action(type::_SHIFT, 14)));
	_state_table.insert(msal(status_index(4, 'E'), action(type::_GOTO, 10)));
	_state_table.insert(msal(status_index(4, 'T'), action(type::_GOTO, 11)));
	_state_table.insert(msal(status_index(4, 'F'), action(type::_GOTO, 12)));

	_state_table.insert(msal(status_index(5, '+'), action(type::_REDUCE, 8)));
	_state_table.insert(msal(status_index(5, '-'), action(type::_REDUCE, 8)));
	_state_table.insert(msal(status_index(5, '*'), action(type::_REDUCE, 8)));
	_state_table.insert(msal(status_index(5, '/'), action(type::_REDUCE, 8)));
	_state_table.insert(msal(status_index(5, '$'), action(type::_REDUCE, 8)));

	_state_table.insert(msal(status_index(6, '('), action(type::_SHIFT, 4)));
	_state_table.insert(msal(status_index(6, 'n'), action(type::_SHIFT, 5)));
	_state_table.insert(msal(status_index(6, 'T'), action(type::_GOTO, 15)));
	_state_table.insert(msal(status_index(6, 'F'), action(type::_GOTO, 3)));

	_state_table.insert(msal(status_index(7, '('), action(type::_SHIFT, 4)));
	_state_table.insert(msal(status_index(7, 'n'), action(type::_SHIFT, 5)));
	_state_table.insert(msal(status_index(7, 'T'), action(type::_GOTO, 16)));
	_state_table.insert(msal(status_index(7, 'F'), action(type::_GOTO, 3)));
}

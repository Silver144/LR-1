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
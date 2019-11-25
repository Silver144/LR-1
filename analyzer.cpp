#include "analyzer.h"

action _get_action(int _status, char _symbol)
{
	auto _T_index = status_index(_status, _symbol);

	if (_state_table.find(_T_index) != _state_table.end())
		return _state_table[_T_index];
	else
		return action(type::_ERROR, 0);
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
	return;
}

void _init_generator()
{
	generator.push_back(std::make_pair('A', "E"));
	generator.push_back(std::make_pair('E', "E+T"));
	generator.push_back(std::make_pair('E', "E-T"));
	generator.push_back(std::make_pair('E', "T"));
	generator.push_back(std::make_pair('T', "T*F"));
	generator.push_back(std::make_pair('T', "T/F"));
	generator.push_back(std::make_pair('T', "F"));
	generator.push_back(std::make_pair('F', "(E)"));
	generator.push_back(std::make_pair('F', "n"));

	/*generator.push_back(std::make_pair('Q', "S"));
	generator.push_back(std::make_pair('S', "CC"));
	generator.push_back(std::make_pair('C', "cC"));
	generator.push_back(std::make_pair('C', "d"));*/

}

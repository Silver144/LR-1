#include "utils.h"

namespace utils
{

	void init()
	{
		_init_generator();
		_first_table();
		_create_dfa();
		_create_table();
		return;
	}

	template <typename T, typename less>
	std::set<T, less> set_union(const std::set<T, less>& a, const std::set<T, less>& b)
	{
		std::set<T, less> result = a;
		result.insert(b.begin(), b.end());
		return result;
	}

	std::set<char> _get_first(std::string sym)
	{
		if (sym.size() == 0)
			return { '$' };

		auto _res = sym[0];

		if (_terminator.find(_res) != _terminator.end())
			return { _res };
		else if (_non_terminator.find(_res) != _non_terminator.end())
		{
			if (first_set[_res].find('$') == first_set[_res].end())
				return first_set[_res];
			else
			{
				auto _T_set = first_set[_res];
				_T_set.erase('$');
				return set_union(_T_set, _get_first(sym.substr(1)));
			}
		}
		else
			return {};
	}

	bool _has_changed(std::map<char, std::set<char>> _first_set)
	{
		auto _it = _first_set.begin();
		auto it = first_set.begin();

		while (it != first_set.end() && _it != _first_set.end())
		{
			if (it->first == _it->first && it->second == _it->second)
			{
				it++, _it++;
				continue;
			}
			else
			{
				first_set = _first_set;
				return true;
			}
		}
		if (it == first_set.end() && _it == _first_set.end())
			return false;
		else
			return true;
	}

	void _first_table()
	{
		std::map<char, std::set<char>> _first_set;

		for (auto x : generator)
			_first_set[x.first] = {}, first_set[x.first] = {};
		do
		{
			for (auto x : generator)
			{
				if (x.first == 'A')
					int a = 0;
				auto new_set = _get_first(x.second);
				_first_set[x.first] = set_union(new_set, _first_set[x.first]);
			}
		}
		while (_has_changed(_first_set));
	}

	int has_kernel(std::set<state_val> _val)
	{
		for (auto it = 0; it != dfa.size(); it++)
		{
			if (dfa[it]._kernel == _val)
				return it;
		}
		return -1;
	}

	void _create_table()
	{
		for (auto it  = 0; it < dfa.size(); it++)
		{
			for (auto val : dfa[it]._direct)
			{
				if (_terminator.find(val.first) != _terminator.end())
					_state_table.insert(msal(status_index(it, val.first), action(type::_SHIFT, val.second)));
				if (_non_terminator.find(val.first) != _non_terminator.end())
					_state_table.insert(msal(status_index(it, val.first), action(type::_GOTO, val.second)));
				else
					assert(true);
			}
			for (auto val : dfa[it]._val)
			{
				auto gene = generator[val._generator_id].second;
				if (val._ip == gene.size())
				{
					for (auto _ahead : val._ahead)
					{
						if (val._generator_id != 0)
							_state_table.insert(msal(status_index(it, _ahead), action(type::_REDUCE, val._generator_id)));
						else
							_state_table.insert(msal(status_index(it, '$'), action(type::_ACCEPT, 0)));
					}
				}
			}
		}
	}

	void _create_dfa()
	{
		int dfa_num = 0;

		state init_state;
		std::set<char> __val = { '$' };
		init_state._val.insert(state_val(0, 0, __val));
		init_state._kernel.insert(state_val(0, 0, __val));
		dfa.push_back(init_state);
		auto _it = 0;

		while (_it != dfa.size())
		{
			auto old = dfa[_it]._val;

			do
			{
				old = dfa[_it]._val;
				std::set<state_val> should_ins;

				for (auto _val : dfa[_it]._val)
				{
					auto _g = generator[_val._generator_id];
					if (_non_terminator.find(_g.second[_val._ip]) != _non_terminator.end())
					{

						for (auto __it = 0; __it != generator.size(); __it++)
						{
							if (_g.second[_val._ip] == generator[__it].first)
							{
								if (__it == 7)
									int a = 1;
								auto look_ahead = _get_first(generator[_val._generator_id].second.substr(_val._ip + 1));
								if (look_ahead.find('$') != look_ahead.end())
								{
									look_ahead.erase('$');
									look_ahead = set_union(look_ahead, _val._ahead);
								}
								should_ins.insert(state_val(__it, 0, look_ahead));
							}
						}
					}
				}

				dfa[_it]._val = set_union(should_ins, dfa[_it]._val);

				class point
				{
				public:
					point(int _x, int _y) : x(_x), y(_y) {}

					int x;
					int y;

					bool operator<(const point& a) const
					{
						if (x < a.x)
							return true;
						if (x > a.x)
							return false;
						if (y < a.y)
							return true;

						return false;
					}

				};

				std::map<point, std::set<char>> combine_ahead;

				std::set<state_val> combine_state;

				for (auto _val : dfa[_it]._val)
				{
					if (combine_ahead.find(point(_val._generator_id, _val._ip)) == combine_ahead.end())
						combine_ahead[point(_val._generator_id, _val._ip)] = _val._ahead;
					else
						combine_ahead[point(_val._generator_id, _val._ip)] = set_union(combine_ahead[point(_val._generator_id, _val._ip)], _val._ahead);
				}

				for (auto _val : dfa[_it]._val)
					combine_state.insert(state_val(_val._generator_id, _val._ip, combine_ahead[point(_val._generator_id, _val._ip)]));

				dfa[_it]._val.clear();
				dfa[_it]._val = std::move(combine_state);
			} while (old != dfa[_it]._val);

			std::map<char, std::set<state_val>> char_to_state;
			for (auto _val : dfa[_it]._val)
			{
				int reach_char = generator[_val._generator_id].second[_val._ip];
				if (generator[_val._generator_id].second.size() == _val._ip)
					continue;

				state_val _t_val(_val._generator_id, _val._ip + 1, _val._ahead);

				if (char_to_state.find(reach_char) == char_to_state.end())
					char_to_state[reach_char] = { _t_val };
				else
					char_to_state[reach_char].insert(_t_val);
			}

			for (auto _val : char_to_state)
			{
				int _des_dfa_id = has_kernel(_val.second);

				if (_des_dfa_id == -1)
				{
					state new_state;
					new_state._val = _val.second;
					new_state._kernel = _val.second;
					dfa.push_back(new_state);
					dfa[_it]._direct[_val.first] = ++dfa_num;
				}
				else
					dfa[_it]._direct[_val.first] = _des_dfa_id;
			}
			++_it;
		}
	}
}

#include "utils.h"

int main()
{
	utils::init();

	std::string _input("(n+n)$");
	analyze(_input);
	return 0;
}

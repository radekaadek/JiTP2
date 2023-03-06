#include <iostream>
#include "range.h"
#include "bcrange.h"
#include "lcrange.h"
#include "rcrange.h"

int main()
{
	lcrange l{ 1, 10 };
	std::cout << l << std::endl;
	std::cout << l.inrange(1) << std::endl;
	std::cout << l.inrange(10) << std::endl;
	std::cout << l.inrange(0) << std::endl;
	std::cout << l.inrange(11) << std::endl;
	std::cout << l(5) << std::endl;
	bcrange b{ 1, 10 };
	std::cout << b << std::endl;
	std::cout << b.inrange(1) << std::endl;
	std::cout << b.inrange(10) << std::endl;
	std::cout << b.inrange(0) << std::endl;
	std::cout << b.inrange(11) << std::endl;
	std::cout << b(5) << std::endl;
	rcrange r{ 1, 10 };
	std::cout << r << std::endl;
	std::cout << r.inrange(1) << std::endl;
	std::cout << r.inrange(10) << std::endl;
	std::cout << r.inrange(0) << std::endl;
	std::cout << r.inrange(11) << std::endl;
	std::cout << r(5) << std::endl;

}

#pragma once
#include "range.h"

class rcrange : public range {
public:
	rcrange(int low, int hi) : range{ low, hi } {}
	bool inrange(int arg) {
		return low() < arg && arg <= high();
	}
	char left_paren() const { return '<'; }
	char right_paren() const { return '>'; }
};

std::ostream& operator<<(std::ostream& os, const rcrange& rng)
{
	return os << rng.left_paren() << rng.low() << ", "
		<< rng.high() << rng.right_paren();
}


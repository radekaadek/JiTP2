#pragma once
#include "range.h"

class rcrange : public range {
public:
	rcrange(int low, int hi) : range{ low, hi } {}
	bool inrange(int arg) {
		return low() < arg && arg <= high();
	}
	char left_paren() const { return '('; }
	char right_paren() const { return '>'; }
};

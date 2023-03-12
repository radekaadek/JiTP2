#pragma once
#include "range.h"

class bcrange : public range {
public:
	bcrange(int low, int hi) : range{ low, hi } {}
	bool inrange(int arg) {
		return low() <= arg && arg <= high();
	}
	char left_paren() const { return '<'; }
	char right_paren() const { return '>'; }
};

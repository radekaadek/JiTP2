#pragma once
#include "range.h"

class lcrange : public range {
public:
	lcrange(int low, int hi) : range{ low, hi } {}
	virtual bool inrange(int arg) {
		return low() <= arg && arg < high();
	}
	char left_paren() const { return '<'; }
	char right_paren() const { return ')'; }
};

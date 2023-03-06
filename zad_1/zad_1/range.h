#pragma once
#include <iostream>

class range {
	int low_, hi_;
public:
	range(int low, int hi) : low_{low}, hi_{hi} {}
	int low() const { return low_; }
	int high() const { return hi_; }
	bool inrange(int x) const { return x >= low_ && x <= hi_; }
	bool operator()(int x) const { return inrange(x); }
	char left_paren() const { return '<'; }
	char right_paren() const { return '>'; }

};

std::ostream& operator<<(std::ostream& os, const range& r) {
	return os << r.left_paren() << r.low() << ',' << r.high() << r.right_paren();
}

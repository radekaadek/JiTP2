#pragma once
#include <iostream>

class range {
	int low_, hi_;
public:
	range(int low, int hi) : low_{ low }, hi_{ hi } {}
	virtual ~range() = default;
	int low() const { return low_; }
	int high() const { return hi_; }
	virtual bool inrange(int x) const { return x > low_ && x < hi_; }
	bool operator()(int x) const { return inrange(x); }
	virtual char left_paren() const { return '('; }
	virtual char right_paren() const { return ')'; }

};

std::ostream& operator<<(std::ostream& os, const range& r) {
	return os << r.left_paren() << r.low() << ',' << r.high() << r.right_paren();
}


class lcrange : public range {
public:
	using range::range;
	bool inrange(int arg) const {
		return low() <= arg && arg < high();
	}
	char left_paren() const { return '<'; }
	char right_paren() const { return ')'; }
};


class bcrange : public range {
public:
	using range::range;
	bool inrange(int arg) const {
		return low() <= arg && arg <= high();
	}
	char left_paren() const { return '<'; }
	char right_paren() const { return '>'; }
};


class rcrange : public range {
public:
	using range::range;
	bool inrange(int arg) const {
		return low() < arg && arg <= high();
	}
	char left_paren() const { return '('; }
	char right_paren() const { return '>'; }
};

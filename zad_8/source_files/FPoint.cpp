#include "FPoint.h"

std::ostream& operator <<(std::ostream& os, const FPoint& p)
{
 os << "(" << p.x << ", " << p.y << ")";
 return os;
}

std::istream& operator >>(std::istream& is, FPoint& p)
{
 is >> p.x >> p.y;
 return is;
}

FPoint::operator Graph_lib::Point() const
{
 return Graph_lib::Point(x, y);
}

FPoint min(const FPoint& lf, const FPoint& rt)
{
 return FPoint(min(lf.x, rt.x), min(lf.y, rt.y));
}

FPoint max(const FPoint& lf, const FPoint& rt)
{
 return FPoint(max(lf.x, rt.x), max(lf.y, rt.y));
}

FPoint operator+(const FPoint& lpoint, float val)
{
 return FPoint(lpoint.x + val, lpoint.y + val);
}

FPoint operator-(const FPoint& lpoint, float val)
{
 return FPoint(lpoint.x - val, lpoint.y - val);
}

FPoint operator+(const FPoint& lpoint, const FPoint& rpoint)
{
 return FPoint(lpoint.x + rpoint.x, lpoint.y + rpoint.y);
}

FPoint operator*(const FPoint& lpoint, const FPoint& rpoint)
{
 return FPoint(lpoint.x * rpoint.x, lpoint.y * rpoint.y);
}

float distance(const FPoint& p1, const FPoint& p2)
{
 return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}


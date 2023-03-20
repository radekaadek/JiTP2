#include "figure.h"


std::ostream& operator<<(std::ostream& os, const FPoint& p)
{
    return os << '(' << p.x << ',' << p.y << ')';
}

std::istream& operator>>(std::istream& is, FPoint& p)
{
    char l, m, r;
    is >> l >> p.x >> m >> p.y >> r;
    if (l != '(' || m != ',' || r != ')')
        throw std::runtime_error("Invalid point.");
    return is;
}

std::vector<FPoint> get_points(std::istream& is)
{
    char ch;
    is >> ch;
    if (ch != '(')
        throw std::runtime_error("Invalid beginning of the points' list.");
    std::vector<FPoint> vp; // zbierane punkty
    do {
        // odczyt punktu i do�o�enie go do vp
        FPoint pt;
        is >> pt;
        vp.push_back(pt);
        is >> ch; // sprawdzamy, co czai si� w strumieniu
        if (ch == '(')
            is.unget(); // odk�adamy '(' �eby punkt by� ca�y
    } while (ch != ')');
    return vp;
}

std::string get_id(std::istream& is)
{
    std::string retv;
    char ch;
    while (is >> ch)
    {
        if (isalpha(ch)) // litery dodajemy do identyfikatora
            retv += ch;
        else if (retv.empty()) // pusty identyfikator
            throw std::runtime_error("Indentifier not found.");
        else
        {
            is.unget(); // nie wiemy co, ale odk�adamy
            return retv;
        }
    }
    return retv;
}

std::istream& operator>>(std::istream& is, figure& f)
{
    f.id = get_id(is);
    if (f.id.length() == 0)
        return is;
    f.fdef = get_points(is);
    return is;
}

std::ostream& operator<<(std::ostream& os, const figure& f)
{
    os << f.id << '(';
    for (int i = 0; i < (int)f.fdef.size(); ++i)
        os << f.fdef[i];
    return os << ')';
}

figure* get_figure(std::istream& is) {
    std::string id = get_id(is);
    if (id.length() == 0)
        return nullptr;
    std::vector<FPoint> pts = get_points(is);
    if (id == Rect::class_id())
        return new Rect(pts);
    if (id == Circ::class_id())
        return new Circ(pts);
    throw std::runtime_error("Unknown figure id.");
}

float distance(const FPoint& p1, const FPoint& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

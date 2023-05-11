#include "figure.h"

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
    if (id == Line::class_id())
        return new Line(pts);
    throw std::runtime_error("Unknown figure id.");
}

pair<FPoint, FPoint> map_bbox(const vector<figure*>& vf) {
    FPoint min = vf[0]->bbox().first;
    FPoint max = vf[0]->bbox().second;
    for (size_t i = 1; i < vf.size(); ++i) {
        auto bb = vf[i]->bbox();
        min.x = std::min(min.x, bb.first.x);
        min.y = std::min(min.y, bb.first.y);
        max.x = std::max(max.x, bb.second.x);
        max.y = std::max(max.y, bb.second.y);
    }
    return { min, max };
}

Graph_lib::Shape* Line::get_shape(const FPoint& scale, const FPoint& translation) const {
    Graph_lib::Lines* l = new Graph_lib::Lines();
    for (size_t i = 1; i < fdef.size(); ++i)
        l->add(FPoint(fdef[i-1].x * scale.x + translation.x,
                    fdef[i-1].y * scale.y + translation.y),
            FPoint(fdef[i].x * scale.x + translation.x,
                fdef[i].y * scale.y + translation.y));
    return l;
}

std::ostream& operator<<(std::ostream& os, const std::pair<FPoint, FPoint>& db1) {
    return os << '[' << db1.first << ',' << db1.second << ']';
}

std::pair<FPoint, FPoint> figure::bbox() const {
    FPoint mi = fdef[0];
    FPoint ma = fdef[0];
    for (int i = 1; i < (int)fdef.size(); ++i) {
        mi = min(mi, fdef[i]);
        ma = max(ma, fdef[i]);
    }
    return { mi, ma };
}

Graph_lib::Shape* Rect::get_shape(
            const FPoint& scale,
            const FPoint& translation) const {
    FPoint ul = FPoint(fdef[0].x * scale.x + translation.x,
                        fdef[0].y * scale.y + translation.y);
    FPoint lr = FPoint(fdef[1].x * scale.x + translation.x,
                        fdef[1].y * scale.y + translation.y);
    FPoint ulrect = FPoint(min(ul.x, lr.x), min(ul.y, lr.y));
    FPoint lrrect = FPoint(max(ul.x, lr.x), max(ul.y, lr.y));
    return new Graph_lib::Rectangle(ulrect, lrrect);
}

// Graph_lib::Shape* Rect::get_shape()

Graph_lib::Shape* Circ::get_shape(
            const FPoint& scale,
            const FPoint& translation) const {
    return new Graph_lib::Circle(
        FPoint(fdef[0].x * scale.x + translation.x,
            fdef[0].y * scale.y + translation.y),
        distance(fdef[0], fdef[1]) * scale.x
    );
}


std::pair<FPoint, FPoint> get_transformation(
        const std::pair<FPoint, FPoint>& obj_bbox,
        const std::pair<FPoint, FPoint>& disp_bbox) {
    FPoint scale = FPoint((disp_bbox.second.x - disp_bbox.first.x) / (obj_bbox.second.x - obj_bbox.first.x),
        (disp_bbox.second.y - disp_bbox.first.y) / (obj_bbox.second.y - obj_bbox.first.y));
    scale.x = scale.y = std::min(scale.x, scale.y);
    FPoint translation = FPoint((disp_bbox.first.x + disp_bbox.second.x) / 2 - (obj_bbox.first.x + obj_bbox.second.x) / 2 * scale.x,
        (disp_bbox.first.y + disp_bbox.second.y) / 2 - (obj_bbox.first.y + obj_bbox.second.y) / 2 * scale.y);
    return { scale, translation };
}

std::string window_title() {
    return "Radoslaw Dabkowski 325683";
}

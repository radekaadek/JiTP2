#pragma once
#include "Graph.h"
#include "fpoint.h"
#include "matrix.h"

float distance(const FPoint& p1, const FPoint& p2);

std::ostream& operator<<(std::ostream& os, const FPoint& p);
std::istream& operator>>(std::istream& is, FPoint& p);
std::ostream& operator<<(std::ostream& os, const std::pair<FPoint, FPoint>& db1);

std::vector<FPoint> get_points(std::istream& is);
std::string get_id(std::istream& is);

class figure {
protected:
    std::string id;
    std::vector<FPoint> fdef;
public:
    figure(const std::vector<FPoint>& fv = {}) : fdef(fv) {}
    virtual ~figure() {}
    virtual std::pair<FPoint, FPoint> bbox() const;
    static std::string class_id() { return "Unknown"; }
    virtual Graph_lib::Shape* get_shape( 
        const std::vector<FPoint>& pts) const = 0;
    virtual std::string get_id() const = 0;
    virtual Graph_lib::Shape* transform(float angle = 0, FPoint center = {0, 0}, FPoint scale = {1, 1}, FPoint translation = {0, 0}) const {
        // Step 1: translate to origin
        Matrix<long double> m = Matrix<long double>::translateMx(-center.x, -center.y);
        // Step 2: rotate
        m = Matrix<long double>::rotateMx(angle) * m;
        // Step 3: scale
        m = Matrix<long double>::scaleMx(scale.x, scale.y) * m;
        // Step 4: translate back
        m = Matrix<long double>::translateMx(center.x, center.y) * m;
        // Step 5: apply transformation
        m = Matrix<long double>::translateMx(translation.x, translation.y) * m;
        std::vector<FPoint> new_fdef(fdef.size());
        std::transform(fdef.begin(), fdef.end(), new_fdef.begin(), [&m](const FPoint& p) { return m.transform(p); });
        return get_shape(new_fdef);
    }
    friend std::istream& operator>>(std::istream& is, figure& f);
    friend std::ostream& operator<<(std::ostream& os, const figure& f);
};


class Rect : public figure {
public:
    Rect(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Rect: ... points.");
        FPoint tr = { fv[1].x, fv[0].y };
        FPoint bl = { fv[0].x, fv[1].y };
        fdef = { fv[0], tr, fv[1], bl };
    }
    virtual ~Rect() {};
    static std::string class_id() { return "Rect"; }
    Graph_lib::Shape* get_shape(const std::vector<FPoint>& fv) const;
    // polyline rectangle
    std::string get_id() const { return class_id(); }
};

class Circ : public figure {
public:
    Circ(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Circ: ... points.");
    }
    static std::string class_id() {
        return "Circ";
    }
    Graph_lib::Shape* get_shape(const std::vector<FPoint>& fv) const;
    std::string get_id() const {
        return class_id();
    }
};

class Line : public figure {
public:
    Line(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() < 2)
            throw std::runtime_error("Line: ... points.");
    }
    static std::string class_id() {
        return "Line";
    }
    Graph_lib::Shape* get_shape(const std::vector<FPoint>& fv) const;
    std::string get_id() const {
        return class_id();
    }
};

std::pair<FPoint, FPoint> get_transformation(
    const std::pair<FPoint, FPoint>& obj_bbox,
    const std::pair<FPoint, FPoint>& win_bbox);

figure* get_figure(std::istream& is);
pair<FPoint, FPoint> map_bbox(const vector<figure*>& vf);
std::string window_title();

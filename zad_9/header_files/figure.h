#pragma once
#include "Graph.h"
#include "FPoint.h"
#include "Matrix.h"

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
        const FPoint& scale = {1.0f, 1.0f},  
        const FPoint& trans = {0.0f, 0.0f}) const = 0;
    virtual std::string get_id() const = 0;
    virtual void rotate(float angle, FPoint center) {
        // Step 2: translate the rectangle so that the center is at the origin
        Matrix<float> trans1 = Matrix<float>::translateMx(center.x, center.y);
        // Step 3: rotate the rectangle
        Matrix<float> rot = trans1 * Matrix<float>::rotateMx(angle);
        // Step 4: translate the rectangle back to its original position
        Matrix<float> trans2 = rot * Matrix<float>::translateMx(-center.x, -center.y);
        // Step 5: apply the transformations to the rectangle
        for (auto& p : fdef) {
            p = trans2.transform(p);
        }
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
    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;
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
    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;
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
    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;
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

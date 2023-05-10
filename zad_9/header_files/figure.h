#pragma once
#include <iostream>
#include <vector>
#include "Graph.h"
#include "FPoint.h"

float distance(const FPoint& p1, const FPoint& p2);

std::ostream& operator<<(std::ostream& os, const FPoint& p);
std::istream& operator>>(std::istream& is, FPoint& p);
std::ostream& operator<<(std::ostream& os, const std::vector<FPoint>& db1);
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
    // get_points() returns a vector of points that define the figure
    virtual std::vector<FPoint> get_points() const {
        return fdef;
    }
    static std::string class_id() { return "Unknown"; }
    virtual Graph_lib::Shape* get_shape( 
        const FPoint& scale = {1.0f, 1.0f},  
        const FPoint& trans = {0.0f, 0.0f}) const = 0;
    virtual std::string get_id() const = 0;
    friend std::istream& operator>>(std::istream& is, figure& f);
    friend std::ostream& operator<<(std::ostream& os, const figure& f);
};


class Rect : public figure {
public:
    Rect(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Rect: ... points.");
    }
    virtual ~Rect() {};
    static std::string class_id() { return "Rect"; }
    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;
    std::string get_id() const { return class_id(); }
    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) {
        return new Graph_lib::Rectangle(p1, p2);
    }
    std::vector<FPoint> get_points() const {
        
    }
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
    std::vector<FPoint> get_points() const {

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

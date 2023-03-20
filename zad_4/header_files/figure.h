#pragma once
#include <iostream>
#include <vector>
#include "Graph.h"

class FPoint {
public:
    float x, y;
    operator Graph_lib::Point() const {
        return Graph_lib::Point(int(this->x), int(this->y)); }
    FPoint(float cx = 0.0f, float cy = 0.0f) : x(cx), y(cy) {}
};

float distance(const FPoint& p1, const FPoint& p2);


std::ostream& operator<<(std::ostream& os, const FPoint& p);
std::istream& operator>>(std::istream& is, FPoint& p);

std::vector<FPoint> get_points(std::istream& is);
std::string get_id(std::istream& is);

class figure {
protected:
    std::string id;
    std::vector<FPoint> fdef;
public:
    figure(const std::vector<FPoint>& fv = {}) : fdef(fv) {}
    virtual ~figure() {}
    static std::string class_id() { return "Unknown"; }
    virtual Graph_lib::Shape* get_shape() const = 0;
    friend std::istream& operator>>(std::istream& is, figure& f);
    friend std::ostream& operator<<(std::ostream& os, const figure& f);
    virtual std::string get_id() const = 0;
};


class Rect : public figure {
public:
    Rect(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Rect: ... points.");
    }
    virtual ~Rect() {};
    static std::string class_id() { return "Rect"; }
    Graph_lib::Shape* get_shape() const
    {
        return new Graph_lib::Rectangle(fdef[0], fdef[1]);
    }
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
    Graph_lib::Shape* get_shape() const
    {
        return new Graph_lib::Circle(fdef[0], distance(fdef[0], fdef[1]));
    }
    std::string get_id() const {
        return class_id();
    }
};

figure* get_figure(std::istream& is);


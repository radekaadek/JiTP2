class Temp {
public:
    virtual void print(std::ostream& os) const = 0;
    virtual ~Temp() {}
};

class Temp_C : public Temp {
private:
    double value;
public:
    Temp_C(double celsius = 0) : value(celsius) {}
    void print(std::ostream& os) const override {
        os << value << "C";
    }
};

class Temp_K : public Temp {
private:
    double value;
public:
    Temp_K(double kelvin = 0) : value(kelvin) {}
    void print(std::ostream& os) const override {
        os << value << "K";
    }
};

std::ostream& operator<<(std::ostream& os, const Temp& temp) {
    temp.print(os);
    return os;
}
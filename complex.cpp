#include <cstdio>
#include <cmath>

class Complex {
    public:
    virtual double real() const = 0;
    virtual double im() const = 0;
    virtual double mod() const = 0;
    virtual double arg() const = 0;

    virtual void print() const {
        double r_ = real();
        double im_ = im();
        if (fabs(im_) < 1e-9) printf("%g", r_); 
        else if (fabs(r_) < 1e-9) printf("%gi", im_);
        else printf("%g%+gi", r_, im_);
    }
};

class Algebratic : public Complex {
    private:
    double a, b;

    public:
    Algebratic(double aa = 0, double bb = 0) : a(aa), b(bb) {}
    double real() const override {return a;}
    double im() const override {return b;}
    double mod() const override {return sqrt(a*a + b*b);}
    double arg() const override {return atan2(b, a);}
};

class Trigonometric : public Complex {
    private:
    double r, phi;

    public:
    Trigonometric(double rr = 0, double angle = 0) : r(rr), phi(angle) {}
    double real() const override {return r*cos(phi);}
    double im() const override {return r*sin(phi);}
    double mod() const override {return r;}
    double arg() const override {return phi;}
};

Algebratic operator+(const Complex& a, const Complex& b) {
    return Algebratic(a.real() + b.real(), a.im() + b.im());
};  
Algebratic operator-(const Complex& a, const Complex& b) {
    return Algebratic(a.real() - b.real(), a.im() - b.im());
};
Algebratic operator-(const Complex& a) {
    return Algebratic(-a.real(), -a.im());
};
Algebratic operator*(const Complex& a, const Complex& b) {
    double mod_ = a.mod() * b.mod();
    double arg_ = a.arg() + b.arg();
    return Algebratic(mod_*cos(arg_), mod_*sin(arg_));
};

Algebratic operator/(const Complex& a, const Complex& b) {
    double mod_ = a.mod() / b.mod();
    double arg_ = a.arg() - b.arg();
    return Algebratic(mod_*cos(arg_), mod_*sin(arg_));
};

Algebratic take_sqrt(const Complex& a) {
    double r = sqrt(a.mod());
    double phi = a.arg() / 2.0;
    return Algebratic(r*cos(phi), r*sin(phi));
};

void discriminant(const Complex& a, const Complex& b, 
    const Complex& c, Algebratic& x_1, Algebratic& x_2) {
        Algebratic four(4, 0);
        Algebratic two(2, 0);
        Algebratic two_a = two * a;
        Algebratic m_b = -b;
        Algebratic D = (b * b) - (four * (a * c));
        //printf("дискри "); two.print();
        x_1 = (m_b + take_sqrt(D)) / two_a;
        x_2 = (m_b - take_sqrt(D)) / two_a;

};

int main() {
    Algebratic p1(4, -5);
    Trigonometric p2(3, M_PI/6);
    p1.print(); printf("\n");
    p2.print(); printf("\n");

    Algebratic summa = p1 + p2;
    Algebratic sth = p1 * p2;
    Algebratic minus = p1 - p2;
    Algebratic delit = p1 / p2;
    summa.print(); printf("\n");
    sth.print(); printf("\n");
    minus.print(); printf("\n");
    delit.print(); printf("\n");

    Algebratic a(1, 0), b(-5, 0), c(4, 0);
    Algebratic x_1, x_2;
    discriminant(a, b, c, x_1, x_2);
    x_1.print(); printf("\n");
    x_2.print(); printf("\n");

    return 0;
}

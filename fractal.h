#pragma once

#include <vector>

struct Point {
    double x, y;
    Point() {}
    Point(const double x_, const double y_) : x(x_), y(y_) {}
};

struct AffineTransform {
    double matrix[4];
    double vec[2];
    double coef[1];
    AffineTransform() {}
    AffineTransform(const std::vector<double>& params) {
        matrix[0] = params[0];
        matrix[1] = params[1];
        matrix[2] = params[2];
        matrix[3] = params[3];
        vec[0] = params[4];
        vec[1] = params[5];
        coef[0] = params[6];
    }
};

using AT = AffineTransform;

struct bound_t {
    bool automatic;
    double left;
    double right;
    double lower;
    double upper;
};

Point apply(const Point& p, const AT& at);

AT compose(const AT& right, const AT& left);

class Fractal {
private:
    std::vector<Fractal*> refs;         // ссылки на другие аттракторы
    std::vector<AT> ref_data;           // параметры преобразований: матрица, вектор, коэффиицент
    std::vector<double> dets;           // определители преобразований для нормальной(надеюсь) отрисовки
    double total;                       // сумма модулей определителей преобразований
    int critical;                       // если на отрисовку выделено меньше точек, она не производится.
    bound_t b;                          // границы области рендеринга

public:
    Fractal();
    
    void set_b(bound_t& b_);

    void init(std::vector<Fractal*>& r, std::vector<AT>& rd);

    bound_t calc_b (    // считаем границы области рендеринга
        AT at = AT({ 1, 0, 0, 1, 0, 0, 1 }), int n = 1e6, int d = 0
    ) const;

    void draw(  // рисуем фрактал на двумерном векторе
        std::vector<std::vector<int>>& canvas,
        AT at = AT({ 1, 0, 0, 1, 0, 0, 1 }), int n = 1e6, bool t = false
    ) const;
};
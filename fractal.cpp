#include <algorithm>
#include "fractal.h"

Point apply(const Point& p, const AT& at) {
    return Point(
        (p.x * at.matrix[0] + p.y * at.matrix[2] + at.vec[0]) * at.coef[0],
        (p.x * at.matrix[1] + p.y * at.matrix[3] + at.vec[1]) * at.coef[0]
    );
}

AT compose(const AT& right, const AT& left) {
    return AT({
        left.coef[0] * (left.matrix[0] * right.matrix[0] + left.matrix[1] * right.matrix[2]),
        left.coef[0] * (left.matrix[0] * right.matrix[1] + left.matrix[1] * right.matrix[3]),
        left.coef[0] * (left.matrix[2] * right.matrix[0] + left.matrix[3] * right.matrix[2]),
        left.coef[0] * (left.matrix[2] * right.matrix[1] + left.matrix[3] * right.matrix[3]),
        left.coef[0] * (left.vec[0] * right.matrix[0] + left.vec[1] * right.matrix[2]) + right.vec[0],
        left.coef[0] * (left.vec[0] * right.matrix[1] + left.vec[1] * right.matrix[3]) + right.vec[1],
        right.coef[0]
    });
}

Fractal::Fractal() {}
    
void Fractal::set_b(bound_t& b_) {
    b = b_;
}

void Fractal::init(std::vector<Fractal*>& r, std::vector<AT>& rd) {
    double max_det = 0;
    refs = r;
    ref_data = rd;
    total = 0;
    dets.resize(r.size());
    for (size_t i = 0; i != refs.size(); ++i) {
        auto det = rd[i].matrix[0] * rd[i].matrix[3] - rd[i].matrix[1] * rd[i].matrix[2];
        det *= rd[i].coef[0] * rd[i].coef[0];
        dets[i] = std::abs(det);
        total += dets[i];
        max_det = std::max(max_det, dets[i]);
    }
    critical = max_det / (total - max_det) + 3;
}
bound_t Fractal::calc_b (
    AT at, int n, int d
) const {
    if (n < critical) {
        auto p = apply({0, 0}, at);
        return { false, p.x, p.x, p.y, p.y };
    }
    double temp_ = total;
    bound_t res;
    for (size_t i = 0; i != dets.size(); ++i) {
        int k = (dets[i] / temp_) * n;
        temp_ -= dets[i];
        n -= k;
        if (i == 0) {
            res = refs[i]->calc_b(compose(at, ref_data[i]), k, d + 1);
        } else {
            auto temp = refs[i]->calc_b(compose(at, ref_data[i]), k, d + 1);
            res.left = std::min(res.left, temp.left);
            res.right = std::max(res.right, temp.right);
            res.lower = std::min(res.lower, temp.lower);
            res.upper = std::max(res.upper, temp.upper);
        }
    }
    return res;
}

void Fractal::draw(
    std::vector<std::vector<int>>& canvas,
    AT at, int n, bool t
) const {
    if (n < critical) {
        auto p = apply({0, 0}, at);
        if (
            p.x >= b.left &&
            p.y >= b.lower &&
            p.x <= b.right &&
            p.y <= b.upper
        ) {
            canvas[
                (p.x-b.left)/(b.right-b.left) * 1000
            ][
                (p.y-b.lower)/(b.upper-b.lower) * 1000
            ] = true;
        }
        return;
    }
    double temp_ = total;
    for (size_t i = 0; i != dets.size(); ++i) {
        int k = (dets[i] / temp_) * n;
        temp_ -= dets[i];
        n -= k;
        if (i == 3 && k <= 3) {
            auto p = apply({ 0, 0 }, at);
            refs[i]->draw(canvas, compose(at, ref_data[i]), k, true);
        } else {
            refs[i]->draw(canvas, compose(at, ref_data[i]), k, false);
        }
    }
    return;
}
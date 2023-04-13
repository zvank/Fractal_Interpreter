#include <algorithm>
#include <vector>

struct Point {
	double x, y;
	Point() {}
	Point(const double x_, const double y_) : x(x_), y(y_) {}
};

struct AffineTransform {
	double matrix[4];
	double vec[2];
	double coef;
	AffineTransform() {}
	AffineTransform(const std::vector<double>& params) {
		matrix[0] = params[0];
		matrix[1] = params[1];
		matrix[2] = params[2];
		matrix[3] = params[3];
		vec[0] = params[4];
		vec[1] = params[5];
		coef = params[6];
	}
};

using AT = AffineTransform;
using bound_t = std::pair<std::pair<double, double>, std::pair<double, double>>;

Point apply(const Point& p, const AT& at) {
	return Point(
		(p.x * at.matrix[0] + p.y * at.matrix[2] + at.vec[0]) * at.coef,
		(p.x * at.matrix[1] + p.y * at.matrix[3] + at.vec[1]) * at.coef
	);
}

AT compose(const AT& right, const AT& left) {
	return AT({
		left.coef * (left.matrix[0] * right.matrix[0] + left.matrix[1] * right.matrix[2]),
		left.coef * (left.matrix[0] * right.matrix[1] + left.matrix[1] * right.matrix[3]),
		left.coef * (left.matrix[2] * right.matrix[0] + left.matrix[3] * right.matrix[2]),
		left.coef * (left.matrix[2] * right.matrix[1] + left.matrix[3] * right.matrix[3]),
		left.coef * (left.vec[0] * right.matrix[0] + left.vec[1] * right.matrix[2]) + right.vec[0],
		left.coef * (left.vec[0] * right.matrix[1] + left.vec[1] * right.matrix[3]) + right.vec[1],
		right.coef
	});
}

class Fractal {
private:
	std::vector<Fractal*> refs;			// ссылки на другие аттракторы
	std::vector<AT> ref_data;			// параметры преобразований: матрица, вектор, коэффиицент
	std::vector<double> dets;			// определители преобразований для нормальной(надеюсь) отрисовки
	double total;						// сумма модулей определителей преобразований
	int critical;						// если на отрисовку выделено меньше точек, она не производится.
	bound_t b;							// границы области рендеринга

public:
	Fractal() {}
	
	void set_b(bound_t& b_) {
		b = b_;
		// std::cout << "-> v: ";
		// for (auto x : dets) {
		// 	std::cout << x << " ";
		// }
		// std::cout << "\n-> t: " << total << "\n";
	}

	void init(std::vector<Fractal*>& r, std::vector<AT>& rd) {
		double max_det = 0;
		refs = r;
		ref_data = rd;
		total = 0;
		dets.resize(r.size());
		for (size_t i = 0; i != refs.size(); ++i) {
			auto det = rd[i].matrix[0] * rd[i].matrix[3] - rd[i].matrix[1] * rd[i].matrix[2];
			det *= rd[i].coef * rd[i].coef;
			dets[i] = std::abs(det);
			total += dets[i];
			max_det = std::max(max_det, dets[i]);
		}
		critical = max_det / (total - max_det) + 3;
	}
	bound_t calc_b (	// считаем границы области рендеринга
		AT at = AT({ 1, 0, 0, 1, 0, 0, 1 }), int n = 1e6, int d = 0
	) const {
		if (n < critical) {
			auto p = apply({0, 0}, at);
			return { { p.x, p.x }, { p.y, p.y } };
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
				res.first.first = std::min(res.first.first, temp.first.first);
				res.first.second = std::max(res.first.second, temp.first.second);
				res.second.first = std::min(res.second.first, temp.second.first);
				res.second.second = std::max(res.second.second, temp.second.second);
			}
		}
		return res;
	}

	void draw(	// рисуем фрактал на двумерном векторе
		std::vector<std::vector<int>>& canvas,
		AT at = AT({ 1, 0, 0, 1, 0, 0, 1 }), int n = 1e6, bool t = false
	) const {
		if (n < critical) {
			auto p = apply({0, 0}, at);
			if (t) {
				// std::cout << "-> p: " << p.x << " " << p.y << "\n";
			}
			if (
				p.x >= b.first.first &&
				p.y >= b.second.first &&
				p.x <= b.first.second &&
				p.y <= b.second.second
			) {
				canvas[
					(p.x-b.first.first)/(b.first.second-b.first.first) * 1000
				][
					(p.y-b.second.first)/(b.second.second-b.second.first) * 1000
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
				// std::cout << "-> draw [" << i << "] with " << k << " points\n";
				auto p = apply({ 0, 0 }, at);
				// std::cout << "-> p was " << p.x << " " << p.y << "\n";
				refs[i]->draw(canvas, compose(at, ref_data[i]), k, true);
			} else {
				refs[i]->draw(canvas, compose(at, ref_data[i]), k, false);
			}
		}
		return;
	}
};
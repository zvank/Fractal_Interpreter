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

public:
	Fractal() {}
	
	void init(std::vector<Fractal*>& r, std::vector<AT>& rd) {
		double max_det = 0;
		refs = r;
		ref_data = rd;
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

	void draw(std::vector<std::vector<bool>>& canvas, AT at = AT({ 1, 0, 0, 1, 0, 0, 1 }), int n = 1e6) {
		// std::cout << "-> draw, n = " << n << "\n";
		if (n < critical) {
			auto p = apply({0, 0}, at);
			if (p.x >= 0 && p.y >= 0 && p.x <= 0.999 && p.y <= 0.999) {
				// std::cout << "-> new point on " << int(1000*p.x) << " " << int(1000*p.y) << "\n";
				canvas[1000*p.x][1000*p.y] = true;
				// std::cout << "-> continue\n";
			}
			return;
		}
		double temp_ = total;
		for (size_t i = 0; i != dets.size(); ++i) {
			int k = (dets[i] / temp_) * n;
			temp_ -= dets[i];
			n -= k;
			refs[i]->draw(canvas, compose(at, ref_data[i]), k);
		}
		return;
	}
};
#include <TXLib.h>
#include "interpreter.h"

int main() {
	FractalReader fr;
	auto x = fr.run();
	txCreateWindow(505, 505);
	for (size_t i = 0; i != x.size(); ++i) {
		for (size_t j = 0; j != x.size(); ++j) {
			if (x[i][j]) {
				txSetPixel(i, 500 - j, TX_white);
			}
		}
	}
}
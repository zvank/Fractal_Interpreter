#include "image.h"
#include "interpreter.h"

int main() {
    FractalReader fr;
    fr.read(std::cin);
    auto x = fr.run();
    Image img(x.size(), x[0].size());
    for (size_t i = 0; i != x.size(); ++i) {
        for (size_t j = 0; j != x.size(); ++j) {
            if (x[i][j]) {
                img.SetPixel({255, 255, 255}, x.size() - j - 1, i);
            } else {
                img.SetPixel({0, 0, 0}, x.size() - j - 1, i);
            }
        }
    }
    img.Write("img.png");
}
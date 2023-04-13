#pragma once

#include <vector>
#include <map>
#include <iostream>
#include "fractal.h"

// #define DEBUG true

// class FractalReader {
// private:
//     std::vector<Fractal> data;
//     std::map<std::string, int> indices;
//     bound_t b;
//     bool auto_b;

// public:
//     FractalReader() : auto_b(false), b({ { 0, 0.5 }, { 0, 0.5 } }) {
//         int i = 0;
//         indices["main"] = i++;
//         std::string s;
//         std::cin >> s;
//         while (s[0] == '!'){
//             if (s == "!add") {
//                 std::cin >> s;
//                 while (s.back() != ';') {
//                     std::cin >> s;
//                     if (s.back() == ',') {
//                         s.pop_back();
//                         indices[s] = i++;
//                     } else if (s.back() == ';') {
//                         s.pop_back();
//                         indices[s] = i++;
//                         s.push_back(';');
//                     }
//                 }
// #if DEBUG
//                 std::cout << "-> read !add\n";
// #endif
//             } else if (s == "!bounds") {
//                 std::cin >> s;
//                 std::cin >> s;
//                 if (s == "auto;") {
//                     auto_b = true;
//                 } else {
//                     std::cin >> s;
//                     b.first.first = std::stod(s);
//                     std::cin >> s;
//                     b.first.second = std::stod(s);
//                     std::cin >> s;
//                     std::cin >> s;
//                     std::cin >> s;
//                     b.second.first = std::stod(s);
//                     std::cin >> s;
//                     b.second.second = std::stod(s);
//                     std::cin >> s;
//                 }
// #if DEBUG
//                 std::cout << "-> read !bounds\n";
// #endif
//             }
//             std::cin >> s;
//         }
    
//         // в данный момент s содержит название первого описываемого аттрактора вне зависимости от того, был ли !add
//         data.resize(i);
//         while (s != "exec")	{
// #if DEBUG
//             std::cout << "-> read new name\n";
// #endif
//             int curr = indices.at(s);
//             while (s.back() != '{') {
//                 std::cin >> s;
//             }
//             std::vector<Fractal*> refs;
//             std::vector<AT> ats;
//             while (true) {
//                 std::cin >> s;
//                 refs.push_back(&data[indices[s]]);
//                 std::vector<double> temp_at;
//                 bool empty = true;
// #if DEBUG
//                 std::cout << "-> reading matrix...\n";
// #endif
//                 std::cin >> s;
//                 while (s.back() != ']') {		// считываем матрицу
//                     if (s[0] != '[') {
//                         empty = false;
//                         temp_at.push_back(std::stof(s));
//                     }
//                     std::cin >> s;
//                 }
//                 if (empty) {
//                     temp_at.push_back(1);
//                     temp_at.push_back(0);
//                     temp_at.push_back(0);
//                     temp_at.push_back(1);
//                 }
//                 empty = true;
// #if DEBUG
//                 std::cout << "-> reading vector...\n";
// #endif
//                 std::cin >> s;
//                 while (s.back() != ']') {		// считываем вектор
//                     if (s[0] != '[') {
//                         empty = false;
//                         temp_at.push_back(std::stof(s));
//                     }
//                     std::cin >> s;
//                 }
//                 if (empty) {
//                     temp_at.push_back(0);
//                     temp_at.push_back(0);
//                 }
//                 empty = true;
// #if DEBUG
//                 std::cout << "-> reading coef...\n";
// #endif
//                 std::cin >> s;
//                 while (s.back() != ']') {		// считываем коэффициент
//                     if (s[0] != '[') {
//                         empty = false;
// #if DEBUG
//                         std::cout << "-> converting " << s << "...\n";
// #endif
//                         temp_at.push_back(std::stof(s));
//                     }
//                     std::cin >> s;
//                 }
//                 if (empty) {
//                     temp_at.push_back(1);
//                 }
//                 ats.push_back(temp_at);
//                 std::cin >> s;
//                 if (s == "}") {
//                     std::cin >> s;
//                     break;
//                 }
//             }
//             data[curr].init(refs, ats);
//         }
//     }

//     std::vector<std::vector<int>> run() {
//         if (auto_b) {
//             b = data[0].calc_b();
//         }
//         for (int i = 0; i != data.size(); ++i) {
//             data[i].set_b(b);
//         }
//         std::vector<std::vector<int>> res(1001, std::vector<int>(1001));
//         data[0].draw(res);
//         return res; 
//     }
// };

class FractalReader {
public:
    FractalReader();

    void read(std::istream& in);

    std::vector<std::vector<int>> run();

private:
    bound_t bound_;
    std::vector<Fractal> data_;
    std::map<std::string, int> indices_;
};
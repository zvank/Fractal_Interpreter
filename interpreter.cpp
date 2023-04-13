#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <optional>
#include <cassert>

#include "interpreter.h"

enum eTType {
    flag,
    name,
    exec,
    number,
    plus,
    open_square_bracket,
    close_square_bracket,
    open_figure_bracket,
    close_figure_bracket,
    equal,
    comma,
    semicolon,
};

struct Token {
    eTType type_;
    std::string data_;
};

#define check(ptr, type) assert(ptr->type_ == eTType::type)

void fill (int n, std::vector<Token>::iterator& curr, double* to) {
    check(curr, open_square_bracket);
    ++curr;
    if (curr->type_ == eTType::number) {
        for (int i = 0; i < n; ++i) {
            check(curr, number);
            to[i] = std::stod(curr->data_);
            ++curr;
        }
        check(curr, close_square_bracket);
        ++curr;
    } else {
        check(curr, close_square_bracket);
        ++curr;
        switch(n) {
            case 1:
                to[0] = 1;
                break;
            case 2:
                to[0] = 0;
                to[1] = 0;
                break;
            case 4:
                to[0] = 1;
                to[1] = 0;
                to[2] = 0;
                to[3] = 1;
                break;
        }
    }
}

inline std::optional<Token> get_token(std::istream& in) {
    std::cout << "bebra\n";
    while (in.peek() != std::char_traits<char>::eof() && isspace(in.peek())) {
        std::cout << "1\n";
        char c;
        in.get(c);
    }
    if (in.peek() == std::char_traits<char>::eof()) {
        std::cout << "2\n";
        return {};
    } else {
        std::cout << "22\n";
        Token t;
        t.data_.push_back(' ');
        in.get(t.data_.back());
        if (t.data_.back() == '!') {
            while (isalpha(in.peek())) {
                std::cout << "3\n";
                t.data_.push_back(' ');
                in.get(t.data_.back());
            }
            t.type_ = eTType::flag;
        } else if (isalpha(t.data_.back())) {
            while (isalpha(in.peek()) || isdigit(in.peek())) {
                std::cout << "4\n";
                t.data_.push_back(' ');
                in.get(t.data_.back());
            }
            t.type_ = t.data_ == "exec" ? eTType::exec : eTType::name;
        } else if (isdigit(t.data_.back())) {
            while (isdigit(in.peek()) || in.peek() == '.') {
                std::cout << "5\n";
                t.data_.push_back(' ');
                in.get(t.data_.back());
            }
            t.type_ = eTType::number;
        } else {
            std::cout << "6\n";
            switch (t.data_.back()) {
                case '+':
                    t.type_ = eTType::plus;
                    break;
                case '[':
                    t.type_ = eTType::open_square_bracket;
                    break;
                case ']':
                    t.type_ = eTType::close_square_bracket;
                    break;
                case '{':
                    t.type_ = eTType::open_figure_bracket;
                    break;
                case '}':
                    t.type_ = eTType::close_figure_bracket;
                    break;
                case '=':
                    t.type_ = eTType::equal;
                    break;
                case ',':
                    t.type_ = eTType::comma;
                    break;
                case ';':
                    t.type_ = eTType::semicolon;
                    break;
                case '#':
                    std::getline(in, t.data_);
                    return {};
                default:
                    return {};
            }
        }
        return t;
    }
}

inline std::vector<Token> tokenise(std::istream& in) {
    std::vector<Token> res;
    while (in.peek() != std::char_traits<char>::eof() && (res.empty() || res.back().type_ != eTType::exec)) {
        auto token_opt = get_token(in);
        std::cout << "a\n";
        if (token_opt.has_value()) {
            res.push_back(token_opt.value());
        }
    }
    return res;
}

FractalReader::FractalReader() : bound_({false, 0, 0.5, 0, 0.5}), data_({{}}), indices_({{"main", 0}}) {
}

void FractalReader::read(std::istream& in) {
    std::vector<Token> tokens = tokenise(in);
    std::cout << "exited\n";
    auto current = tokens.begin();
    auto end = tokens.end();
    while (current != end) {
        std::cout << "b\n";
        if (current->type_ == eTType::flag) {
            if (current->data_ == "!add") {
                ++current;
                check(current, equal);
                ++current;
                check(current, name);
                data_.push_back({});
                indices_[current->data_] = data_.size() - 1;
                while ((++current)->type_ != eTType::semicolon) {
                    check(current, comma);
                    ++current;
                    check(current, name);
                    data_.push_back({});
                    indices_[current->data_] = data_.size() - 1;
                }
            } else if (current->data_ == "!bounds") {
                ++current;
                if (current->type_ == eTType::name && current->data_ == "auto") {
                    ++current;
                    check(current, semicolon);
                    bound_.automatic = true;
                } else {
                    double temp[2];
                    fill(2, current, temp);
                    bound_.left = temp[0];
                    bound_.right = temp[1];
                    fill(2, current, temp);
                    bound_.lower = temp[0];
                    bound_.upper = temp[1];
                }
            }
            break;
        } else if (current->type_ == eTType::name) {
            std::string s = current->data_;
            std::vector<Fractal*> fractals;
            std::vector<AT> ats;
            ++current;
            check(current, equal);
            ++current;
            check(current, open_figure_bracket);
            ++current;
            while (true) {
                check(current, name);
                std::string curr_name = current->data_;
                ++current;
                AT at;
                fill(4, current, at.matrix);
                fill(2, current, at.vec);
                fill(1, current, at.coef);
                fractals.push_back(&data_[indices_[curr_name]]);
                ats.push_back(at);
                if (current->type_ == close_figure_bracket) {
                    ++current;
                    break;
                } else {
                    check(current, plus);
                    ++current;
                }
            }
            data_[indices_[s]].init(fractals, ats);
            break;

        } else if (current->type_ == eTType::exec) {
            return;
        } else {
            std::cerr << "parsing error\n";
            assert(false);
        }
    }
}

std::vector<std::vector<int>> FractalReader::run() {
    if (bound_.automatic) {
        bound_ = data_[0].calc_b();
    }
    for (int i = 0; i != data_.size(); ++i) {
        data_[i].set_b(bound_);
    }
    std::vector<std::vector<int>> res(1001, std::vector<int>(1001));
    data_[0].draw(res);
    return res; 
}

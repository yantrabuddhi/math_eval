//
// Created by mande on 2025-02-20.
//

#include "def_math.h"
#include <cmath>
namespace meval {
    double add(const double a, const double b, const double epsilon) {
        return a + b;
    }
    double sub(const double a, const double b, const double epsilon) {
        return a - b;
    }
    double mul(const double a, const double b, const double epsilon) {
        return a * b;
    }
    double div(const double a, const double b, const double epsilon) {
        if (fabs(b) < epsilon) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }
    double mod(const double a, const double b, const double epsilon) {
        return fmod(a, b);
    }

    double pow(const double a, const double b, const double epsilon) {
        return std::pow(a, b);
    }

    void init_def_vars(const std::shared_ptr<var_map>& vmap) {
        (*vmap)["pi"] = 3.14159265358979323846;
        (*vmap)["e"] = 2.71828182845904523536;
        (*vmap)["x"] = 0.0;
        (*vmap)["y"] = 0.0;
    }

    void init_def_funcs(const std::shared_ptr<func_map>& fmap) {
        (*fmap)["sin"] = sin;
        (*fmap)["cos"] = cos;
        (*fmap)["tan"] = tan;
        (*fmap)["asin"] = asin;
        (*fmap)["acos"] = acos;
        (*fmap)["atan"] = atan;
        (*fmap)["sinh"] = sinh;
        (*fmap)["cosh"] = cosh;
        (*fmap)["tanh"] = tanh;
        (*fmap)["asinh"] = asinh;
        (*fmap)["acosh"] = acosh;
        (*fmap)["atanh"] = atanh;
        (*fmap)["sqrt"] = sqrt;
        (*fmap)["log"] = log;
        (*fmap)["log10"] = log10;
        (*fmap)["exp"] = exp;
        (*fmap)["abs"] = fabs;
        (*fmap)["ceil"] = ceil;
        (*fmap)["floor"] = floor;
        (*fmap)["round"] = round;
    }

    void init_def_ops(const std::shared_ptr<operator_map>& omap) {
        uint32_t i = 1;
        (*omap)["+"] = std::pair<func_binary,uint32_t>{add,i};
        (*omap)["-"] = std::pair<func_binary,uint32_t>{sub,i};
        (*omap)["*"] = std::pair<func_binary,uint32_t>{mul,i};
        (*omap)["/"] = std::pair<func_binary,uint32_t>{div,i};
        (*omap)["%"] = std::pair<func_binary,uint32_t>{mod,i};
        (*omap)["^"] = std::pair<func_binary,uint32_t>{pow,i};
    }
} // mpl
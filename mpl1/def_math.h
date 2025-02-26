//
// Created by mande on 2025-02-20.
//

#ifndef DEF_MATH_H
#define DEF_MATH_H

#include "meval.h"

namespace meval {

    double add(double a, double b, double epsilon = 1e-20);
    double sub(double a, double b, double epsilon = 1e-20);
    double mul(double a, double b, double epsilon = 1e-20);
    double div(double a, double b, double epsilon = 1e-20);
    double mod(double a, double b, double epsilon = 1e-20);
    double pow(double a, double b, double epsilon = 1e-20);
    void init_def_vars(const std::shared_ptr<var_map>& vmap);
    void init_def_funcs(const std::shared_ptr<func_map>& fmap);
    void init_def_ops(const std::shared_ptr<operator_map>& omap);

} // mpl

#endif //DEF_MATH_H

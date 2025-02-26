#include <iostream>
#include "def_math.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::string s{"$x+.1"};
    std::shared_ptr<meval::var_map> vars = std::make_shared<meval::var_map>();
    std::shared_ptr<meval::func_map> funcs = std::make_shared<meval::func_map>();
    std::shared_ptr<meval::operator_map> ops = std::make_shared<meval::operator_map>();
    meval::init_def_vars(vars);
    meval::init_def_funcs(funcs);
    meval::init_def_ops(ops);
    try {
        meval::math_expr mexp(s, vars, funcs, ops);
        std::cout << mexp.eval() << std::endl;
        (*vars)["x"] = 1.0;
        std::cout << mexp.eval() << std::endl;
    }catch (const meval::meval_error& merr) {
        std::cout << merr.what() << std::endl;
    }
    //system("pause");
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
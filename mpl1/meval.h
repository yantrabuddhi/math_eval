#ifndef MEVAL_H
#define MEVAL_H

#include <string_view>
#include <string>
#include <map>
#include <memory>
#include <stack>
#include <vector>
#include <exception>
#include <cstdint>
#include <functional>

namespace meval {

    class meval_error final : public std::exception {
    public:
        enum class error_type {
            unknown_token,
            unknown_operator,
            unknown_function,
            unknown_variable,
            invalid_expression,
        };
        explicit meval_error(const std::string &msg,error_type et, uint32_t pos=0);
        [[nodiscard]] const char* what() const noexcept override;
        [[nodiscard]] error_type get_error_type() const noexcept { return m_type; }
        [[nodiscard]] uint32_t get_pos() const noexcept { return m_pos; }
    private:
        std::string m_msg;
        error_type m_type;
        uint32_t m_pos;
    };

    typedef std::function<double(double)> func_unary;
    typedef std::function<double(double,double,double)> func_binary;
    typedef std::map<std::string_view,func_unary> func_map;
    typedef std::map<std::string_view,std::pair<func_binary,uint32_t>> operator_map;
    typedef std::map<std::string_view,double> var_map;

    class math_expr {
    public:
        enum class token_type {
            number,
            variable,
            function,
            operator_binary,
            open_bracket,
            close_bracket,
            eof
        };


        static const char sym_var_start;// = "$";
        static const char sym_func_start;// = "@";

        static const std::vector<char> m_reserved_symbols;
        static const std::pair<char,char> bracket;// = {"(",")"};

        explicit math_expr(const std::string& expr,
            const std::shared_ptr<var_map>& vars,
            const std::shared_ptr<func_map>& funcs,
            const std::shared_ptr<operator_map>& ops,
            double epsilon=1e-20);

        [[nodiscard]] double eval() const;
        [[nodiscard]] token_type get_token_type(std::string_view token) const;
        [[nodiscard]] double get_epsilon() const noexcept{return m_epsilon;}
        [[nodiscard]] std::string get_expr() const noexcept{return m_expr;}
    private:
        enum class token_element_type {
            digit,
            letter,
            period,
            underscore,
            open_bracket,
            close_bracket,
            comma,
            colon,
            semicolon,
            space,
            reserved_symbol,
            user_symbol,
        };

        struct token {
            token_type type;
            double number;
            std::string_view name;
        };

        std::string m_expr;
        std::shared_ptr<var_map> m_vars;
        std::shared_ptr<func_map> m_funcs;
        std::shared_ptr<operator_map> m_ops;

        double m_epsilon;
        std::stack<token> m_opr_stack;
        std::vector<token> m_postfix;
        uint32_t m_current_token_str_pos=0;

        static void stack_move(std::stack<token>& stackA, std::stack<token>& stackB);
        void reset_token_str_pos();
        token get_full_token(token_type);
        token parse_next_token_from_str();
        std::vector<token> tokenize();
        void to_postfix();

    };
}
#endif //MEVAL_H
// meval.cpp
#include "meval.h"
#include <sstream>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <stack>

namespace meval {
    // Implementation of meval_error
    meval_error::meval_error(const std::string &msg, error_type et, uint32_t pos)
        : m_msg("[Error at pos " + std::to_string(pos) + "] " + msg), m_type(et), m_pos(pos) {
    }

    const char *meval_error::what() const noexcept {
        return m_msg.c_str();
    }

    // Implementation of math_expr
    const char math_expr::sym_var_start = '$';
    const char math_expr::sym_func_start = '@';
    const std::vector<char> math_expr::m_reserved_symbols = {
        sym_var_start, sym_func_start, //sym_iota_start,
        '.',
        '(', ')'
    };
    const std::pair<char, char> math_expr::bracket = {'(', ')'};

    math_expr::math_expr(const std::string& expr,
                         const std::shared_ptr<var_map>& vars,
                         const std::shared_ptr<func_map>& funcs,
                         const std::shared_ptr<operator_map>& ops,
                         const double epsilon)
        : m_expr(expr),
          m_vars(vars),
          m_funcs(funcs),
          m_ops(ops),
          m_epsilon(epsilon) {
        //validate variable names
        //validate function names
        //validate operator names
        m_expr = "("+m_expr+")";
        to_postfix();
    }

    void math_expr::stack_move(std::stack<token>& stackA, std::stack<token>& stackB) {
        while (!stackA.empty()) {
            stackB.push(stackA.top());
            stackA.pop();
        }
    }

    void math_expr::to_postfix() {
        auto tokens = tokenize();
        //validate expression like brackets containing data and matching open close
        int obc{};
        std::stack<token> op_tkn;
        m_postfix.clear();
        for (auto &tok: tokens) {
            switch (tok.type) {
                case token_type::number:
                case token_type::variable:
                    m_postfix.push_back(tok);
                    break;
                case token_type::function:
                    op_tkn.push(tok);
                    break;
                case token_type::operator_binary:
                    if (op_tkn.empty() ||
                        ((*m_ops)[tok.name].second>(*m_ops)[op_tkn.top().name].second)) {
                        op_tkn.push(tok);
                    } else {
                        while (!op_tkn.empty() &&
                            ((*m_ops)[tok.name].second<=(*m_ops)[op_tkn.top().name].second)) {
                            m_postfix.push_back(op_tkn.top());
                            op_tkn.pop();
                        }
                        op_tkn.push(tok);
                    }
                    break;
                case token_type::open_bracket:
                    obc++;
                    op_tkn.push(tok);
                    break;
                case token_type::close_bracket:
                    if (obc<1)
                        throw meval_error("Invalid[1]: mismatched closed bracket",
                            meval_error::error_type::invalid_expression);
                    obc--;
                    while (!op_tkn.empty() && op_tkn.top().type != token_type::open_bracket) {
                        m_postfix.push_back(op_tkn.top());
                        op_tkn.pop();
                    }
                    if (op_tkn.empty())
                        throw meval_error("Invalid[2]: mismatched closed bracket",
                            meval_error::error_type::invalid_expression);
                    op_tkn.pop();
                    break;
                default:
                    throw meval_error("Unexpected token type",
                                      meval_error::error_type::unknown_token,
                                      m_current_token_str_pos);
            }
        }
        if (obc)
            throw meval_error("Invalid: mismatched open bracket",
                meval_error::error_type::invalid_expression);
    }

    double math_expr::eval() const {
        std::stack<double> ex;
        double op1{},op2{};
        for (const auto& tkn: m_postfix) {
            switch (tkn.type) {
                case token_type::number:
                    ex.push(tkn.number);
                break;
                case token_type::variable:
                    ex.push((*m_vars)[tkn.name]);
                break;
                case token_type::function:
                    op1 = ex.top();
                    ex.pop();
                    ex.push((*m_funcs)[tkn.name](op1));
                break;
                case token_type::operator_binary:
                    op2 = ex.top();
                    ex.pop();
                    op1 = ex.top();
                    ex.pop();
                    ex.push((*m_ops)[tkn.name].first(op1,op2,m_epsilon));
                break;
                default:
                    throw meval_error("Unexpected token type in postfix expression",
                        meval_error::error_type::invalid_expression);
            }
        }
        if (ex.empty())
            throw meval_error("Unexpected token type in postfix expression",
                        meval_error::error_type::invalid_expression);
        return ex.top();
    }


    std::vector<math_expr::token> math_expr::tokenize() {
        reset_token_str_pos();
        std::vector<token> tokens;
        while (m_current_token_str_pos < m_expr.size()) {
            tokens.push_back(parse_next_token_from_str());
        }
        return tokens;
    }

    math_expr::token math_expr::parse_next_token_from_str() {
        if (m_current_token_str_pos >= m_expr.size()) return token{token_type::eof, 0, ""};
        const std::string c_expr(m_expr.substr(m_current_token_str_pos));

        token_type tk_type{};

        try {
            tk_type = get_token_type(c_expr);
            return get_full_token(tk_type);
        } catch (const meval_error &err) {
            if (err.get_error_type() == meval_error::error_type::unknown_token) {
                try {
                    return get_full_token(token_type::operator_binary);
                } catch (const meval_error &) {
                    throw;//invalid_expression or unknown operator from get full token
                }
            }
            throw;//invalid_expression or unknown operator from get full token
        }
    }


    math_expr::token math_expr::get_full_token(token_type tk_type) {
        auto pos = m_current_token_str_pos;
        std::string s{};
        switch (tk_type) {
            case token_type::number:
                while (pos < m_expr.size() &&
                    (isdigit(m_expr[pos]) || m_expr[pos] == '.')) {pos++;}
                if (pos == m_current_token_str_pos)
                    throw meval_error("Unexpected number",
                        meval_error::error_type::invalid_expression,
                        m_current_token_str_pos);
                s = std::string(m_expr.substr(m_current_token_str_pos, pos - m_current_token_str_pos));
                m_current_token_str_pos = pos;
                return token{tk_type,
                    std::stod(s),
                        ""};
            case token_type::variable:
                m_current_token_str_pos++;
                for (const auto& key : (*m_vars)) {
                    if (key.first == m_expr.substr(m_current_token_str_pos, key.first.size())) {
                        m_current_token_str_pos += key.first.size();
                        return token{tk_type, 0,key.first};
                    }
                }
            throw meval_error("Unexpected variable name",
                meval_error::error_type::unknown_variable,
                m_current_token_str_pos);
            case token_type::function:
                m_current_token_str_pos++;
            for (const auto& key : (*m_funcs)) {
                auto op = m_expr.substr(m_current_token_str_pos, key.first.size());
                if (key.first == op) {
                    m_current_token_str_pos += key.first.size();
                    if (m_current_token_str_pos>=m_expr.size() ||
                        m_expr[m_current_token_str_pos]!=bracket.first) {
                        throw meval_error("function name must be followed by brackets",
                            meval_error::error_type::invalid_expression);
                    }

                    return token{tk_type, 0,key.first};
                }
            }
            throw meval_error("Unexpected function name",
                meval_error::error_type::unknown_function,
                m_current_token_str_pos);
            case token_type::operator_binary:
                for (const auto& key : (*m_ops)) {
                    auto op = m_expr.substr(m_current_token_str_pos, key.first.size());
                    if (key.first == op) {
                        m_current_token_str_pos += key.first.size();
                        return token{tk_type, 0,key.first};
                    }
                }
            throw meval_error("Unexpected operator name",
                meval_error::error_type::unknown_operator,
                m_current_token_str_pos);

            case token_type::open_bracket:
                m_current_token_str_pos++;
                return token{tk_type, 0, "("};
            case token_type::close_bracket:
                m_current_token_str_pos++;
                return token{tk_type, 0, ")"};
            case token_type::eof:
            default:
                throw meval_error("Unexpected token",
                    meval_error::error_type::unknown_token,
                    m_current_token_str_pos);
        }
    }


    void math_expr::reset_token_str_pos() {
        m_current_token_str_pos = 0;
    }

    math_expr::token_type math_expr::get_token_type(std::string_view token) const {
        if (token.empty()) return token_type::eof;
        auto tk_symbol = token[0];
        if (tk_symbol == sym_func_start) return token_type::function;
        if (tk_symbol == sym_var_start) return token_type::variable;
        if (std::isdigit(tk_symbol) || (tk_symbol == '.')) return token_type::number;
        if (tk_symbol == bracket.first) return token_type::open_bracket;
        if (tk_symbol == bracket.second) return token_type::close_bracket;

        if (m_ops->contains(token)) {
            return token_type::operator_binary;
        }
        throw meval_error("Unknown token: " + std::string(token),
                          meval_error::error_type::unknown_token,
                          m_current_token_str_pos);
    }
}

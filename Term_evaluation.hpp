/* 
 * File:   Term_evaluation.hpp
 * Author: lefty
 *
 * Created on 28. prosinec 2013, 12:06
 */

#ifndef TERM_EVALUATION_HPP
#define	TERM_EVALUATION_HPP

#include <string>
#include <map>
#include <functional>
#include <exception>

class Term_evaluation {
public:
    Term_evaluation(const std::string& term);
    /**
     * @brief Evaluates and returns value of the term using specified value function
     * @param var_fcn function taking string& variable name and returning its bool value
     * @return bool - term value
     */
    bool eval(std::function<bool(std::string&)> var_fcn);
private:
    //!< Convert term from infix to postfix notation
    std::string infix_to_postfix(const std::string& infix_term);
    // Shunting-yard algorithm helper methods:
    void pop_less_precedent_operators_to_out(char c);
    void pop_operators_to_out_until_left_parenthesis(void);
    bool is_op(char c);
    bool is_var(char c);
    
    //!< Evaluates given postfix term using var->value function
    bool eval_postfix(const std::string& postfix_term, std::function<bool(std::string&)> var_fcn);
    bool eval_op(const char operation);
    
    std::string term_postfix;
    std::vector<char> stack,out;
    std::map<unsigned char,int> operators = {{'&',2},{'|',2},{'=',1},{'^',2},{'!',3}}; //!< Operators with priorities
};

class Mismatched_parenthesis_exception : public std::exception {
public:
    virtual const char *what(void) {
        return "Expression contains mismatched parenthesis!";
    }
};

#endif	/* TERM_EVALUATION_HPP */


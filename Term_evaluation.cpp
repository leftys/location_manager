/* 
 * File:   Term_evaluation.cpp
 * Author: lefty
 * 
 * Created on 28. prosinec 2013, 12:06
 */

#include <memory>
#include <cctype>
#include <vector>
#include <glog/logging.h>

#include "Term_evaluation.hpp"

using namespace std;

Term_evaluation::Term_evaluation(const string& term) {
    term_postfix = infix_to_postfix(term);
    VLOG(1) << "Postfix: '" << term_postfix << "'";
}

bool Term_evaluation::eval(std::function<bool(string&)> var_fcn) {
    return eval_postfix(term_postfix, var_fcn);
}

std::string Term_evaluation::infix_to_postfix(const std::string &infix_term) {
    LOG_ASSERT(stack.size() == 0);
    out.clear();
    for (int i = 0; i < infix_term.size(); i++) {
        char c = infix_term[i];
        if (isblank(c)) 
            continue;
        /*else if (isdigit(c)) {
            out.push_back(c);
            out.push_back(' ');
        }*/
        else if (is_op(c)) {
            pop_less_precedent_operators_to_out(c);
            stack.push_back(c);
        } else if (c == '(') 
            stack.push_back(c);
        else if (c == ')') 
            pop_operators_to_out_until_left_parenthesis(); 
        //... ↑ including the parenthesis, which wont be pushed to output.
        else if (is_var(c)) {
            while (i < infix_term.size()) {
                out.push_back(c);
                c = infix_term[++i];
                if (!is_var(c)) {
                    --i;
                    break;
                }
            }
            out.push_back(' ');
        }
    } // Done reading term
    while (stack.size()>0) {
        char c = stack.back();
        stack.pop_back();
        if (c=='(' || c==')') throw Mismatched_parenthesis_exception();
        out.push_back(c);
    }
    out.push_back((char)0); //End of string
    return out.data();
}

void Term_evaluation::pop_less_precedent_operators_to_out(char c) {
    int preced = operators[c];
    char op;
    while ((stack.size() > 0) && (op = stack.back()) && is_op(op) && (operators[op] < preced)) {
        stack.pop_back();
        out.push_back(op);
        out.push_back(' ');
    }
}

void Term_evaluation::pop_operators_to_out_until_left_parenthesis(void) {
    char op;
    while ((op = stack.back()) && (is_op(op))) {
        stack.pop_back();
        out.push_back(op);
        out.push_back(' ');
    }
    LOG_ASSERT(stack.back() == '(');
    stack.pop_back();
}

bool Term_evaluation::is_op(char c) {
    return (bool)operators.count(c);
}

bool Term_evaluation::is_var(char c) {
    return (isalpha(c) || c=='_');
}

bool Term_evaluation::eval_postfix(const std::string &postfix_term, std::function<bool(std::string&)> var_fcn) {
    LOG_ASSERT(stack.size()==0);
    for (int i = 0; i < postfix_term.size(); i++) {
        char c = postfix_term[i];
        if (isblank(c)) continue;
        else if (is_var(c)) {
            string var;
            while (is_var(c) && i < postfix_term.size()) {
                var.push_back(c);
                c = postfix_term[++i];
            }
            bool value = var_fcn(var);
            stack.push_back(value);
        } else if (is_op(c)) {
            eval_op(c);
        }
    }
    LOG_ASSERT(stack.size()==1);
    bool res = stack.back();
    stack.pop_back();
    return res;
}

bool Term_evaluation::eval_op(const char operation) {
    bool res;
    bool arg1 = stack.back();
    stack.pop_back();
    
    // Unary operators
    if (operation == '!') {
        res = !arg1;
        stack.push_back(res);
        return res;
    }
    
    bool arg2 = stack.back();
    stack.pop_back();
    
    // Binary operators
    if (operation == '&') {
        res = arg1 & arg2;
    }
    if (operation == '|') {
        res = arg1 | arg2;
    }
    if (operation == '^') {
        res = arg1 ^ arg2;
    }
    stack.push_back(res);
    return res;
}

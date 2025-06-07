#ifndef EXPR_CPP
#define EXPR_CPP

#include <iostream>
#include "expr.h"

using namespace std;

int expr::precedence(char op) {
    return (op == '+' ? 1 : (op == '*' ? 2 : 0));
}

string expr::apply_operator(const string& a, const string& b, char op) {
    return a + op + b;
}

void expr::process(stack<string>& operands, stack<char>& operators, function<char(char)> op_map) {
    char op = operators.top(); 
    operators.pop();
    string b = operands.top(); 
    operands.pop();
    string a = operands.top(); 
    operands.pop();
    char mapped_op = op_map(op);
    string result = apply_operator(a, b, mapped_op);
    this->steps.push_back(result);
    operands.push(result);
}

void expr::evaluate_expression(bool invert /*= false*/) {
    stack<string> operands;
    stack<char> operators;

    auto op_map = invert ? [](char op) {
        return (op == '+') ? '*' : (op == '*') ? '+' : op;
    } : [](char op) { return op; };

    for (size_t i = 0; i < this->expression.size(); ++i) {
        char token = this->expression[i];

        if (isalpha(token)) {
            operands.push(string(1, token));
        }
        else if (token == '+' || token == '*') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                process(operands, operators, op_map);
            }
            operators.push(token);
        }
        else if (token == '(') {
            operators.push(token);
        }
        else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                process(operands, operators, op_map);
            }
            if (!operators.empty()) operators.pop(); // remove '('
        }
    }

    while (!operators.empty()) {
        process(operands, operators, op_map);
    }

    // return operands.top();
}

#endif
#ifndef EXPR_H
#define EXPR_H

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cctype>
#include <functional>

using namespace std;

class expr {
    private:
        string expression;
        
    protected:
        int precedence(char op);
        string apply_operator(const string& a, const string& b, char op);
        void process(stack<string>& operands, stack<char>& operators, function<char(char)> op_map);

    public:
        vector<string> steps;

        void evaluate_expression(bool invert = false);   // Can use string for return value

        friend istream& operator>>(istream& is, expr& l) {
            string value;
            if (is >> value) {
                l.expression = value;
            }
            return is;
        }

        friend ostream& operator<<(ostream& os, const expr& l) {
            for (const auto& s : l.steps) {
                os << "\"" << s << "\"" << " " << endl;
            }
            return os;
        }
};

#endif
#include "stack.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#ifndef __Arithmetic_H__
#define __Arithmetic_H__

const string inspect = "/*-+^()";

template <typename T> class Lexema {
public:
  string name;
  int priority;
  int type; // 1-скобки, 2-число, 3-функция, 4-оператор

  Lexema() : name(""), priority(-1), type(0) {}
  Lexema(string s, int t = 2, int p = -1) : name(s), type(t), priority(p) {}
};

class Function : public Lexema<string> {
  double (*function)(double);

public:
  Function(string s) : Lexema(s) {
    type = 3;
    name = s;
    if (s == "sin") {
      function = sin;
    } else if (s == "cos") {
      function = cos;
    } else if (s == "log") {
      function = log;
    }
  }
  double execute(double x) { return function(x); }
};

class Operation : public Lexema<string> {
  double (*operations)(double, double);

public:
  Operation() : Lexema<string>("", 4, 0), operations(nullptr) {}
  Operation(string s) : Lexema(s, 4, 0) {
    if (s == "+") {
      operations = [](double a, double b) { return a + b; };
      priority = 1;
    } else if (s == "-") {
      operations = [](double a, double b) { return a - b; };
      priority = 1;
    } else if (s == "*") {
      operations = [](double a, double b) { return a * b; };
      priority = 2;
    } else if (s == "/") {
      operations = [](double a, double b) { return a / b; };
      priority = 2;
    } else if (s == "^") {
      operations = pow;
      priority = 3;
    }
  }

  double execute(double a, double b) { return operations(a, b); }
};

bool isOperator(char c) { return inspect.find(c) != -1; }

bool isFunction(const string &str, size_t pos) {
  string functions[] = {"sin", "cos", "log"};
  for (const auto &func : functions) {
    if (str.substr(pos, func.length()) == func) {
      return true;
    }
  }
  return false;
}

inline TStack<string> toStack(const string &s) {
  auto precedence = [](const string &op) {
    if (op == "+" || op == "-")
      return 1;
    if (op == "*" || op == "/")
      return 2;
    if (op == "^")
      return 3;
    return -1;
  };
  auto isRightAssoc = [](const string &op) { return op == "^"; };

  TStack<string> output;
  TStack<string> ops;

  size_t i = 0;
  auto skip = [&](size_t &k) {
    while (k < s.size() && isspace((unsigned char)s[k]))
      k++;
  };
  bool expectingValue = true;

  while (true) {
    skip(i);
    if (i >= s.size())
      break;

    if (expectingValue) {
      if (s[i] == '+' || s[i] == '-') {
        size_t j = i + 1;
        skip(j);
        if (j < s.size() && (isdigit((unsigned char)s[j]) || s[j] == '.')) {

          i = j - 1;
        } else if (j < s.size() && s[j] == '(') {

          if (s[i] == '-') {
            output.push("0");
            ops.push("-");
          }
          i = j;
        } else {
          throw invalid_argument("Incorrect input");
        }
      }

      // число
      if (i < s.size() && (isdigit((unsigned char)s[i]) || s[i] == '.')) {
        string num;
        bool hasDot = false, hasDigit = false;
        while (i < s.size() && (isdigit((unsigned char)s[i]) || s[i] == '.')) {
          if (s[i] == '.') {
            if (hasDot)
              throw invalid_argument("Incorrect input");
            hasDot = true;
          } else {
            hasDigit = true;
          }
          num += s[i++];
        }
        if (!hasDigit)
          throw invalid_argument("Incorrect input");
        output.push(num);
        expectingValue = false;
        continue;
      }

      // функция
      if (i < s.size() && isalpha((unsigned char)s[i])) {
        if (!isFunction(s, i))
          throw invalid_argument("Incorrect input");
        string fname;
        while (i < s.size() && isalpha((unsigned char)s[i]))
          fname += s[i++];
        ops.push(fname);
        skip(i);
        if (i >= s.size() || s[i] != '(')
          throw invalid_argument("Incorrect input");
        ops.push("(");
        ++i;
        expectingValue = true;
        continue;
      }

      // открывающая скобка
      if (s[i] == '(') {
        ops.push("(");
        ++i;
        expectingValue = true;
        continue;
      }

      throw invalid_argument("Incorrect input");
    } else {
      // оператор
      if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' ||
          s[i] == '^') {
        string cur(1, s[i]);
        while (!ops.isEmpty()) {
          string top = ops.Top();
          if (top == "(")
            break;
          int pt = precedence(top), pc = precedence(cur);
          if (pt > pc || (pt == pc && !isRightAssoc(cur))) {
            output.push(ops.pop());
          } else
            break;
        }
        ops.push(cur);
        ++i;
        expectingValue = true;
        continue;
      }
      // закрывающая скобка
      if (s[i] == ')') {
        while (!ops.isEmpty() && ops.Top() != "(") {
          output.push(ops.pop());
        }
        if (ops.isEmpty())
          throw invalid_argument("Incorrect input");
        ops.pop(); // снять '('
        if (!ops.isEmpty() &&
            (ops.Top() == string("sin") || ops.Top() == string("cos") ||
             ops.Top() == string("log"))) {
          output.push(ops.pop());
        }
        ++i;
        expectingValue = false;
        continue;
      }
      throw invalid_argument("Incorrect input");
    }
  }

  while (!ops.isEmpty()) {
    if (ops.Top() == "(")
      throw invalid_argument("Incorrect input");
    output.push(ops.pop());
  }

  if (expectingValue)
    throw invalid_argument("Incorrect input");
  return output;
}

// Проверка, является ли строка корректным арифметическим выражением
inline bool isArithmeticExpression(const string &s) {
  auto isSpace = [](char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  };
  auto isOp = [](char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
  };

  size_t i = 0;
  int paren = 0;
  bool expectingValue = true;

  while (i < s.size()) {
    while (i < s.size() && isSpace(s[i]))
      i++;
    if (i == s.size())
      break;

    if (expectingValue) {
      size_t j = i;
      if (s[j] == '+' || s[j] == '-') {
        size_t k = j + 1;
        while (k < s.size() && isSpace(s[k]))
          k++;
        if (k < s.size() && (isdigit(s[k]) || s[k] == '.')) {
          j = k;
        } else {
          return false;
        }
      }

      if (j < s.size() && (isdigit(s[j]) || s[j] == '.')) {
        bool hasDigit = false;
        bool hasDot = false;
        while (j < s.size() && (isdigit(s[j]) || s[j] == '.')) {
          if (s[j] == '.') {
            if (hasDot)
              return false;
            hasDot = true;
          } else {
            hasDigit = true;
          }
          j++;
        }
        if (!hasDigit)
          return false;
        i = j;
        expectingValue = false;
        continue;
      }

      if (isalpha(s[i])) {
        if (!isFunction(s, i))
          return false;
        string names[] = {"sin", "cos", "log"};
        string matched = "";
        for (const auto &nm : names) {
          if (s.compare(i, nm.size(), nm) == 0) {
            matched = nm;
            break;
          }
        }
        if (matched.empty())
          return false;
        i += matched.size();
        while (i < s.size() && isSpace(s[i]))
          i++;
        if (i == s.size() || s[i] != '(')
          return false;
        paren++;
        i++;
        expectingValue = true;
        continue;
      }

      if (s[i] == '(') {
        paren++;
        i++;
        expectingValue = true;
        continue;
      }

      return false;
    } else {
      if (isOp(s[i])) {
        i++;
        expectingValue = true;
        continue;
      }
      if (s[i] == ')') {
        if (paren == 0)
          return false;
        paren--;
        i++;
        expectingValue = false;
        continue;
      }
      return false;
    }
  }

  return !expectingValue && paren == 0;
}
// Вычисление ОПН
inline double evaluateRPN(const vector<string> &rpn) {
  stack<double> st;
  auto isNum = [](const string &t) {
    return !t.empty() && (isdigit((unsigned char)t[0]) || t[0] == '.');
  };

  for (const auto &t : rpn) {
    if (isNum(t)) {
      st.push(stod(t));
    } else if (t == "+" || t == "-" || t == "*" || t == "/" || t == "^") {
      if (st.size() < 2)
        throw invalid_argument("Incorrect input");
      double b = st.top();
      st.pop();
      double a = st.top();
      st.pop();
      double v = 0.0;
      if (t == "+")
        v = a + b;
      else if (t == "-")
        v = a - b;
      else if (t == "*")
        v = a * b;
      else if (t == "/") {
        if (b == 0.0)
          throw invalid_argument("Incorrect input");
        v = a / b;
      } else if (t == "^")
        v = pow(a, b);
      st.push(v);
    } else if (t == "sin" || t == "cos" || t == "log") {
      if (st.empty())
        throw invalid_argument("Incorrect input");
      double x = st.top();
      st.pop();
      double v = 0.0;
      if (t == "sin")
        v = sin(x);
      else if (t == "cos")
        v = cos(x);
      else if (t == "log") {
        if (x <= 0.0)
          throw invalid_argument("Incorrect input");
        v = log(x);
      }
      st.push(v);
    } else {
      throw invalid_argument("Incorrect input");
    }
  }

  if (st.size() != 1)
    throw invalid_argument("Incorrect input");
  return st.top();
}

inline double evaluateExpression(const string &s) {
  if (!isArithmeticExpression(s))
    throw invalid_argument("Incorrect input");
  TStack<string> rpnStack = toStack(s);

  vector<string> rpn;
  while (!rpnStack.isEmpty())
    rpn.push_back(rpnStack.pop());
  reverse(rpn.begin(), rpn.end());

  return evaluateRPN(rpn);
}

#endif
#include "stack.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#ifndef __Arit_H__
#define __Arit_H__

const string inspect = "/*-+^()";

const string fInspect="cossinlogsqrt";

template <typename T> class Lexema {
public:
  string name;
  int priority;
  int type; // 1-скобки, 2-число, 3-функция, 4-оператор

  Lexema() : name(""), priority(-1), type(0) {}
  Lexema(string s) : name(s), type(1), priority(0) {} //bracket
  string GetName(){
    return name;
  }
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
    } else if (s == "sqrt") {
      function = sqrt;
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

#include "input.h"

int asciiToNumber(char ch) {
  if (ch >= '0' &&
      ch <= '9') { // символы по кодам аски "0" это 48 а "9" соотвественно 57
    return ch - '0'; // 48 - 48 =0 (получаем 0)  49-48=1 (единичка)
  }
  return -1;
}

int isValidChar(char ch) {
  const char availableChars[] =
      "+-*/sincoslogsqrt().0123456789x"; // набор всех доступных символов
  int is_valid = 0;
  for (int i = 0; availableChars[i] != '\0'; i++) {
    if (ch == availableChars[i]) {
      is_valid = 1;
    }
  }
  return is_valid;
}

int isFuncStart(const char *str, int index, const char **functions,
                int num_functions, int *func_len) {
  for (int i = 0; i < num_functions; i++) {
    int len = strlen(functions[i]);
    if (strncmp(&str[index], functions[i], len) == 0) {
      *func_len = len;
      return 1;
    } // проверяет наличие подстроки функции в выражении (ищет всякие синусы
      // косинусы и тд)
  }
  return 0;
}

int input(const char *str, size_t len) {
  const char *functions[] = {"sqrt", "log", "sin", "cos"};
  int is_error = 0, previous = 0, was_num = 0, decimal_used = 0;
  int parentheses_count = 0, last_was_operator = 0, last_was_func = 0;
  int has_x_or_func = 0;
  for (size_t i = 0; i < len && !is_error; i++) {
    char cur = str[i];
    int num = asciiToNumber(cur), func_len = 0;

    if (!isValidChar(cur) && cur != '\n' && cur != '\0')
      is_error = 1; // ловит ошибки

    if (last_was_operator && strchr("+-*/", cur))
      is_error = 1; // двойной оператор
    last_was_operator = strchr("+-*/", cur) != NULL;

    if (!was_num && num == 0 && i < len - 1 && asciiToNumber(str[i + 1]) != -1)
      is_error = 1; // ведущие нули

    if (cur == '.' &&
        (decimal_used ||
         (!was_num && (i == len - 1 || asciiToNumber(str[i + 1]) == -1))))
      is_error = 1;
    decimal_used |= (cur == '.'); // ловит неверный ввод нецелого числа

    if (isFuncStart(str, i, functions, 6, &func_len)) {
      if (last_was_func || (was_num && previous != '('))
        is_error = 1; // скобки и их баланс

      i += func_len - 1;
      if (i + 1 >= len || str[i + 1] != '(')
        is_error = 1;

      last_was_func = 1;
      has_x_or_func = 1;
    } else {
      if (last_was_func && (num != -1 || cur == '(')) {
        last_was_func = 0;
      }
      last_was_func = 0;
    }

    if (cur == '(') {
      parentheses_count++;
      last_was_operator = 0;
    } else if (cur == ')') {
      parentheses_count--;
      is_error |= (parentheses_count < 0 || previous == '(');
    }

    was_num = (num != -1 || cur == 'x');
    if (cur == 'x' || last_was_func)
      has_x_or_func = 1;
    if (strchr("+-*/(", cur))
      was_num = decimal_used = 0;

    if (!is_error)
      previous = cur;
  }

  if (parentheses_count || last_was_operator)
    is_error = 1;

  if (!has_x_or_func)
    is_error = 1; // проверяет наличие самой переменной

  return is_error;
}

double execute(string& s, double glx) {
    int i = 0;
    TStack<double> numbers;
    TStack<Lexema<string>*> res;  // Исправлен тип шаблона
    
    while (i < s.size()) {
        // Обработка скобок
        if (s[i] == ')' || s[i] == '(') {
            res.push(new Lexema<string>(string(1, s[i])));
            i++;
            continue;
        }
        
        // Обработка чисел
        if (i < s.size() && isdigit(s[i])) {
            string tmp = "";
            tmp += s[i];
            i++;
            while (i < s.size() && (isdigit(s[i]) || s[i] == '.')) {
                tmp += s[i];
                i++;
            }
            numbers.push(stod(tmp));
            continue;
        }
        
        // Обработка переменной x
        if (s[i] == 'x') {
            numbers.push(glx);
            i++;
            continue;
        }
        
        // Обработка операторов
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^') {
            res.push(new Operation(string(1, s[i])));
            i++;
            continue;
        }
        
        // Обработка функций
        if (s[i] == 's' || s[i] == 'l' || s[i] == 'c') {
            string tmp = "";
            // Проверяем возможные функции
            if (s.substr(i, 3) == "sin") {
                tmp = "sin";
                i += 3;
            } else if (s.substr(i, 3) == "cos") {
                tmp = "cos";
                i += 3;
            } else if (s.substr(i, 3) == "log") {
                tmp = "log";
                i += 3;
            } else if (s.substr(i, 4) == "sqrt") {
                tmp = "sqrt";
                i += 4;
            } else {
                i++; // Пропускаем неизвестный символ
            }
            if (!tmp.empty()) {
                res.push(new Function(tmp));
            }
            continue;
        }
        
        i++; // Переход к следующему символу, если не распознан
    }
    
    // Разворачиваем стеки для правильного порядка вычислений
    numbers = numbers.reverse();
    res = res.reverse();
    
    // Выполняем вычисления
    while (!res.isEmpty()) {
        Lexema<string>* current = res.pop();
        
        if (current->type == 3) { // Функция
            Function* func = dynamic_cast<Function*>(current);
            if (func && !numbers.isEmpty()) {
                double tmp = numbers.pop();
                numbers.push(func->execute(tmp));
            }
        } 
        else if (current->type == 4) { // Оператор
            Operation* op = dynamic_cast<Operation*>(current);
            if (op && numbers.get_size() >= 2) {
                double tmp2 = numbers.pop();
                double tmp1 = numbers.pop();
                numbers.push(op->execute(tmp1, tmp2));
            }
        }
        // Для чисел и скобок ничего не делаем в этой простой реализации
        
        delete current; // Освобождаем память
    }
    
    // Возвращаем результат
    if (!numbers.isEmpty()) {
        return numbers.pop();
    } else {
        throw runtime_error("No result calculated");
    }
}


// double Execute(string&s,double glx){
//     int i=0;
//     TStack<double> numbers;
//     TStack<Lexema<string<string>*> res;
//     while(i<s.size()){
//         if(i<s.size() || i==')' || i=='('){
//             res.push(Lexema(s[i]));
//         }
//         if(i<s.size() && isdigit(s[i])){
//             string tmp=s[i];
//             i++;
//             while(i<s.size() && isdigit(s[i]) || s[i]=='.'){
//                 tmp+=s[i];
//                 i++;
//             }
//             numbers.push(stod(tmp));
//         }
//         if(s[i]='x'){
//             numbers.push(glx);
//             i++;
//         }
//         if(s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='^'){
//             res.push(Operation(s[i]));
//             i++;
//         }
//         if(s[i]=='s' || s[i]=='l' || s[i]=='c'){
//             string tmp=s[i];
//             i++;
//             tmp+=s[i];
//             i++;
//             tmp+=s[i];
//             res.push(Function(tmp));
//             i++;
//         }
//     }  
//     numbers=numbers.reverse();
//     res=res.reverse();
//     while(!numbers.isEmpty() || !res.isEmpty()){
//         if(fInspect.find(res.top().GetName())!=-1){
//             double tmp=numbers.pop();
//             numbers.push(res.pop().execute(tmp));
//         }
//         if(inspect.find(res.top().GetName())!=-1){
//             double tmp1=numbers.pop();
//             double tmp2=numbers.pop();
//             numbers.push(res.pop().execute(tmp1,tmp2));
//         }
//     }
// }
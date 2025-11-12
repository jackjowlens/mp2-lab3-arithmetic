#include <gtest.h>
#include <cmath>
#include <map>
#include <stdexcept>
#include "art.h"

using namespace std;

// Тесты для функции input (валидация ввода)
TEST(InputValidationTest, ValidExpressions) {
    EXPECT_EQ(input("x+1", 3), 0);
    EXPECT_EQ(input("sin(x)", 6), 0);
    EXPECT_EQ(input("cos(x)*2", 8), 0);
    EXPECT_EQ(input("(x+1)*2", 7), 0);
    EXPECT_EQ(input("sqrt(x)", 7), 0);
    EXPECT_EQ(input("log(x+1)", 8), 0);
    EXPECT_EQ(input("x^2", 3), 0);
    EXPECT_EQ(input("1.5*x", 5), 0);
    EXPECT_EQ(input("x+y", 3), 0);
    EXPECT_EQ(input("x*y+z", 5), 0);
}

TEST(InputValidationTest, InvalidExpressions) {
    // Двойные операторы
    EXPECT_EQ(input("x++1", 4), 1);
    EXPECT_EQ(input("x--1", 4), 1);
    EXPECT_EQ(input("x**2", 4), 1);
    
    // Неправильные скобки
    EXPECT_EQ(input("(x+1", 4), 1);
    EXPECT_EQ(input("x+1)", 4), 1);
    EXPECT_EQ(input(")x+1(", 5), 1);
    
    // Ведущие нули
    EXPECT_EQ(input("x+01", 4), 1);
    
    // Неправильные функции
    EXPECT_EQ(input("sinx", 4), 1);
    EXPECT_EQ(input("sin1", 4), 1);
    
    // Недопустимые символы
    EXPECT_EQ(input("x#1", 3), 1);
    EXPECT_EQ(input("x@y", 3), 1);
}

// Тесты для функции execute с переменными
TEST(ExecuteTest, BasicOperations) {
    map<string, double> vars = {{"x", 0}, {"y", 0}, {"z", 0}};
    
    string expr = "2+3";
    EXPECT_NEAR(execute(expr, vars), 5.0, 0.001);
    
    expr = "5-2";
    EXPECT_NEAR(execute(expr, vars), 3.0, 0.001);
    
    expr = "3*4";
    EXPECT_NEAR(execute(expr, vars), 12.0, 0.001);
    
    expr = "8/2";
    EXPECT_NEAR(execute(expr, vars), 4.0, 0.001);
    
    expr = "2^3";
    EXPECT_NEAR(execute(expr, vars), 8.0, 0.001);
}

TEST(ExecuteTest, WithVariables) {
    map<string, double> vars = {{"x", 5.0}, {"y", 3.0}, {"z", 2.0}};
    
    string expr = "x+1";
    EXPECT_NEAR(execute(expr, vars), 6.0, 0.001);
    
    expr = "2*x";
    EXPECT_NEAR(execute(expr, vars), 10.0, 0.001);
    
    expr = "x^y";
    EXPECT_NEAR(execute(expr, vars), 125.0, 0.001);
    
    expr = "x+y+z";
    EXPECT_NEAR(execute(expr, vars), 10.0, 0.001);
    
    expr = "x*y-z";
    EXPECT_NEAR(execute(expr, vars), 13.0, 0.001);
}

TEST(ExecuteTest, FunctionsWithVariables) {
    map<string, double> vars = {{"x", 0}, {"y", M_PI/2}, {"z", 1.0}};
    
    string expr = "sin(0)";
    EXPECT_NEAR(execute(expr, vars), 0.0, 0.001);
    
    expr = "cos(0)";
    EXPECT_NEAR(execute(expr, vars), 1.0, 0.001);
    
    expr = "log(1)";
    EXPECT_NEAR(execute(expr, vars), 0.0, 0.001);
    
    expr = "sqrt(4)";
    EXPECT_NEAR(execute(expr, vars), 2.0, 0.001);
    
    expr = "sin(y)";
    EXPECT_NEAR(execute(expr, vars), 1.0, 0.001);
    
    expr = "log(z)";
    EXPECT_NEAR(execute(expr, vars), 0.0, 0.001);
}

TEST(ExecuteTest, ComplexExpressions) {
    map<string, double> vars = {{"x", 2.0}, {"y", 3.0}, {"z", 4.0}};
    
    string expr = "2+3*4";
    EXPECT_NEAR(execute(expr, vars), 14.0, 0.001);
    
    expr = "(2+3)*4";
    EXPECT_NEAR(execute(expr, vars), 20.0, 0.001);
    
    expr = "sin(x)+cos(x)";
    EXPECT_NEAR(execute(expr, vars), sin(2.0) + cos(2.0), 0.001);
    
    expr = "2*sin(x)";
    EXPECT_NEAR(execute(expr, vars), 2 * sin(2.0), 0.001);
    
    expr = "x*y+z";
    EXPECT_NEAR(execute(expr, vars), 10.0, 0.001);
}

TEST(ExecuteTest, DecimalNumbers) {
    map<string, double> vars = {{"x", 2.0}, {"y", 0.5}};
    
    string expr = "1.5+2.5";
    EXPECT_NEAR(execute(expr, vars), 4.0, 0.001);
    
    expr = "0.5*4";
    EXPECT_NEAR(execute(expr, vars), 2.0, 0.001);
    
    expr = "x*1.5";
    EXPECT_NEAR(execute(expr, vars), 3.0, 0.001);
    
    expr = "y*10";
    EXPECT_NEAR(execute(expr, vars), 5.0, 0.001);
}



TEST(ExecuteTest, OperatorPrecedence) {
    map<string, double> vars = {{"x", 2.0}, {"y", 3.0}, {"z", 4.0}};
    
    string expr = "2+3*4";
    EXPECT_NEAR(execute(expr, vars), 14.0, 0.001); // 2 + 12 = 14
    
    expr = "2*3+4";
    EXPECT_NEAR(execute(expr, vars), 10.0, 0.001); // 6 + 4 = 10
    
    expr = "2^3*4";
    EXPECT_NEAR(execute(expr, vars), 32.0, 0.001); // 8 * 4 = 32
    
    expr = "2*3^2";
    EXPECT_NEAR(execute(expr, vars), 18.0, 0.001); // 2 * 9 = 18
}

// Тесты для классов
TEST(ClassTest, LexemaCreation) {
    Lexema<string> bracket("(");
    EXPECT_EQ(bracket.type, 1);
    EXPECT_EQ(bracket.name, "(");
    EXPECT_EQ(bracket.GetName(), "(");
}

TEST(ClassTest, FunctionCreation) {
    Function sinFunc("sin");
    EXPECT_EQ(sinFunc.type, 3);
    EXPECT_EQ(sinFunc.name, "sin");
    EXPECT_NEAR(sinFunc.execute(M_PI/2), 1.0, 0.001);
    
    Function cosFunc("cos");
    EXPECT_NEAR(cosFunc.execute(0), 1.0, 0.001);
    
    Function logFunc("log");
    EXPECT_NEAR(logFunc.execute(1), 0.0, 0.001);
    
    Function sqrtFunc("sqrt");
    EXPECT_NEAR(sqrtFunc.execute(4), 2.0, 0.001);
}

TEST(ClassTest, OperationCreation) {
    Operation add("+");
    EXPECT_EQ(add.type, 4);
    EXPECT_EQ(add.priority, 1);
    EXPECT_NEAR(add.execute(2, 3), 5.0, 0.001);
    
    Operation multiply("*");
    EXPECT_EQ(multiply.priority, 2);
    EXPECT_NEAR(multiply.execute(4, 5), 20.0, 0.001);
    
    Operation power("^");
    EXPECT_EQ(power.priority, 3);
    EXPECT_NEAR(power.execute(2, 3), 8.0, 0.001);
    
    Operation divide("/");
    EXPECT_EQ(divide.priority, 2);
    EXPECT_NEAR(divide.execute(10, 2), 5.0, 0.001);
}

// Тесты для вспомогательных функций
TEST(UtilityTest, IsOperator) {
    EXPECT_TRUE(isOperator('+'));
    EXPECT_TRUE(isOperator('-'));
    EXPECT_TRUE(isOperator('*'));
    EXPECT_TRUE(isOperator('/'));
    EXPECT_TRUE(isOperator('^'));
    EXPECT_TRUE(isOperator('('));
    EXPECT_TRUE(isOperator(')'));
    EXPECT_FALSE(isOperator('x'));
    EXPECT_FALSE(isOperator('1'));
    EXPECT_FALSE(isOperator('.'));
}

TEST(UtilityTest, AsciiToNumber) {
    EXPECT_EQ(asciiToNumber('0'), 0);
    EXPECT_EQ(asciiToNumber('5'), 5);
    EXPECT_EQ(asciiToNumber('9'), 9);
    EXPECT_EQ(asciiToNumber('a'), -1);
    EXPECT_EQ(asciiToNumber('+'), -1);
}

TEST(UtilityTest, IsValidChar) {
    EXPECT_TRUE(isValidChar('+'));
    EXPECT_TRUE(isValidChar('x'));
    EXPECT_TRUE(isValidChar('y'));
    EXPECT_TRUE(isValidChar('z'));
    EXPECT_TRUE(isValidChar('1'));
    EXPECT_TRUE(isValidChar('.'));
    EXPECT_TRUE(isValidChar('('));
    EXPECT_TRUE(isValidChar(')'));
    EXPECT_FALSE(isValidChar('a')); // кроме x,y,z
    EXPECT_FALSE(isValidChar('#'));
    EXPECT_FALSE(isValidChar(' '));
}

// Тесты для приоритетов операторов
TEST(OperatorTest, Priorities) {
    Operation add("+");
    Operation multiply("*");
    Operation power("^");
    
    EXPECT_GT(multiply.priority, add.priority);
    EXPECT_GT(power.priority, multiply.priority);
    EXPECT_EQ(add.priority, 1);
    EXPECT_EQ(multiply.priority, 2);
    EXPECT_EQ(power.priority, 3);
}

// Тесты для сложных математических выражений
TEST(ComplexMathTest, TrigonometricCombinations) {
    map<string, double> vars = {{"x", M_PI/4}};
    
    string expr = "sin(x)^2+cos(x)^2";
    EXPECT_NEAR(execute(expr, vars), 1.0, 0.001);
    
    expr = "2*sin(x)*cos(x)";
    EXPECT_NEAR(execute(expr, vars), sin(M_PI/2), 0.001);
}

TEST(ComplexMathTest, NestedFunctions) {
    map<string, double> vars = {{"x", 1.0}};
    
    string expr = "sqrt(sin(x)^2+cos(x)^2)";
    EXPECT_NEAR(execute(expr, vars), 1.0, 0.001);
    
    expr = "log(exp(1))";
    EXPECT_NEAR(execute(expr, vars), 1.0, 0.001);
}


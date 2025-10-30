// ����� ��� ���������� �������������� ���������

#include <gtest.h>
#include "art.h"

TEST(InputValidationTest, ValidExpressions) {
    EXPECT_EQ(input("x+1", 3), 0);
    EXPECT_EQ(input("sin(x)", 6), 0);
    EXPECT_EQ(input("cos(x)*2", 8), 0);
    EXPECT_EQ(input("(x+1)*2", 7), 0);
    EXPECT_EQ(input("sqrt(x)", 7), 0);
    EXPECT_EQ(input("log(x+1)", 8), 0);
    EXPECT_EQ(input("x^2", 3), 0);
    EXPECT_EQ(input("1.5*x", 5), 0);
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
    
    // Неправильные десятичные числа
    EXPECT_EQ(input("x+.", 3), 1);
    EXPECT_EQ(input("x+1.", 4), 1);
    
    // Неправильные функции
    EXPECT_EQ(input("sinx", 4), 1);
    EXPECT_EQ(input("sin1", 4), 1);
    
    // Недопустимые символы
    EXPECT_EQ(input("x+y", 3), 1);
    EXPECT_EQ(input("x#1", 3), 1);
}

// Тесты для функции execute (вычисление выражений)
TEST(ExecuteTest, BasicOperations) {
    string expr;
    
    expr = "2+3";
    EXPECT_NEAR(execute(expr, 0), 5.0, 0.001);
    
    expr = "5-2";
    EXPECT_NEAR(execute(expr, 0), 3.0, 0.001);
    
    expr = "3*4";
    EXPECT_NEAR(execute(expr, 0), 12.0, 0.001);
    
    expr = "8/2";
    EXPECT_NEAR(execute(expr, 0), 4.0, 0.001);
    
    expr = "2^3";
    EXPECT_NEAR(execute(expr, 0), 8.0, 0.001);
}

TEST(ExecuteTest, WithVariableX) {
    string expr;
    
    expr = "x+1";
    EXPECT_NEAR(execute(expr, 5.0), 6.0, 0.001);
    
    expr = "2*x";
    EXPECT_NEAR(execute(expr, 3.0), 6.0, 0.001);
    
    expr = "x^2";
    EXPECT_NEAR(execute(expr, 4.0), 16.0, 0.001);
}

TEST(ExecuteTest, Functions) {
    string expr;
    
    expr = "sin(0)";
    EXPECT_NEAR(execute(expr, 0), 0.0, 0.001);
    
    expr = "cos(0)";
    EXPECT_NEAR(execute(expr, 0), 1.0, 0.001);
    
    expr = "log(1)";
    EXPECT_NEAR(execute(expr, 0), 0.0, 0.001);
    
    expr = "sqrt(4)";
    EXPECT_NEAR(execute(expr, 0), 2.0, 0.001);
    
    expr = "sin(x)";
    EXPECT_NEAR(execute(expr, M_PI/2), 1.0, 0.001);
}

TEST(ExecuteTest, ComplexExpressions) {
    string expr;
    
    expr = "2+3*4";
    EXPECT_NEAR(execute(expr, 0), 14.0, 0.001);
    
    expr = "(2+3)*4";
    EXPECT_NEAR(execute(expr, 0), 20.0, 0.001);
    
    expr = "sin(x)+cos(x)";
    EXPECT_NEAR(execute(expr, 0), 1.0, 0.001);
    
    expr = "2*sin(x)";
    EXPECT_NEAR(execute(expr, M_PI/6), 1.0, 0.001);
}

TEST(ExecuteTest, DecimalNumbers) {
    string expr;
    
    expr = "1.5+2.5";
    EXPECT_NEAR(execute(expr, 0), 4.0, 0.001);
    
    expr = "0.5*4";
    EXPECT_NEAR(execute(expr, 0), 2.0, 0.001);
    
    expr = "x*1.5";
    EXPECT_NEAR(execute(expr, 2.0), 3.0, 0.001);
}

TEST(ExecuteTest, EdgeCases) {
    string expr;
    
    // Деление на ноль
    expr = "1/0";
    EXPECT_THROW(execute(expr, 0), runtime_error);
    
    // Отрицательные числа под корнем
    expr = "sqrt(-1)";
    EXPECT_TRUE(isnan(execute(expr, 0)) || !isfinite(execute(expr, 0)));
    
    // Логарифм от неположительного числа
    expr = "log(0)";
    EXPECT_TRUE(isinf(execute(expr, 0)) || !isfinite(execute(expr, 0)));
}

TEST(ExecuteTest, OperatorPrecedence) {
    string expr;
    
    expr = "2+3*4";
    EXPECT_NEAR(execute(expr, 0), 14.0, 0.001); // 2 + 12 = 14
    
    expr = "2*3+4";
    EXPECT_NEAR(execute(expr, 0), 10.0, 0.001); // 6 + 4 = 10
    
    expr = "2^3*4";
    EXPECT_NEAR(execute(expr, 0), 32.0, 0.001); // 8 * 4 = 32
    
    expr = "2*3^2";
    EXPECT_NEAR(execute(expr, 0), 18.0, 0.001); // 2 * 9 = 18
}

// Тесты для классов Lexema, Function, Operation
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
    EXPECT_TRUE(isValidChar('1'));
    EXPECT_TRUE(isValidChar('.'));
    EXPECT_TRUE(isValidChar('('));
    EXPECT_TRUE(isValidChar(')'));
    EXPECT_FALSE(isValidChar('y'));
    EXPECT_FALSE(isValidChar('#'));
    EXPECT_FALSE(isValidChar(' '));
}
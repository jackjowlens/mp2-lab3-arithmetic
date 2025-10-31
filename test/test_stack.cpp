// ����� ��� �����

#include "stack.h"
#include <gtest.h>


TEST(TStackTest, ConstructorAndBasicOperations) {
    TStack<int> s1tack(5);
    EXPECT_TRUE(s1tack.isEmpty());
    EXPECT_EQ(s1tack.get_size(), 5);
}

// Тест добавления и извлечения элементов
TEST(TStackTest, PushAndPop) {
    TStack<int> s1tack(3);
    s1tack.push(10);
    s1tack.push(20);
    s1tack.push(30);
    
    EXPECT_FALSE(s1tack.isEmpty());
    EXPECT_EQ(s1tack.pop(), 30);
    EXPECT_EQ(s1tack.pop(), 20);
    EXPECT_EQ(s1tack.pop(), 10);
    EXPECT_TRUE(s1tack.isEmpty());
}

// Тест метода Top
TEST(TStackTest, TopMethod) {
    TStack<int> stack(3);
    stack.push(42);
    EXPECT_EQ(stack.Top(), 42);
    EXPECT_FALSE(stack.isEmpty()); // Top не должен удалять элемент
    
    stack.push(100);
    EXPECT_EQ(stack.Top(), 100);
    EXPECT_EQ(stack.pop(), 100); // После Top стек должен остаться неизменным
    EXPECT_EQ(stack.pop(), 42);
}

// Тест исключений при пустом стеке
TEST(TStackTest, EmptyStackExceptions) {
    TStack<int> stack(2);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_THROW(stack.pop(), invalid_argument);
    EXPECT_THROW(stack.Top(), invalid_argument);
}

// Тест автоматического расширения стека
TEST(TStackTest, DynamicResize) {
    TStack<int> stack(3);
    
    // Заполняем больше начального размера
    for (int i = 0; i < 10; ++i) {
        stack.push(i);
    }
    
    // Должен был произойти resize
    EXPECT_GE(stack.get_size(), 10);
    
    // Проверяем, что все элементы сохранились в правильном порядке
    for (int i = 9; i >= 0; --i) {
        EXPECT_EQ(stack.pop(), i);
    }
    EXPECT_TRUE(stack.isEmpty());
}

// Тест со строками
TEST(TStackTest, StringStack) {
    TStack<string> stack(2);
    stack.push("hello");
    stack.push("world");
    stack.push("!");
    
    EXPECT_EQ(stack.pop(), "!");
    EXPECT_EQ(stack.pop(), "world");
    EXPECT_EQ(stack.pop(), "hello");
    EXPECT_TRUE(stack.isEmpty());
}

// Тест метода reverse
TEST(TStackTest, ReverseMethod) {
    TStack<int> stack(5);
    stack.push(1);
    stack.push(2);
    stack.push(3);
    
    TStack<int> reversed = stack.reverse();
    
    // Проверяем порядок элементов в reversed стеке
    EXPECT_EQ(reversed.pop(), 1);
    EXPECT_EQ(reversed.pop(), 2);
    EXPECT_EQ(reversed.pop(), 3);
    EXPECT_TRUE(reversed.isEmpty());
}

// Тест последовательных операций
TEST(TStackTest, MixedOperations) {
    TStack<int> stack(2);
    
    // Push-Pop-Push sequence
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.pop(), 2);
    stack.push(3);
    EXPECT_EQ(stack.pop(), 3);
    EXPECT_EQ(stack.pop(), 1);
    EXPECT_TRUE(stack.isEmpty());
    
    // Повторное использование после очистки
    stack.push(100);
    EXPECT_EQ(stack.Top(), 100);
    EXPECT_EQ(stack.pop(), 100);
}

// Тест граничных условий
TEST(TStackTest, BoundaryConditions) {
    TStack<int> stack(1);
    
    // Один элемент
    stack.push(999);
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.Top(), 999);
    
    // Добавление второго элемента (должен вызвать resize)
    stack.push(888);
    EXPECT_EQ(stack.pop(), 888);
    EXPECT_EQ(stack.pop(), 999);
    EXPECT_TRUE(stack.isEmpty());
}

// Тест с пользовательским типом (структурой)
TEST(TStackTest, CustomType) {
    struct Point {
        int x, y;
        Point() : x(0), y(0) {}  // Добавлен default конструктор
        Point(int x, int y) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    TStack<Point> stack(2);
    stack.push(Point(1, 2));
    stack.push(Point(3, 4));
    
    Point p = stack.pop();
    EXPECT_EQ(p.x, 3);
    EXPECT_EQ(p.y, 4);
    
    p = stack.pop();
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 2);
}
#pragma once
#include "Node.h"

class Stack {
private:
    Node* top; // get top element
public:
    Stack() { top = nullptr; } // constructor
    ~Stack();                  // distructor
    void push(const std::string& value); // function for add elements in to stack
    std::string pop();                   // delete a top element from stack
    std::string watch() const;           // see a top element in stack
    bool isEmpty() const;                // check is stack empty
    Node* getTop() const { return top; } // return top element from stack
    int size() const;                    // get a size of stack
};
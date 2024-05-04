#include "Stack.h"

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

int Stack::size() const {
    int count = 0;
    Node* current = top;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void Stack::push(const std::string& value) {
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = top;
    top = newNode;
}

std::string Stack::pop() {
    if (isEmpty()) {
        return "0";
    }
    std::string value = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;
    return value;
}


std::string Stack::watch() const {
    if (isEmpty()) {
        return "0";
    }
    return top->data;
}

bool Stack::isEmpty() const {
    return top == nullptr;
}
#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string.h>

const int ERROR_VALUE = -1; // declaration for ERROR
const int NO_ERROR = 0;

struct Node {
    std::string data; // info
    Node* next;       // next pointer
};

// transformation int to string
static std::string int_to_string(int number) {
    if (number == 0) {
        return "0";
    }

    std::string result = "";
    bool is_negative = false;

    if (number < 0) {
        is_negative = true;
        number = -number;
    }

    while (number > 0) {
        int digit = number % 10;
        result = static_cast<char>(digit + '0') + result;
        number /= 10;
    }
    if (is_negative) {
        result = "-" + result;
    }

    return result;
}

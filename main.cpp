#include "Stack.h"

bool isOperator(char token) { 
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '(' || token == ')';
}

int priority(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == 'N' || op == 'I' || op == 'M')
        return 3;
    return 0;
}

int countOperandsInsideMINMAX(const std::string& read, int startIndex) {
    int depthLevel = 0;
    int operandCount = 0;

    for (int i = startIndex; i < read.size(); ++i) {
        if (read[i] == '(') {
            depthLevel++;
        }
        else if (read[i] == ')') {
            depthLevel--;
            if (depthLevel == 0) {
                return operandCount + 1;
            }
        }
        else if (read[i] == ',' && depthLevel == 1) {
            operandCount++;
        }
    }
    return operandCount + 1;
}

// transformation a given string to postfxi form
std::string infixTransform(const std::string& read) {
    Stack operatorStack;
    Stack countOfOperandsStack;
    std::string postfix = "";
    std::string MAX = "MAX";
    std::string MIN = "MIN";
    bool IF_Operation = false;
    bool MAX_Operation = false;
    bool MIN_Operation = false;
    int operandCount = 0;
    for (int i = 0; i < read.size(); ++i) {
        if (read[i] == ' ') continue;
        if (isdigit(read[i])) { // if it's a number
            std::string operand = "";
            while (isdigit(read[i])) {
                operand += read[i];
                i++;
            }
            postfix += operand + " "; // add all numbers to postfix form
        }
        else if (read[i] == '(') {
            operatorStack.push("(");
        }
        else if (read[i] == ')') {
            while (!operatorStack.isEmpty() && operatorStack.watch() != "(") { // if stack isn't empty and a top element not equals "(" 
                std::string transformateOperandCount = countOfOperandsStack.watch();
                // to check a posible MAX or MIN behind brackets
                if (operatorStack.watch() == MAX + transformateOperandCount || operatorStack.watch() == MIN + transformateOperandCount) {
                    postfix += operatorStack.pop() + " ";
                    transformateOperandCount = countOfOperandsStack.pop();
                }
                //if not - just add all operators to postfix form
                else postfix += operatorStack.pop() + " ";
            }
            operatorStack.pop();
        }
        else if (isOperator(read[i])) { // check for operator
            // compare a priority of a operator which is reading and current top operator from stack
            while (!operatorStack.isEmpty() && priority(read[i]) <= priority(operatorStack.watch()[0])) {
                std::string transformateOperandCount = countOfOperandsStack.watch();
                if (operatorStack.watch() == MAX + transformateOperandCount || operatorStack.watch() == MIN + transformateOperandCount) {
                    postfix += operatorStack.pop() + " ";
                    transformateOperandCount = countOfOperandsStack.pop();
                }
                else postfix += operatorStack.pop() + " ";
            }
            operatorStack.push(std::string(1, read[i]));
        }
        else if (read[i] == 'I' && (i + 2 < read.size()) && (read[i + 1] == 'F')) { // IF operation
            operatorStack.push("IF");
            IF_Operation = true;
        }
        else if (read[i] == 'N') { // negation operation
            operatorStack.push(std::string(1, read[i]));
        }
        else if ((read[i] == 'M') && (i + 3 < read.size()) && (read[i + 1] == 'I') && (read[i + 2] == 'N')) { // MIN operation
            MAX_Operation = false;
            MIN_Operation = true;
            i += 2; // skip 2 elements after M
            operandCount = countOperandsInsideMINMAX(read, i); // count operands inside current MIN operation
            countOfOperandsStack.push(int_to_string(operandCount)); // add count of operands to new stack which contain only count of operands for MIN MAX operations
            operatorStack.push(MIN + int_to_string(operandCount)); // add to main stack with operators
        }
        else if ((read[i] == 'M') && (i + 3 < read.size()) && (read[i + 1] == 'A') && (read[i + 2] == 'X')) {
            MAX_Operation = true;
            MIN_Operation = false;
            i += 2;
            operandCount = countOperandsInsideMINMAX(read, i);
            countOfOperandsStack.push(int_to_string(operandCount));
            operatorStack.push(MAX + int_to_string(operandCount));
        }
        else if (read[i] == ',' && (IF_Operation || MIN_Operation || MAX_Operation)) { // definition for IF MAX MIN operations with check for ',' symbol in string
            std::string transformateOperandCount = countOfOperandsStack.watch();
            // the same situation to print MAX MIN in a correct place in postfix form
            while (!operatorStack.isEmpty() && (operatorStack.watch() == MAX + transformateOperandCount || operatorStack.watch() == MIN + transformateOperandCount)) {
                postfix += operatorStack.pop() + " ";
                transformateOperandCount = countOfOperandsStack.pop();
            }
            // fix my personal problem for IF operation with incorrect placing IF in postfix form when IF is placing with another operator or operation
            while (!operatorStack.isEmpty() && (operatorStack.watch() == "N" || operatorStack.watch() == "*" || operatorStack.watch() == "/" || operatorStack.watch() == "+" || operatorStack.watch() == "-" || operatorStack.watch() == "IF")) {
                postfix += operatorStack.pop() + " ";
            }
        }
    }
    while (!operatorStack.isEmpty()) {
        postfix += operatorStack.pop() + " ";
    }
    return postfix;
}

// function to getting operands from stack which haven't been used yet.
void addNotUsedOperandsFromStack(Node* current) {
    while (current != nullptr) {
        printf("%s ", current->data.c_str());
        current = current->next;
    }
}

// function to count MAX or MIN operation and placing in correct place in console 
void processMinMax(const std::string& read, Stack& operandStack, Stack& countStackForMINMAX, int& i, bool dontPrintResult, bool isMax) {
    std::string count;
    // get a number to print a count of operands inside current MIN or MAX
    while (isdigit(read[i + 3])) {
        count += read[i + 3];
        i++;
    }
    countStackForMINMAX.push(count);
    int maxOperandCount = atoi(countStackForMINMAX.pop().c_str());

    Stack tempStack; // stack to keep operands inside MIN MAX
    for (int j = 0; j < maxOperandCount; j++) {
        tempStack.push(operandStack.pop()); // add all operands to temp stack
    }

    int result = isMax ? INT_MIN : INT_MAX; // is it MIN or MAX operation
    Node* current = operandStack.getTop();
    if (!dontPrintResult) {
        printf("%s%d ", (isMax ? "MAX" : "MIN"), maxOperandCount); // print MAX or MIN and count of operands inside
        Stack reversedOperandStack; // stack to revers the position of the operands to the other way
        while (!tempStack.isEmpty()) {
            int operand = atoi(tempStack.watch().c_str()); // get a top operand
            reversedOperandStack.push(int_to_string(operand)); // add to reversed stack
            if ((isMax && operand > result) || (!isMax && operand < result)) {
                result = operand;
            }
            tempStack.pop();
        }
        while (!reversedOperandStack.isEmpty()) {
            printf("%s ", reversedOperandStack.pop().c_str());
        }
        addNotUsedOperandsFromStack(current);
    }
    printf("\n");
    operandStack.push(int_to_string(result));
}

// calculating a postfix form which we get from function "infixTransform"
int calculatePostfix(const std::string& read, int& final_result) {
    Stack operandStack;
    Stack resultStack;
    Stack countStackForMINMAX;
    bool dontPrintResult = false;
    std::string countOperands;
    for (int i = 0; i < read.size(); ++i) {
        if (read[i] == ' ') continue;

        if (isdigit(read[i])) {
            if (i > 1 && (read[i - 1] == 'X' || read[i - 1] == 'N')) { // to delete number which tell us about count of operands inside MINMAX from stack
                while (isdigit(read[i])) {
                    countOperands += read[i];
                    i++;
                }
                countStackForMINMAX.pop();
            }
            else {
                std::string operand = "";
                while (isdigit(read[i])) {
                    operand += read[i];
                    i++;
                }
                i--;
                operandStack.push(operand);
            }
        }
        else if (read[i] == 'N' && (read[i + 1] == ' ')) {
            std::string lastOperand = operandStack.pop();
            int value = atoi(lastOperand.c_str());
            Node* current = operandStack.getTop();
            value = -value;
            std::string secondOperand = operandStack.watch();
            operandStack.push(int_to_string(value));
            if (!dontPrintResult) {
                printf("N %s ", lastOperand.c_str());
                addNotUsedOperandsFromStack(current);
                printf("\n");
            }
        }
        else if (read[i] == 'I' && (i + 2 < read.size()) && (read[i + 1] == 'F')) {
            std::string falseValue = operandStack.pop();
            std::string trueValue = operandStack.pop();
            std::string condition = operandStack.pop();
            Node* current = operandStack.getTop();
            int value = atoi(condition.c_str()) > 0 ? atoi(trueValue.c_str()) : atoi(falseValue.c_str());
            operandStack.push(int_to_string(value));
            if (!dontPrintResult) {
                printf("IF %s %s %s ", falseValue.c_str(), trueValue.c_str(), condition.c_str());
                addNotUsedOperandsFromStack(current);
                printf("\n");
            }
        }
        else if ((read[i] == 'M') && (read[i + 1] == 'A') && (read[i + 2] == 'X')) {
            processMinMax(read, operandStack, countStackForMINMAX, i, dontPrintResult, true);
        }
        else if ((read[i] == 'M') && (read[i + 1] == 'I') && (read[i + 2] == 'N')) {
            processMinMax(read, operandStack, countStackForMINMAX, i, dontPrintResult, false);
        }
        else if (isOperator(read[i])) {
                std::string operation;
                int result = 0;
                int operand2 = atoi(operandStack.pop().c_str());
                int operand1 = atoi(operandStack.pop().c_str());
                Node* current = operandStack.getTop();
                switch (read[i]) {
                case '+':
                    result = operand1 + operand2;
                    operation = "+";
                    break;
                case '-':
                    result = operand1 - operand2;
                    operation = "-";
                    break;
                case '*':
                    result = operand1 * operand2;
                    operation = "*";
                    break;
                case '/':
                    if (operand2 == 0 && !dontPrintResult) {
                        operation = "/";
                        if (operandStack.size() > 0) {
                            resultStack.push(operation + " " + int_to_string(operand2) + " " + int_to_string(operand1));
                            while (!resultStack.isEmpty()) {
                                printf("%s ", resultStack.pop().c_str());
                            }
                            addNotUsedOperandsFromStack(current);
                            printf("\n");
                        }
                        else {
                            resultStack.push(operation + " " + int_to_string(operand2) + " " + int_to_string(operand1));
                            while (!resultStack.isEmpty()) {
                                printf("%s\n", resultStack.pop().c_str());
                            }
                        }
                        dontPrintResult = true;
                        break;
                    }
                    else if (operand2 == 0 && dontPrintResult) {
                        dontPrintResult = true;
                    }
                    else {
                        result = operand1 / operand2;
                        operation = "/";
                        break;
                    }
                default:
                    break;
                }
                if (!dontPrintResult) {
                    operandStack.push(int_to_string(result));
                    resultStack.push(operation + " " + int_to_string(operand2) + " " + int_to_string(operand1));
                    while (!resultStack.isEmpty()) {
                        printf("%s ", resultStack.pop().c_str());
                    }
                    addNotUsedOperandsFromStack(current);
                    printf("\n");
                }
            }
        }
    if (dontPrintResult) {
        return ERROR_VALUE;
    }
    final_result = atoi(operandStack.pop().c_str());
    return NO_ERROR;
}

int main() {
    int n;
    scanf("%d",&n);

    for (int i = 0; i < n; ++i) {
        std::string read;
        int ch;
        while ((ch = getchar()) != '.') {
            read += static_cast<char>(ch);
        }
        std::string postfix = infixTransform(read);
        printf("%s\n", postfix.c_str());
        int result;
        int status = calculatePostfix(postfix, result);
        if (status == ERROR_VALUE)
            printf("ERROR\n\n");
        else
            printf("%d\n\n", result);
    }
    return 0;
}
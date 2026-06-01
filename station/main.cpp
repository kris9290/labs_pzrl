#include <iostream>
#include <stack>
#include <string>
#include <sstream>

// Проверяет, является ли строка целым числом (в том числе отрицательным)
bool isNumericToken(const std::string& token) {
    if (token.length() == 0)
	    return false;
    size_t i = 0;
    if (token[0] == '-') {
        if (token.length() == 1) 
		return false;
        ++i;
    }
    for (; i < token.length(); i++) {
        if (!std::isdigit(token[i]))
	       	return false;
    }
    return true;
}

// Проверяет, является ли строка бинарным оператором (+, -, *, /, %)
bool isBinaryOperator(const std::string& token) {
    return (token == "+") || (token == "-") || (token == "*") || (token == "/") || (token == "%");
}

// Возвращает приоритет оператора: для + и - приоритет 0, для *, /, % — 1
int getOperatorPrecedence(const std::string& op) {
    if (op == "+" || op == "-")
	    return 0;
    return 1;
}

int main() {
    std::string inputLine;
    std::cout<<"Введите выражение:\n";
    std::getline(std::cin, inputLine);
    std::istringstream tokenStream(inputLine);

    std::stack<std::string> operatorStack;   // стек для операторов и скобок
    std::string currentToken;

    while (tokenStream >> currentToken) {
        // Если токен — число, отправляем его сразу на выход
        if (isNumericToken(currentToken)) {
            std::cout << currentToken << ' ';
            continue;
        }

        // Если токен — оператор
        if (isBinaryOperator(currentToken)) {
            // Пока на стеке есть оператор с бoльшим или равным приоритетом, выталкиваем его
            while (!operatorStack.empty() && isBinaryOperator(operatorStack.top()) &&
                   getOperatorPrecedence(operatorStack.top()) >= getOperatorPrecedence(currentToken)) {
                std::cout << operatorStack.top() << ' ';
                operatorStack.pop();
            }
            operatorStack.push(currentToken);
            continue;
        }

        // Если токен — открывающая скобка, кладём её в стек
        if (currentToken == "(") {
            operatorStack.push(currentToken);
            continue;
        }

        // Если токен — закрывающая скобка, выталкиваем все операторы до соответствующей "("
        if (currentToken == ")") {
            if (operatorStack.empty()) {
                throw std::runtime_error("Пропущена скобка");
            }
            while (operatorStack.top() != "(") {
                std::cout << operatorStack.top() << ' ';
                operatorStack.pop();
                if (operatorStack.empty()) {
                    throw std::runtime_error("Пропущена скобка");
                }
            }
            operatorStack.pop();   // удаляем саму "("
            continue;
        }

        // Если ни одно из условий не подошло — неизвестный токен
        throw std::runtime_error("Неизвестный токен");
    }

    // После обработки всех токенов выталкиваем оставшиеся операторы из стека
    while (!operatorStack.empty()) {
        if (operatorStack.top() == "(") {
            throw std::runtime_error("Пропущена скобка");
        }
        std::cout << operatorStack.top() << ' ';
        operatorStack.pop();
    }

    std::cout << std::endl;
}

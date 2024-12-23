#include "header.h"
long long check(const std::string& x) {
    long long y;
    std::cout << x;
    while (true) {
        std::cin >> y;
        if (std::cin.fail() || std::cin.peek() != '\n') {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Пожалуйста, следуйте инструкции и введите число и только число!" << "\n";
        }
        else {
            break;
        }
    }
    return y;
}
int input() {
    long long x;
    x = check("Введите: 1 - продолжить программу, 2 - завершить программу: ");
    if (x == 1) {
        return 1;
    }
    else if (x == 2) {
        exit(0);
    }
    else {
        std::cout << "Неверный ввод, давайте попробуем еще раз!\n";
        return input();
    }
}
std::string checkkk(const std::string& x) {
    std::string y;
    std::cout << x;
    while (true) {
        std::getline(std::cin, y);
        bool a = true;
        for (char c : y) {
            if (isdigit(c)) {
                a = false;
                break;
            }
        }
        if (a) {
            break; 
        } else {
            std::cout << "Некорректный ввод!\n";
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return y;
}
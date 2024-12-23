#include <iostream>
#include "header.h"
int main() {
    const char* filename = "participants.dat";
    int choice;
    while (true) {
        displayMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                displayFile(filename);
                break;
            case 2:
                clearElasticsearchIndex("participants");
                addParticipantToFile(filename);
                break;
            case 3: {
                int num;
                std::cout << "Введите номер для поиска: ";
                std::cin.ignore();
                std::cin >> num;
                searchPlayerByNumber(num);
                break;
            }
            case 4:
                deleteParticipantFromFile(filename);
                break;
            case 5:
                modifyParticipantInFile(filename);
                break;
            case 6:
                sortAndDisplayParticipantsByAgeDescending(filename);
                break;
            case 7:
                createDynamicArray(filename);
                break;
            case 0:
                return 0;
            default:
                std::cout << "Неверный выбор.\n";
        }
    }
}
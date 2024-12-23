#include "header.h"
void displayMenu() {
    std::cout << "Меню:\n"
              << "1. Показать участников\n"
              << "2. Добавить участника\n"
              << "3. Найти участников по номеру\n"
              << "4. Удалить участника\n"
              << "5. Изменить участника\n"
              << "6. Отсортировать участников по возрасту\n"
              << "7. Создать динамический массив участников\n"
              << "0. Выйти\n"
              << "Введите ваш выбор: ";
}
    void sortAndDisplayParticipantsByAgeDescending(const char* filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            return;
        }

        Participant participants[MAX_PARTICIPANTS];
        int count = 0;
        while (inFile.read(reinterpret_cast<char*>(&participants[count]), sizeof(Participant)) && count < MAX_PARTICIPANTS) {
            count++;
        }
        inFile.close();
        for (int gap = count / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < count; i++) {
                Participant temp = participants[i];
                int j;
                for (j = i; j >= gap && participants[j - gap].age < temp.age; j -= gap) {
                    participants[j] = participants[j - gap];
                }
                participants[j] = temp;
            }
        }
        std::cout << "Фамилии спортсменов с возрастом больше 20 лет (по убыванию возраста):\n";
        for (int i = 0; i < count; ++i) {
            if (participants[i].age > 20) {
                std::string fullName = participants[i].name;
                size_t spacePos = fullName.find(' ');
                std::string lastName = (spacePos != std::string::npos) ? fullName.substr(0, spacePos) : fullName;
                std::cout << lastName << " (Возраст: " << participants[i].age << ")\n";
            }
        }
    }


void createDynamicArray(const char* filename) {
    static Participant* participants = nullptr; 
    static int currentSize = 0; 
    static int allocatedSize = 0; 

    char stopCondition[50] = "";
    int choice;

    std::cout << "Выберите режим ввода:\n"
              << "1. Ввод заранее заданного количества структур\n"
              << "2. Ввод до появления структуры с заданным признаком\n"
              << "Ваш выбор: ";
    std::cin >> choice;

    if (choice == 2) {
        std::cout << "Введите критерий остановки (например, название страны): ";
        std::cin.ignore();
        std::cin.getline(stopCondition, sizeof(stopCondition));
    }

    std::ofstream outFile(filename, std::ios::binary | std::ios::app);
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи.\n";
        return;
    }

    int n = 0;
    if (choice == 1) {
        std::cout << "Введите количество структур: ";
        std::cin >> n;
    } else {
        n = MAX_PARTICIPANTS; 
    }
    if (allocatedSize < currentSize + n) {
        allocatedSize = currentSize + n;
        participants = (Participant*)realloc(participants, allocatedSize * sizeof(Participant));
        if (participants == nullptr) {
            std::cerr << "Ошибка выделения памяти.\n";
            outFile.close();
            return;
        }
    }

    for (int i = currentSize; i < currentSize + n; ++i) {
        std::cout << "Введите данные для участника " << i + 1 << ":\n";

        std::cout << "Страна: ";
        std::cin >> participants[i].country;

        if (strcmp(participants[i].country, "Russia") == 0) {
            strcpy(participants[i].team, "National Team");
        } else {
            std::cout << "Команда: ";
            std::cin >> participants[i].team;
        }

        std::cout << "Имя: ";
        std::cin >> participants[i].name;

        std::cout << "Номер: ";
        std::cin >> participants[i].number;

        std::cout << "Возраст: ";
        std::cin >> participants[i].age;

        std::cout << "Рост: ";
        std::cin >> participants[i].attributes.height;

        outFile.write(reinterpret_cast<const char*>(&participants[i]), sizeof(Participant));

        if (choice == 2 && strcmp(participants[i].country, stopCondition) == 0) {
            std::cout << "Остановлено: введен участник с заданным признаком.\n";
            n = i - currentSize + 1; 
            break;
        }
    }

    currentSize += n; 

    std::cout << "Динамический массив участников:\n";
    for (int i = 0; i < currentSize; ++i) {
        std::cout << "Страна: " << participants[i].country
                  << ", Команда: " << participants[i].team
                  << ", Имя: " << participants[i].name
                  << ", Номер: " << participants[i].number
                  << ", Возраст: " << participants[i].age
                  << ", Рост: " << participants[i].attributes.height << '\n';
    }
    outFile.close();
    uploadToElasticsearch(filename);
    free(participants);
}
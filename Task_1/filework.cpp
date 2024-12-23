#include "header.h"
void createFile(const char* filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Не удалось создать файл.\n";
        return;
    }
    std::cout << "Файл успешно создан.\n";
}

void displayFile(const char* filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        return;
    }

    Participant participant;
    while (inFile.read(reinterpret_cast<char*>(&participant), sizeof(Participant))) {
        std::cout << "Страна: " << participant.country
                  << ", Команда: " << participant.team
                  << ", Имя: " << participant.name
                  << ", Номер: " << participant.number
                  << ", Возраст: " << participant.age
                  << ", Рост: " << participant.attributes.height << '\n';
    }
    inFile.close();
}

void addParticipantToFile(const char* filename) {
    std::ofstream outFile(filename, std::ios::binary | std::ios::app);
    if (!outFile) {
        return;
    }

    Participant participant;
    std::cout << "Введите данные для нового участника:\n";
    std::cout << "Страна: ";
    std::cin >> participant.country;
    std::cout << "Команда: ";
    std::cin >> participant.team;
    std::cout << "Имя: ";
    std::cin >> participant.name;
    std::cout << "Номер: ";
    std::cin >> participant.number;
    std::cout << "Возраст: ";
    std::cin >> participant.age;
    std::cout << "Рост: ";
    std::cin >> participant.attributes.height;

    outFile.write(reinterpret_cast<const char*>(&participant), sizeof(Participant));
    outFile.close();

    std::cout << "Участник успешно добавлен.\n";
    uploadToElasticsearch(filename); 
}

void deleteParticipantFromFile(const char* filename) {
    int number;
    std::cout << "Введите номер участника для удаления: ";
    std::cin >> number;

    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        return;
    }

    std::ofstream tempFile("temp.dat", std::ios::binary);
    Participant participant;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&participant), sizeof(Participant))) {
        if (participant.number == number) {
            found = true;
        } else {
            tempFile.write(reinterpret_cast<const char*>(&participant), sizeof(Participant));
        }
    }

    inFile.close();
    tempFile.close();

    if (found) {
        std::remove(filename);
        std::rename("temp.dat", filename);
        deleteFromElasticsearch(number); 
        std::cout << "Участник успешно удалён.\n";
    } else {
        std::cout << "Участник с таким номером не найден.\n";
    }
}

void modifyParticipantInFile(const char* filename) {
    int number;
    std::cout << "Введите номер участника для изменения: ";
    std::cin >> number;

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        return;
    }

    Participant participant;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&participant), sizeof(Participant))) {
        if (participant.number == number) {
            found = true;
            deleteFromElasticsearch(participant.number);

            std::cout << "Введите новые данные для участника:\n";
            std::cout << "Страна: ";
            std::cin >> participant.country;
            std::cout << "Команда: ";
            std::cin >> participant.team;
            std::cout << "Имя: ";
            std::cin >> participant.name;
            std::cout << "Номер: ";
            std::cin >> participant.number;
            std::cout << "Возраст: ";
            std::cin >> participant.age;
            std::cout << "Рост: ";
            std::cin >> participant.attributes.height;

            file.seekp(-sizeof(Participant), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&participant), sizeof(Participant));
            std::cout << "Участник успешно изменён.\n";
            nlohmann::json jsonParticipant = {
                {"country", participant.country},
                {"team", participant.team},
                {"name", participant.name},
                {"number", participant.number},
                {"age", participant.age},
                {"height", participant.attributes.height}
            };
            std::string url = "http://localhost:9200/participants/_doc";
            sendPostRequest(url, jsonParticipant.dump());

            break;
        }
    }

    if (!found) {
        std::cout << "Участник с таким номером не найден.\n";
    }

    file.close();
}
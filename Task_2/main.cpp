#include "header.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

struct Employee {
    std::string fullName;  
    long long department;       
    std::string position; 
    std::string startDate; 
};

unsigned long long WriteCallback(void* contents, unsigned long long size, unsigned long long nmemb, std::string* userp) {
    unsigned long long totalSize = size * nmemb;
    (*userp).append((char*)contents, totalSize);
    return totalSize;
}

std::string sendPostRequest(const std::string& url, const std::string& jsonData) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        std::string auth = "elastic:iQkD4DMqRy_Qx2TdvVwO"; 
        curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cout << "Ошибка: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}
int getExperienceInDays(const Employee* emp) {
        struct tm tm_start = {};
        if (strptime(emp->startDate.c_str(), "%Y-%m-%d", &tm_start) == nullptr) {
            return 0;   
        }
        time_t start_time = mktime(&tm_start);
        time_t current_time = time(nullptr);
        double seconds = difftime(current_time, start_time);
        return seconds / (60 * 60 * 24); 
    }
void uploadToElasticsearch(const std::vector<Employee>& employees) {
    const std::string esUrl = "http://localhost:9200/employees/_doc";

    for (const auto& emp : employees) {
        nlohmann::json employeeJson = {
            {"full_name", emp.fullName},
            {"department", emp.department},
            {"position", emp.position},
            {"start_date", emp.startDate},
            {"experience_in_days", getExperienceInDays(&emp)}
        };
        sendPostRequest(esUrl, employeeJson.dump());
    }
}
void clearElasticsearchIndex(const std::string& indexName) {
    const std::string esUrl = "http://localhost:9200/" + indexName + "/_delete_by_query";
    std::string query = R"({
        "query": {
            "match_all": {}
        }
    })";
    sendPostRequest(esUrl, query);
}

void deleteFromElasticsearch(const std::string& fullName) {
    const std::string esUrl = "http://localhost:9200/employees/_delete_by_query";

    nlohmann::json queryJson = {
        {"query", {
            {"match", {{"full_name", fullName}}}
        }}  
    };

    sendPostRequest(esUrl, queryJson.dump());
}
void loadEmployeesFromFile(const std::string& filename, std::vector<Employee>& employees) {
    employees.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string fullName, position, startDate;
        long long department;

        if (iss >> std::quoted(fullName) >> department >> std::quoted(position) >> startDate) {
            employees.emplace_back();  // Создаем пустой объект
            employees.back().fullName = fullName;  // Инициализируем поля
            employees.back().department = department;
            employees.back().position = position;
            employees.back().startDate = startDate;
        }
    }
    file.close();
}
void saveEmployeesToFile(const std::string& filename, const std::vector<Employee>& employees) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        return;
    }

    for (const auto& emp : employees) {
        file << std::quoted(emp.fullName) << " "
             << emp.department << " "
             << std::quoted(emp.position) << " "
             << emp.startDate << "\n";
    }
    file.close();
}
void editEmployeeRecord(std::vector<Employee>& employees) {
    if (employees.empty()) {
        std::cout << "Список сотрудников пуст.\n";
        return;
    }
    std::string fullName = checkkk("Введите ФИО сотрудника для редактирования: ");

    auto it = std::find_if(employees.begin(), employees.end(), [&fullName](const Employee& emp) {
        return emp.fullName == fullName;
    });

    if (it == employees.end()) {
        std::cout << "Сотрудник с ФИО \"" << fullName << "\" не найден.\n";
        return;
    }

    std::cout << "Введите новые данные (оставьте поле пустым для сохранения текущего значения):\n";

    std::string newStartDate;
    long long newDepartment;

    std::string newFullName = checkkk("Новое ФИО: ");
    if (!newFullName.empty()) {
        deleteFromElasticsearch((*it).fullName);
        (*it).fullName = newFullName;
    }
    std::string departmentInput = checkkk("Новый номер отдела: ");
    if (!departmentInput.empty()) {
        newDepartment = std::stoll(departmentInput);
        (*it).department = newDepartment;
    }
    std::string newPosition = checkkk("Новая должность: ");
    if (!newPosition.empty()) {
        (*it).position = newPosition;
    }
    std::cout << "Новая дата начала работы (YYYY-MM-DD): ";
    std::getline(std::cin, newStartDate);
    if (!newStartDate.empty()) {
        (*it).startDate = newStartDate;
    }

    uploadToElasticsearch(employees); 
}
void deleteEmployeeRecord(std::vector<Employee>& employees) {
    if (employees.empty()) {
        std::cout << "Список сотрудников пуст.\n";
        return;
    }

    std::string fullName = checkkk("Введите ФИО сотрудника для удаления: ");
    auto it = std::find_if(employees.begin(), employees.end(), [&fullName](const Employee& emp) {
        return emp.fullName == fullName;
    });

    if (it == employees.end()) {
        std::cout << "Сотрудник с ФИО \"" << fullName << "\" не найден.\n";
        return;
    }

    deleteFromElasticsearch((*it).fullName); 
    employees.erase(it);                  
    std::cout << "Запись удалена.\n";
}
void searchEmployeeByFullName(const std::string& fullName) {
    const std::string esUrl = "http://localhost:9200/employees/_search";

    nlohmann::json queryJson = {
        {"query", {
            {"match", {
                {"full_name", fullName}
            }}
        }}
    };
    std::string response = sendPostRequest(esUrl, queryJson.dump());
    nlohmann::json jsonResponse = nlohmann::json::parse(response);
    if (jsonResponse.contains("hits") && jsonResponse["hits"].contains("hits")) {
        const auto& hits = jsonResponse["hits"]["hits"];
        if (hits.is_array() && !hits.empty()) {
            std::cout << "Результаты поиска для ФИО \"" << fullName << "\":\n";
            for (const auto& hit : hits) {
                if (hit.contains("_source")) {
                    const auto& source = hit["_source"];
                    std::cout << "ФИО: " << source["full_name"]
                              << ", Отдел: " << source["department"]
                              << ", Должность: " << source["position"]
                              << ", Дата начала работы: " << source["start_date"]
                              << ", Стаж (в днях): " << source["experience_in_days"] << "\n";
                }
            }
        } else {
            std::cout << "Нет данных для указанного ФИО.\n";
        }
    }
}
void inputEmployees(std::vector<Employee>& employees) {
    long long n = check("Введите количество сотрудников, который неоюходимо добавить: ");
    std::cin.ignore();

    for (int i = 0; i < n; ++i) {
        std::string startDate;
        std::string fullname = checkkk("Введите ФИО сотрудника: ");
        long long department = check("Введите номер отдела: ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string position = checkkk("Введите должность: ");
        std::cout << "Введите дату начала работы (YYYY-MM-DD): ";
        std::getline(std::cin, startDate);

        employees.emplace_back();  
        employees.back().fullName = fullname;
        employees.back().department = department;
        employees.back().position = position;
        employees.back().startDate = startDate;
    }
    uploadToElasticsearch(employees);
}
void displayEmployeesByDepartments(const std::vector<Employee>& employees) {
    if (employees.empty()) {
        std::cout << "Список сотрудников пуст.\n";
        return;
    }
    std::map<int, std::vector<Employee>> departmentGroups;
    for (const auto& emp : employees) {
        departmentGroups[emp.department].push_back(emp);
    }
    for (auto& [department, group] : departmentGroups) {
        std::sort(group.begin(), group.end(), [](const Employee& a, const Employee& b) {
            return getExperienceInDays(&a) > getExperienceInDays(&b);
        });
        std::cout << "Отдел " << department << ":\n";
        for (const auto& emp : group) {
            std::cout << "  ФИО: " << emp.fullName
                      << ", Должность: " << emp.position
                      << ", Дата начала работы: " << emp.startDate
                      << ", Стаж (в днях): " << getExperienceInDays(&emp) << "\n";
        }
    }
}
int main() {
    std::vector<Employee> employees;
    const std::string filename = "employees.txt";
    loadEmployeesFromFile(filename, employees);

    while (true) {
        long long a = check("1. Ввести сотрудников\n2. Редактировать запись\n3. Удалить запись\n4. Поиск данных по ФИО\n5. Вывод данных всех сотрудников\n6. Выход\nВведите: ");
        std::cin.ignore();
        switch (a) {
            case 1: {
                clearElasticsearchIndex("employees");
                inputEmployees(employees);
                saveEmployeesToFile(filename, employees);
                break;
            }
            case 2: {
                clearElasticsearchIndex("employees");
                editEmployeeRecord(employees);
                saveEmployeesToFile(filename, employees);
                break;
            }
            case 3: {
                deleteEmployeeRecord(employees);
                saveEmployeesToFile(filename, employees);
                break;
            }
            case 4: {
                std::string fullName = checkkk("Введите ФИО для поиска: ");
                searchEmployeeByFullName(fullName);
                break;
            }
            case 5: {
                displayEmployeesByDepartments(employees);
                break;
            }
            case 6: {
                return 0;
            }
            default:
                std::cout << "Неверный выбор.\n";
        }
    }
}
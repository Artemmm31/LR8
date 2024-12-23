#include "header.h"
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

void uploadToElasticsearch(const char* filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Не удалось открыть файл для загрузки в Elasticsearch.\n";
        return;
    }

    Participant participant;
    while (inFile.read(reinterpret_cast<char*>(&participant), sizeof(Participant))) {
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
    }
    inFile.close();
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
void deleteFromElasticsearch(int number) {
    std::string url = "http://localhost:9200/participants/_delete_by_query";
    nlohmann::json queryJson = {
        {"query", {{"match", {{"number", number}}}}}
    };
    sendPostRequest(url, queryJson.dump());
}
void searchPlayerByNumber(int number) {
    const std::string esUrl = "http://localhost:9200/participants/_search";

    nlohmann::json queryJson = {
        {"query", {{"match", {{"number", number}}}}}
    };

    std::string response = sendPostRequest(esUrl, queryJson.dump());
        nlohmann::json jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("hits") && jsonResponse["hits"].contains("hits")) {
            const auto& hits = jsonResponse["hits"]["hits"];
            if (hits.is_array() && !hits.empty()) {
                std::cout << "Результаты поиска для номера \"" << number << "\":\n";
                for (const auto& hit : hits) {
                    if (hit.contains("_source")) {
                        const auto& source = hit["_source"];
                        std::cout << "Страна: " << source["country"]
                                  << ", Команда: " << source["team"]
                                  << ", Имя: " << source["name"]
                                  << ", Номер: " << source["number"]
                                  << ", Возраст: " << source["age"]
                                  << ", Рост: " << source["height"] << "\n";
                    }
                }
            } else {
                std::cout << "Нет данных для указанного номера.\n";
            }
        } else {
            std::cout << "Ошибка: некорректный ответ от сервера Elasticsearch.\n";
        }
}
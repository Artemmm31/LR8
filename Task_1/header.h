#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#define MAX_PARTICIPANTS 100

typedef struct {
    char country[50];
    char team[50];
    char name[50];
    int number;
    int age;
    union {
        float height;
        float weight;
    } attributes;
} Participant;
void createFile(const char* filename);
void displayFile(const char* filename); 
unsigned long long WriteCallback(void* contents, unsigned long long size, unsigned long long nmemb, std::string* userp);
std::string sendPostRequest(const std::string& url, const std::string& jsonData);
void uploadToElasticsearch(const char* filename);
void deleteFromElasticsearch(int number);
void clearElasticsearchIndex(const std::string& indexName);
void searchPlayerByNumber(int number);
void modifyParticipantInFile(const char* filename);
void deleteParticipantFromFile(const char* filename);
void addParticipantToFile(const char* filename);
void displayMenu();
void sortAndDisplayParticipantsByAgeDescending(const char* filename);
void createDynamicArray(const char* filename);
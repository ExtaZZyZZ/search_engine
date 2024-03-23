#include "ConverterJSON.h"
#include <iostream>
#include <fstream>
const string FILE_ANSWER_PATH = "C:/Users/1testik/CLionProjects/search_engine/include/answers.json";
ConverterJson* ConverterJson::instance = nullptr;
ConverterJson* ConverterJson::getInstance(){
    if(instance == nullptr) {
        instance = new ConverterJson;
    }
    return instance;
}
std::vector<std::string> ConverterJson::GetTextDocuments() {
textDocument.clear();
for(const auto el: recourcePath) {
    std::cout << el << std::endl;
    ifstream elRead(el);
    if(elRead.is_open()) {
        string buffer;
        while(!elRead.eof()) {
            string word;
            elRead >> word;
            buffer += word;
            buffer += " ";
        }
        textDocument.push_back(buffer);
        cout << buffer << endl;
        elRead.close();
    }
    else {
        cerr << "Can not open the file" << endl;
    }
}
    cout << "Download docs is success " << recourcePath.size() << endl;
    return textDocument;
}
void ConverterJson::readConfigFile(string path) {
    cout << path << endl;
    ifstream file(path);
    if (file.is_open()) {
        if (is_empty(file)) {
            cerr << "config file is empty" << endl;
            throw runtime_error("File is empty " + path);
        }
        else {
            nlohmann::json configDigtionary;
            file >> configDigtionary;
            applicationName = configDigtionary["config"]["name"];
            applicationVersion = configDigtionary["config"]["version"];
            max_responses = configDigtionary["config"]["max_responses"];
            recourcePath.clear();
            for(auto & el: configDigtionary["files"]) {
                recourcePath.push_back(el);

            }
            cout << "Name: " << applicationName << endl;
            cout << "Version: " << applicationVersion << endl;
            cout << "Files library: " << recourcePath.size() << endl;
            cout << "Max responses: " << max_responses << endl;
            file.close();
        }
    }
        else{
            cerr << "config file is missing " << endl;
            throw runtime_error("File is missing " + path);
        }
}
void ConverterJson::readRequistFile(std::string path) {
    cout << "Request reading: " << endl;
    ifstream file(path);
    if(file.is_open()) {
        nlohmann::json requestDict;
        file >> requestDict;
        requests.clear();
        for(const auto& el: requestDict["requests"]) {
            requests.push_back(el);

        }
        file.close();
        cout << requests.size() << " requested are found" << endl;
    }
    else {
        cerr << "File is not found" << endl;
    }
}
void ConverterJson::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    nlohmann::json answersDict;
    if (!answers.empty())
    {
        std::cout << "Answers pushing... ";
        std::ofstream answersFile(FILE_ANSWER_PATH, std::ios_base::trunc);
        if (answersFile.is_open())
        {
            int requestCount {0};
            nlohmann::json answerDictionary;
            for (auto request : answers)
            {
                answersDict["answers"]["request" + std::to_string(requestCount)]["result"] = !request.empty();
                if (request.size() == 1)
                {
                    answersDict["answers"]["request" + std::to_string(requestCount)]["docid"] = request[0].first;
                    answersDict["answers"]["request" + std::to_string(requestCount)]["rank"] = request[0].second;
                }
                else
                {
                    auto relevance_array = nlohmann::json::array();
                    int relevanceID{0};
                    for (auto relevance : request)
                    {
                        ++relevanceID;
                        if (relevanceID > max_responses) break;

                        auto relevance_member = nlohmann::json::object();
                        relevance_member["docid"] = relevance.first;
                        relevance_member["rank"] = relevance.second;
                        relevance_array.push_back(relevance_member);
                    }
                    answersDict["answers"]["request" + std::to_string(requestCount)]["relevance"] = relevance_array;
                }
                ++requestCount;
            }
            answersFile << answersDict;
            answersFile.close();
            std::cout << "done\n";
        }
        else
        {
            std::cout << "\t error - file not found: " + FILE_ANSWER_PATH << "\n";
        }
    }
    else
    {
        std::cout << "No answers to push.\n";
    }
}

int ConverterJson::GetResponsesLimit () {
    return max_responses;
}
std::vector<std::string> ConverterJson::GetRequests()
{
    return requests;
}

//
// Created by 1testik on 01.03.2024.
//

#include <vector>
#include <istream>
#include <fstream>
#include <exception>
#include "../nlohmann_json/single_include/nlohmann/json.hpp"


using namespace std;
class ConverterJson {
public:
    static ConverterJson* getInstance();
    ConverterJson() = default;
    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
    void readRequistFile(string path = "C:/Users/1testik/CLionProjects/search_engine/src/requests.json");
    void readConfigFile(string path = "C:/Users/1testik/CLionProjects/search_engine/src/config.json");
private:
    static ConverterJson* instance;
    vector<string> requests;
    vector<string> textDocument;
    vector<string> recourcePath = {};

    string applicationName;
    string applicationVersion;
    int max_responses = 5;
    bool is_empty(std::ifstream& pFile) {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }
};


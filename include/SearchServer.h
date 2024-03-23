#include "InvertedIndex.h"
using namespace std;
struct RelativeIndex{
    size_t doc_id;
    float rank;
    size_t absolutIndex = 0;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
    bool operator > (const RelativeIndex& other) const {
        return (rank > other.rank || (rank == other.rank && doc_id < other.doc_id));
    }
};
class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    void setMaxResponses(const int& newMaxResponses);
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:
    int max_respons {5};
    set<string> getUniqueWord(const string &request);
    vector<pair<string, size_t>> getEntireWord(const set<string> &words);
    void sortWord(vector<pair<string, size_t>> &words);
    vector<size_t> GetWordsFromAllDoc(const vector<pair<string, size_t>>& words);
    InvertedIndex _index;
    size_t getAblsolutRelativeIndex(size_t docId, std::set<std::string> &uniqueWords);


};

#include <vector>
#include <map>
#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
using namespace std;
struct Entry {
    size_t doc_id, count;
    bool operator == (const Entry& other) const {
        return (doc_id == other.doc_id && count ==  other.count);
    }
};
class InvertedIndex {
    public:
    InvertedIndex() = default;
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    std::vector<Entry> GetWordCount(const std::string& word);
    void IndexFile(const string &filecontent, size_t doc_id);
    size_t GetWordCountInDoc(const size_t doc_id, const string &word) const;
    private:
    bool isgoingIndex;
    std::vector<std::string> docs;
    std::vector<std::string> recources;// список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};
// Created by 1testik on 03.03.2024.
//

#ifndef GOOGLE_TESTS_INVERTEDINDEX_H
#define GOOGLE_TESTS_INVERTEDINDEX_H

#endif //GOOGLE_TESTS_INVERTEDINDEX_H

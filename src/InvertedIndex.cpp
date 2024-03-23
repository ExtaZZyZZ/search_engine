#include "InvertedIndex.h"
static mutex MutexIndex;
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

    if(!input_docs.empty()) {
        isgoingIndex = true;
        freq_dictionary.clear();
        size_t doc_id = 0;

        for(auto& el: input_docs) {
            thread index([this,&el, doc_id]() {IndexFile(el, doc_id);});
            doc_id ++;
            index.join();
        }
    }
    else {
        cerr << "The file is empty " << endl;
    }
    isgoingIndex = false;
}
void InvertedIndex::IndexFile(const std::string &filecontent, size_t doc_id) {
    istringstream content(filecontent);
    map<string, Entry> freq_dict;
    Entry entry{};
    entry.doc_id = doc_id;
    entry.count = 1;
    for(string word; content >> word; ) {
        transform(word.begin(), word.end(), word.begin(), [] (unsigned char c){
            return tolower(c);
        });
        pair<string, Entry> temp_freq {word, entry};
        if(!freq_dict.emplace(temp_freq).second) {
            freq_dict.find(word)->second.count += 1;
        }
    }
    MutexIndex.lock();
    for(const auto& wordIterator: freq_dict) {
        pair<string, vector<Entry>> word_freq;
        word_freq.first = wordIterator.first;
        vector<Entry> insideEntry {wordIterator.second};
        word_freq.second = insideEntry;
        if(!freq_dictionary.emplace(word_freq).second) {
            freq_dictionary.find(word_freq.first)->second.push_back(wordIterator.second);
        }

    }
    MutexIndex.unlock();
    }

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    if (isgoingIndex)
    {
        std::cout << "Index is ongoing, please repeat the request later.\n";
        return {};
    }
auto it = freq_dictionary.find(word);
if(it!=freq_dictionary.end()) {
    return it->second;
}
else {
    return {};
}
}
size_t InvertedIndex::GetWordCountInDoc(const size_t doc_id, const std::string &word) const {
    if (isgoingIndex)
    {
        std::cout << "Index is ongoing, please repeat the request later.\n";
        return {};
    }
    size_t wordCount = 0;
    auto it = freq_dictionary.find(word);
    if(it!=freq_dictionary.end()) {
        auto entryVector = it->second;
        for(auto entry: entryVector) {
            if(entry.doc_id == doc_id) {
                wordCount +=entry.count;
            }

        }
    }
    return wordCount;
}


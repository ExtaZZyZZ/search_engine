#include "SearchServer.h"

set<string> SearchServer::getUniqueWord(const string &request) {
    std::istringstream file(request);
    set<string> uniqueWord;
    for(string word; file >> word; ) {
        transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
            return tolower(c);
        });
        uniqueWord.insert(word);
    }
    return uniqueWord;
}
vector<pair<string, size_t>> SearchServer::getEntireWord(const set<std::string> &words) {
    vector<pair<string,size_t>> result;
    for(const auto& word: words) {
        auto wordEntries = _index.GetWordCount(word);
        size_t wordEntriesSum = 0;
        for(auto wordEntry: wordEntries) {
            wordEntriesSum += wordEntry.count;
        }
        pair<string, size_t> temp;
        temp.first = word;
        temp.second = wordEntriesSum;
        result.push_back(temp);
    }
    return result;
}
void SearchServer::sortWord(vector<pair<std::string, size_t>> &words) {
    sort(words.begin(), words.end(), [] (auto& left, auto& right) {
        return left.second < right.second;
    });
}
vector<size_t> SearchServer::GetWordsFromAllDoc(const vector<pair<std::string, size_t>> &words) {
    vector<size_t> docs_id {};
    for(auto& docs: words) {
        auto wordEntries = _index.GetWordCount(docs.first);
        for(auto& entries: wordEntries) {
            docs_id.push_back(entries.doc_id);
        }
    }
    set<size_t> docsUniq {docs_id.begin(), docs_id.end()};
    docs_id.clear();
    docs_id.assign(docsUniq.begin(), docsUniq.end());
    sort(docs_id.begin(), docs_id.end(), less<size_t>());
    return docs_id;
}
size_t SearchServer::getAblsolutRelativeIndex(size_t docId, std::set<std::string> &uniqueWords) {
    size_t Relative = 0;
    for(const auto& word: uniqueWords ) {
        size_t wordCount = _index.GetWordCountInDoc(docId, word);
        Relative += wordCount;
    }
    return  Relative;
}
vector<vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    vector<vector<RelativeIndex>> result {};
    if(queries_input.empty()) {
        cout << "Request are empty, try again " << endl;
        return result;
    }


        for(const auto& query : queries_input) {
            set<string> UniqueWords = getUniqueWord(query);
            if(UniqueWords.empty()){
                cout << "Bad request " << endl;
                continue;
            }
           auto wordsEnries = getEntireWord(UniqueWords);
            sortWord(wordsEnries);
            auto documentsId = GetWordsFromAllDoc(wordsEnries);
            string doc = documentsId.size() == 1 ? "document" : "documents ";
            string words = UniqueWords.size() == 1 ? "words " : "word ";
            vector<RelativeIndex>* RelactiveIndex = new vector<RelativeIndex>();
            size_t MaxRelative = 0;
            for(const auto & docId: documentsId) {
                size_t AbsolutRelactive = getAblsolutRelativeIndex(docId, UniqueWords);
                auto* RelativInd = new RelativeIndex();
                RelativInd->doc_id = docId;
                RelativInd->absolutIndex = AbsolutRelactive;
                RelactiveIndex->push_back(*RelativInd);
                delete RelativInd;
                if(AbsolutRelactive > MaxRelative) {MaxRelative = AbsolutRelactive;}
            }
            for(auto& rel: *RelactiveIndex) {
                if(MaxRelative != 0 ) {
                    float rank = (float) rel.absolutIndex / (float) MaxRelative;
                    rel.rank = rank;
                }
                else {rel.rank = 0;}
            }
            sort(RelactiveIndex->begin(), RelactiveIndex->end(),[&RelactiveIndex] (RelativeIndex& left, RelativeIndex& right) {
                return (left.rank > right.rank || (left.rank == right.rank && left.doc_id < right.doc_id));
            });
            if(RelactiveIndex->size()> max_respons) {
                RelactiveIndex->erase(RelactiveIndex->begin()+max_respons, RelactiveIndex->end());
            }
            result.push_back(*RelactiveIndex);
            delete RelactiveIndex;
        }
        return result;
    }

void SearchServer::setMaxResponses(const int &newMaxResponses)
{
    this->max_respons = newMaxResponses;
}
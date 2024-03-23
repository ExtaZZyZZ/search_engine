#include <iostream>
#include "SearchServer.h"
#include "ConverterJSON.h"
using namespace std;
void writeAnswers(const vector<vector<RelativeIndex>>& allRelativeIndex){
    if(allRelativeIndex.empty()) {
        cout << "Not found relative request " << endl;
        return;
    }
    vector<vector<pair<int, float>>> requestResult;
    for(auto & result: allRelativeIndex) {
        vector<pair<int, float>> temp_res;

        for (auto &relevance: result) {
            pair<int, float> pair_relevance;
            pair_relevance.first = (int) relevance.doc_id;
            pair_relevance.second = relevance.rank;
            temp_res.push_back(pair_relevance);
        }
        requestResult.push_back(temp_res);
    }
    ConverterJson::getInstance()->putAnswers(requestResult);
}
int main()
{
    ConverterJson::getInstance()->readConfigFile();
   ConverterJson::getInstance()->readRequistFile();

   vector<string> documents = ConverterJson::getInstance()->GetTextDocuments();
   auto* InventedIndex = new InvertedIndex();
   InventedIndex->UpdateDocumentBase(documents);
   cout << "Search ... " << endl;
   SearchServer searchServer(*InventedIndex);
   searchServer.setMaxResponses(ConverterJson::getInstance()->GetResponsesLimit());
   auto requestResult = searchServer.search(ConverterJson::getInstance()->GetRequests());
    writeAnswers(requestResult);
    cout << "End of search.\n";

    cin.get();
    return 0;

}

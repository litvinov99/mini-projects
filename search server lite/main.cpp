/* 
                            -- ПРИНЦИП РАБОТЫ --
Класс SearchSystem реализует поисковый индекс. Чтобы добавить новый документ в
индекс используется метод AddDocument(id, new_document). С помощью метода 
FindTopDocuments(raw_reque) можно получить отсортированный по релевантности мас-
сив структур Document, содержащих id и relevance каждого документа на запрос 
raw_reque.
 
                    -- ДОКАЗАТЕЛЬСТВО КОРРЕКТНОСТИ --
1. Код корректно индексирует документы, разбивая их на слова и сохраняя их коли-
чество в word_index[word][id];
2. При поиске запроса считается релевантность документов, суммируя количество 
вхождений запрашиваемых слов.
3. Быстрая фильтрация nth_element оставляет топ-5 релевантных документов, а sort 
упорядочивает их по убыванию релевантности и id;
Алгоритм полностью соответствует условиям задачи и гарантирует корректный вывод.

                       -- ВРЕМЕННАЯ СЛОЖНОСТЬ --
* Добавление документа AddDocument():
    - SplitIntoWordsNoStop() разбивает строку документа на слова за O(L), где L - 
    количество слов в документе;
    - Запись в word_index — в среднем за O(1) на слово. 
    Итоговая сложность - O(N * L), где N — число документов, L — средняя длина до-
    кумента.

* Обработка запроса FindTopDocuments() 
    - SplitIntoWordsNoStop() разбивает строку запроса на слова за также за O(L);
    - FindMatchedDocuments() перебирает все слова за О(Q), для каждого слова по-
    лучаем список документов, в которых оно встречается. В худшем случае слово
    встречается во всех документах O(N);
    - Сортировка релевантных документов: nth_element за O(D) находит топ-5 элемен-
    тов и помещает их в начало, где D - кол-во релевантных документов. sort сорти-
    рует только 5 элементов, поэтому можно считать O(5 log 5) = O(1)
    Итогова сложность обработки одного запроса O(Q * N + D);
 
                    -- ПРОСТРАНСТВЕННАЯ СЛОЖНОСТЬ --
- хранение индекса word_index - в худшем случае, если каждое слово есть в каждом 
документе, word_index занимает O(W * N) памяти, где W - количество уникальных эле-
ментов;
- обработка запроса (query_words) - хранит слова из запроса и их количество О(1);
- хранение релевантностей (relevances) - в худшем случае, все N документов релеван-
тны О(N) памяти;
- хранение топ-документов (top_documents) - хранит маскимум 5 элементов О(1);
Итоговая сложность O(W * N + N) ≈ O(W * N) (если W ≈ L, то O(L * N)).
Потребление памяти может быть очень большим, если в каждом документе много уникаль-
ных слов.
*/
 
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_map>

const size_t MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    int id;
    int relevance;
};

class SearchSystem {
public:
    void AddDocument(int id, const std::string& document) {
        std::unordered_map<std::string, int> document_words = SplitIntoWordsNoStop(document);
        for (const auto& [word, count] : document_words) {
            word_index[word][id] += count;
        }
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query) const {
        std::unordered_map<std::string, int> query_words = SplitIntoWordsNoStop(raw_query);
        std::vector<Document> top_documents = FindMatchedDocuments(query_words);
        if (top_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            std::nth_element(top_documents.begin(), 
                             top_documents.begin() + MAX_RESULT_DOCUMENT_COUNT, 
                             top_documents.end(),
                             [](const Document& lhs, const Document& rhs) {
                                 return (lhs.relevance == rhs.relevance) ? (lhs.id < rhs.id) : (lhs.relevance > rhs.relevance);
                             });

            top_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
            std::sort(top_documents.begin(), top_documents.end(),
                    [](const Document& lhs, const Document& rhs) {
                        return (lhs.relevance == rhs.relevance) ? (lhs.id < rhs.id) : (lhs.relevance > rhs.relevance);
                    });
        }
        std::sort(top_documents.begin(), top_documents.end(),
              [](const Document& lhs, const Document& rhs) {
                  return (lhs.relevance == rhs.relevance) ? (lhs.id < rhs.id) : (lhs.relevance > rhs.relevance);
              });
        return top_documents;
    }

private:
    std::set<std::string> stop_words = {}; 
    // стоп слова, например {"with", "a", "the", "without", "in", "and", "at"}
    std::unordered_map<std::string, std::unordered_map<int, int>> word_index; // слово -> {id документа -> количество}

    bool IsStopWord(const std::string& word) const {
        return stop_words.count(word) > 0;
    }

    std::unordered_map<std::string, int> SplitIntoWordsNoStop(const std::string& text) const {
        std::stringstream ss(text);
        std::unordered_map<std::string, int> words;
        std::string word;
        while (ss >> word) {
            if (!IsStopWord(word)) {
                ++words[word];
            }
        }
        return words;
    }

    std::vector<Document> FindMatchedDocuments(std::unordered_map<std::string, int>& query_words) const {
        std::unordered_map<int, int> relevances; // {id документа -> релевантность}
        for (const auto& [word, count] : query_words) {
            if (word_index.count(word) == 0) {
                continue;
            }
            for (const auto& [id, freq] : word_index.at(word)) {
                relevances[id] += freq;
            }
        }
        std::vector<Document> matched_documents;
        for (const auto &[id, relevance] : relevances) {
            matched_documents.push_back({id, relevance});
        }
        return matched_documents;
    }   
};

int main() {
    SearchSystem search_system;
    int doc_count;
    std::cin >> doc_count;
    std::cin.ignore();
    for (int i = 0; i < doc_count; ++i) {
        std::string new_document;
        getline(std::cin, new_document);
        search_system.AddDocument(i + 1, new_document);
    }

    int request_count;
    std::cin >> request_count;
    std::cin.ignore();
    std::vector<std::string> requests;
    for (int i = 0; i < request_count; ++i) {
        std::string new_request;
        getline(std::cin, new_request);
        requests.push_back(new_request);
    }

    for (const auto& request : requests) {
        std::vector<Document> top_documents = search_system.FindTopDocuments(request);
        for (const auto& [id, relevance] : top_documents) {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }
}

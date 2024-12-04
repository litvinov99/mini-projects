#include "document.h"

#include<iostream>

Document::Document() = default;
Document::Document(int id, double relevance, int rating)
    : id(id)
    , relevance(relevance)
    , rating(rating) {
}

void PrintDocument(const Document& document) {
    std::cout << "{ "
         << "document_id = " << document.id << ", "
         << "relevance = " << document.relevance << ", "
         << "rating = " << document.rating << " }";
}

std::ostream& operator<<(std::ostream& out, Document doc) {
    out << "{ "
         << "document_id = " << doc.id << ", "
         << "relevance = " << doc.relevance << ", "
         << "rating = " << doc.rating << " }";
    return out;
}

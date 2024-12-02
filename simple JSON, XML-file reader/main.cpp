#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

#include "json.h"
#include "xml.h"

struct Spending {
    std::string category;
    int amount;
};

int CalculateTotalSpendings(const std::vector<Spending>& spendings) {
    return accumulate(
        spendings.begin(), spendings.end(), 0, [](int current, const Spending& spending){
            return current + spending.amount;
        });
}

std::string FindMostExpensiveCategory(const std::vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

// reads JSON from the input stream and returns a spending vector
std::vector<Spending> LoadFromJson(std::istream& input) {
    std::vector<Spending> result;
    json::Document doc = json::Load(input);
    std::vector<json::Node> vec_obj = doc.GetRoot().AsArray();
    for (const auto& obj : vec_obj) {
        int amount = obj.AsMap().at("amount").AsInt();
        std::string category = obj.AsMap().at("category").AsString();
        result.push_back({category, amount});
    }  
    return (result);
}

// reads XML from the input stream and returns a spending vector
std::vector<Spending> LoadFromXml(std::istream& input) {
    std::vector<Spending> result;
    xml::Document doc = xml::Load(input);
    std::vector<xml::Node> vec_doc = doc.GetRoot().Children();
    for (const auto& data : vec_doc) {
        int amount = data.AttributeValue<int>("amount");
        std::string category = data.AttributeValue<std::string>("category");
        result.push_back({ category, amount });
    }
    return (result);
}

void TestReadingFromJSON() {
    std::istringstream input {
        "[\n"
            "{\"amount\": 2500, \"category\": \"food\"},"
            "{\"amount\": 1150, \"category\": \"transport\"},"
            "{\"amount\": 5780, \"category\": \"restaurants\"},"
            "{\"amount\": 7500, \"category\": \"clothes\"},"
            "{\"amount\": 23740, \"category\": \"travel\"},"
            "{\"amount\": 12000, \"category\": \"sport\"}"
        "]\n" 
    };
    const std::vector<Spending> spendings = LoadFromJson(input);
    assert(CalculateTotalSpendings(spendings) == 52670);
    assert(FindMostExpensiveCategory(spendings) == "travel");
    std::cout << "TEST READING JSON - OK\n";
}

void TestReadingFromXML() {
    std::istringstream input {
        "<july>\n"
            "<spend amount=\"2500\" category=\"food\"></spend>\n"
            "<spend amount=\"1150\" category=\"transport\"></spend>\n"
            "<spend amount=\"5780\" category=\"restaurants\"></spend>\n"
            "<spend amount=\"7500\" category=\"clothes\"></spend>\n"
            "<spend amount=\"23740\" category=\"travel\"></spend>\n"
            "<spend amount=\"12000\" category=\"sport\"></spend>\n"
        "</july>\n"
    };
    const std::vector<Spending> spendings = LoadFromXml(input);
    assert(CalculateTotalSpendings(spendings) == 52670);
    assert(FindMostExpensiveCategory(spendings) == "travel");
    std::cout << "TEST READING XML - OK\n";

}

int main() {
    TestReadingFromJSON();
    TestReadingFromXML();
}
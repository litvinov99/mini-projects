#include "json.h"

namespace json {

Node::Node(std::vector<Node> array)
    : as_array_(move(array)) {
}

Node::Node(std::map<std::string, Node> map)
    : as_map_(move(map)) {
}

Node::Node(int value)
    : as_int_(value) {
}

Node::Node(std::string value)
    : as_string_(move(value)) {
}

const std::vector<Node>& Node::AsArray() const {
    return as_array_;
}

const std::map<std::string, Node>& Node::AsMap() const {
    return as_map_;
}

int Node::AsInt() const {
    return as_int_;
}

const std::string& Node::AsString() const {
    return as_string_;
}

Document::Document(Node root)
    : root_(std::move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Node LoadArray(std::istream& input) {
    std::vector<Node> result;

    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(std::move(result));
}

Node LoadInt(std::istream& input) {
    int result = 0;
    while (isdigit(input.peek())) {
        result *= 10;
        result += input.get() - '0';
    }
    return Node(result);
}

Node LoadString(std::istream& input) {
    std::string line;
    getline(input, line, '"');
    return Node(std::move(line));
}

Node LoadDict(std::istream& input) {
    std::map<std::string, Node> result;

    for (char c; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }

        std::string key = LoadString(input).AsString();
        input >> c;
        result.insert({std::move(key), LoadNode(input)});
    }

    return Node(move(result));
}

Node LoadNode(std::istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else {
        input.putback(c);
        return LoadInt(input);
    }
}

Document Load(std::istream& input) {
    return Document{LoadNode(input)};
}

} // json namespace
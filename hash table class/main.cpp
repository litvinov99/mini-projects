/* 
                            -- ПРИНЦИП РАБОТЫ --
Класс HashMap реализует хеш-таблицу с цепочечным хешированием. Добавление пар 
"ключ-значение" выполняется методом insert(key, value), поиск значения по клю-
чу - методом get(key), а удаление - методом remove(key). Внутри используется 
динамически расширяемый массив списков (цепочек), в которых хранятся элементы 
с одинаковым хешем.
 
                    -- ДОКАЗАТЕЛЬСТВО КОРРЕКТНОСТИ --
1. Хеш-таблица корректно распределяет элементы с помощью хеш-функции на основе 
MurmurHash.
2. Коллизии разрешаются методом цепочек, каждый бакет содержит односвязный спи-
сок элементов.
3. При превышении порога заполненности (FILL_FACTOR = 90) выполняется рехеширо-
вание в новый массив большего размера, где размер выбирается как следующее прос-
тое число.
4. Операции вставки, поиска и удаления элементов работают в среднем за O(1) при 
равномерном распределении элементов.

                       -- ВРЕМЕННАЯ СЛОЖНОСТЬ --
* Добавление элемента insert():
        - вычисление хеш-функции O(1);
        - вставка элемента в начало списка O(1);
        - в худшем случае (все элементы в одном бакете) O(N);
        - рехеширование в случае превышения FILL_FACTOR выполняется за O(N);
        Итоговая сложность в среднем случае O(1), в худшем - O(N).
* Поиск элемента get():
        - вычисление хеш-функции O(1);
        - поиск элемента в списке в среднем случае O(1), в худшем - O(N);
        Итоговая сложность в среднем случае O(1) , в худшем - O(N).
* Удаление элемента remove():
        - вычисление хеш-функции O(1);
        - поиск и удаление из списка в среднем случае O(1), в худшем - O(N);
        Итоговая сложность в среднем случае O(1), в худшем - O(N).
 
                    -- ПРОСТРАНСТВЕННАЯ СЛОЖНОСТЬ --
* Хранение массива бакетов O(N);
* В худшем случае, если все элементы попадают в один бакет, пространство может 
достигать O(N);
* при увеличении размера массива после рехеширования дополнительная память O(N);
Итоговая сложность: O(N).
*/
#include <iostream>
#include <vector>
#include <cstdint>
#include <optional>

class HashMap {
public:
    HashMap(size_t init_capacity = 7) 
        : size_(0)
        , capacity_(init_capacity) {
        table_ = new Bucket[capacity_];
    }

    ~HashMap() {
        for (size_t i = 0; i < capacity_; ++i) {
            clear_list(table_[i].head);
        }
        delete[] table_;
    }

    void insert(int key, int value) {
        if ((size_ * 100) / capacity_ >= FILL_FACTOR) {
            rehash();
        }
        size_t index = hash_function(key) % capacity_;
        Node* node = table_[index].head;

        while (node) {
            if (node->key == key) {
                node->value = value;
                return;
            }
            node = node->next;
        }

        Node* new_node = new Node(key, value);
        new_node->next = table_[index].head;
        table_[index].head = new_node;
        ++size_;
    }

    std::optional<int> get(int key) const {
        size_t index = hash_function(key) % capacity_;
        Node* node = table_[index].head;
        while (node) {
            if (node->key == key) {
                int value = node->value;
                return value;
            }
            node = node->next;
        }
        return std::nullopt;
    }

    std::optional<int> remove(int key) {
        size_t index = hash_function(key) % capacity_;
        Node* node = table_[index].head;
        Node* prev = nullptr;

        while (node) {
            if (node->key == key) {
                int removed_value = node->value;
                if (prev) {
                    prev->next = node->next;
                } else {
                    table_[index].head = node->next;
                }
                delete node;
                --size_;
                return removed_value;
            }
            prev = node;
            node = node->next;
        }
        return std::nullopt;
    }

private:
    struct Node {
        int key;
        int value;
        Node *next;
        Node() : value(0), next(nullptr) {}
        Node(int k, int v) : key(k), value(v), next(nullptr) {}
        Node(int k, int v, Node *next) : key(k), value(v), next(next) {}
    };

    struct Bucket {
        Node* head;
        Bucket() : head(nullptr) {}
    };

    Bucket* table_;
    size_t size_;
    size_t capacity_;

    const size_t FILL_FACTOR = 90;

    uint64_t hash_function(int key) const {
        key ^= (key >> 16);
        key *= 0x85ebca6b;
        key ^= (key >> 13);
        key *= 0xc2b2ae35;
        key ^= (key >> 16);
        return key;
    }

    void clear_list(Node* node) {
        while (node) {
            Node* temp = node;
            node = node->next;
            delete temp;
        }
    }

    bool is_prime(size_t num) const {
        if (num < 2) return false;
        for (size_t i = 2; i * i <= num; ++i) {
            if (num % i == 0) return false;
        }
        return true;
    }

    size_t next_prime_capacity(size_t current) const {
        size_t next_capacity = current * 2; 
        while (!is_prime(next_capacity)) {
            ++next_capacity;
        }
        return next_capacity;
    }

    void rehash() {
        size_t new_capacity = next_prime_capacity(capacity_);
        Bucket* new_table = new Bucket[new_capacity];

        for (size_t i = 0; i < capacity_; ++i) {
            Node* node = table_[i].head;
            while (node) {
                size_t new_index = hash_function(node->key) % new_capacity;
                Node* new_node = new Node(node->key, node->value);
                new_node->next = new_table[new_index].head;
                new_table[new_index].head = new_node;
                node = node->next;
            }
            clear_list(table_[i].head);
        }

        delete[] table_;
        table_ = new_table;
        capacity_ = new_capacity;
    }

};

int main() {
    HashMap hash_map;
    int request_count;
    std::cin >> request_count;
    for (int i = 0; i < request_count; ++i) {
        std::string command;
        std::cin >> command;
        if (command == "put") {
            int key;
            int value;
            std::cin >> key >> value;
            hash_map.insert(key, value);
        } else if (command == "get") {
            int key;
            std::cin >> key;
            auto item = hash_map.get(key);
            std::cout << (item ? std::to_string(item.value()) : "None") << std::endl;
        } else if (command == "delete") {
            int key;
            std::cin >> key;
            auto item = hash_map.remove(key);
            std::cout << (item ? std::to_string(item.value()) : "None") << std::endl;
        }
    }

}
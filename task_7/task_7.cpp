#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

#define LOG_SWITCH


// Базовый интерфейс для хранения данных
class DataStorage {
public:
    virtual ~DataStorage() = default;
    virtual void insert(int element) = 0;
    virtual void erase(int element) = 0;
    virtual bool has(int element) const = 0;
    virtual std::vector<int> get_all() const = 0;
};

// Реализация на основе массива
class ArrayStorage : public DataStorage {
    std::vector<int> container;
public:
    void insert(int element) override {
        if (!has(element)) container.push_back(element);
    }
    void erase(int element) override {
        container.erase(std::remove(container.begin(), container.end(), element), container.end());
    }
    bool has(int element) const override {
        return std::find(container.begin(), container.end(), element) != container.end();
    }
    std::vector<int> get_all() const override {
        return container;
    }
};

// Реализация на основе хеш-множества
class HashStorage : public DataStorage {
    std::unordered_set<int> container;
public:
    void insert(int element) override {
        container.insert(element);
    }
    void erase(int element) override {
        container.erase(element);
    }
    bool has(int element) const override {
        return container.count(element) > 0;
    }
    std::vector<int> get_all() const override {
        return std::vector<int>(container.begin(), container.end());
    }
};

// Основной класс для работы с множеством
class SmartSet {
    std::unique_ptr<DataStorage> storage;

    static constexpr size_t SWITCH_LIMIT = 10;
    
    void check_and_switch() {
        size_t current_size = storage->get_all().size();
        bool is_hash_based = dynamic_cast<HashStorage*>(storage.get()) != nullptr;

#ifdef LOG_SWITCH
        std::string new_type = is_hash_based ? "array" : "hash";
        std::cout << "Switching storage to " << new_type 
                  << " (current size: " << current_size << ")\n";
#endif

        if (!is_hash_based && current_size > SWITCH_LIMIT) {
            auto elements = storage->get_all();
            storage = std::make_unique<HashStorage>();
            for (int elem : elements) storage->insert(elem);
        } 
        else if (is_hash_based && current_size <= SWITCH_LIMIT) {
            auto elements = storage->get_all();
            storage = std::make_unique<ArrayStorage>();
            for (int elem : elements) storage->insert(elem);
        }
    }

public:
    SmartSet() : storage(std::make_unique<ArrayStorage>()) {}

    void insert(int element) {
        storage->insert(element);
        if (storage->get_all().size() == SWITCH_LIMIT + 1) check_and_switch();
    }
    
    void erase(int element) {
        storage->erase(element);
        if (storage->get_all().size() == SWITCH_LIMIT) check_and_switch();
    }
    
    bool contains(int element) const {
        return storage->has(element);
    }
    
    SmartSet combine(const SmartSet& other) const {
        SmartSet result;
        for (int elem : storage->get_all()) result.insert(elem);
        for (int elem : other.storage->get_all()) result.insert(elem);
        return result;
    }
    
    SmartSet common(const SmartSet& other) const {
        SmartSet result;
        for (int elem : storage->get_all()) {
            if (other.contains(elem)) result.insert(elem);
        }
        return result;
    }
    
    void display() const {
        auto elements = storage->get_all();
        std::cout << "{";
        for (size_t i = 0; i < elements.size(); ++i) {
            std::cout << elements[i] << (i+1 < elements.size() ? ", " : "");
        }
        std::cout << "}\n";
    }
};

int main() {
    SmartSet setA;
    for (int i = 1; i < 12; ++i) setA.insert(i);

    setA.display();  // Использует хеш-хранилище
    setA.erase(5);
    setA.display();  // Может переключиться на массив

    SmartSet setB;
    setB.insert(2); setB.insert(3); setB.insert(100);

    auto combined = setA.combine(setB);
    auto intersection = setA.common(setB);

    std::cout << "Combined: "; combined.display();
    std::cout << "Common: "; intersection.display();
}
#include <iostream>
#include <string>
#include "TypeMap.hpp"

struct RecordA {
    std::string text;
};

struct RecordB {
    int number;
};

int main() {
    TypeStorage<int, RecordA, double, RecordB> storage;

    // Добавление элементов
    storage.Store<int>(42);
    storage.Store<double>(3.14);
    storage.Store<double>(32.14);
    storage.Store<RecordA>("Hello from TypeMap!");
    storage.Store<RecordB>(10);

    // Получение и вывод значений
    std::cout << "Integer value: " << storage.Retrieve<int>() << std::endl;
    std::cout << "Double value: " << storage.Retrieve<double>() << std::endl;
    std::cout << "RecordA text: " << storage.Retrieve<RecordA>().text << std::endl;
    std::cout << "RecordB number: " << storage.Retrieve<RecordB>().number << std::endl;

    // Проверка наличия элемента
    std::cout << "Contains int? " << (storage.Contains<int>() ? "Yes" : "No") << std::endl;

    // Удаление элемента
    storage.Remove<double>();

    // Попытка получения удаленного элемента
    try {
        std::cout << "Double after removal: " << storage.Retrieve<double>() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
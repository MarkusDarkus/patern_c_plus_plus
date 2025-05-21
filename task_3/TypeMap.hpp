#ifndef TYPE_STORAGE_HPP
#define TYPE_STORAGE_HPP

#include <memory>
#include <array>
#include <variant>
#include <stdexcept>
#include <typeinfo>
#include <utility>

template<typename... Types>
class TypeStorage {
private:
    template<typename T, typename... Ts>
    struct TypeIndex;

    template<typename T, typename... Ts>
    struct TypeIndex<T, T, Ts...> {
        static constexpr size_t value = 0;
    };

    template<typename T, typename U, typename... Ts>
    struct TypeIndex<T, U, Ts...> {
        static constexpr size_t value = 1 + TypeIndex<T, Ts...>::value;
    };

    template<typename T>
    struct TypeIndex<T> {
        static constexpr size_t value = 0;
    };

    template<typename T>
    static constexpr bool contains = (std::is_same_v<T, Types> || ...);

    using StorageVariant = std::variant<std::unique_ptr<Types>...>;
    std::array<StorageVariant, sizeof...(Types)> storage;

public:
    template<typename T, typename... Args>
    void Store(Args&&... args) {
        static_assert(contains<T>, "Type not supported in this TypeStorage");
        constexpr size_t index = TypeIndex<T, Types...>::value;
        storage[index] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    T& Retrieve() {
        if (!Contains<T>()) {
            throw std::runtime_error(std::string("Type not found: ") + typeid(T).name());
        }
        constexpr size_t index = TypeIndex<T, Types...>::value;
        return *std::get<std::unique_ptr<T>>(storage[index]);
    }

    template<typename T>
    bool Contains() const {
        if constexpr (!contains<T>) {
            return false;
        } else {
            constexpr size_t index = TypeIndex<T, Types...>::value;
            return std::holds_alternative<std::unique_ptr<T>>(storage[index]) && 
                   std::get<std::unique_ptr<T>>(storage[index]) != nullptr;
        }
    }

    template<typename T>
    void Remove() {
        if (!Contains<T>()) {
            throw std::runtime_error(std::string("Cannot remove type: ") + typeid(T).name());
        }
        constexpr size_t index = TypeIndex<T, Types...>::value;
        std::get<std::unique_ptr<T>>(storage[index]).reset();
    }
};

#endif // TYPE_STORAGE_HPP
// TypeList.hpp
#ifndef NEW_TYPELIST_H
#define NEW_TYPELIST_H

#include <type_traits>

namespace TypeSystem {
    namespace Internal {
        template <int Count, typename...> struct Counter;
        
        template <int Count, typename First, typename... Rest>
        struct Counter<Count, First, Rest...> : Counter<Count + 1, Rest...> {};
        
        template <int Count>
        struct Counter<Count> {
            enum { Value = Count };
        };

        template <typename Target, typename...> struct Finder;
        
        template <typename Target, typename First, typename... Rest>
        struct Finder<Target, First, Rest...> : Finder<Target, Rest...> {};
        
        template <typename Target, typename... Rest>
        struct Finder<Target, Target, Rest...> {
            enum { Found = true };
        };
        
        template <typename Target>
        struct Finder<Target> {
            enum { Found = false };
        };

        template <int Pos, int Start, typename Target, typename...> struct Locator;
        
        template <int Pos, int Start, typename Target, typename First, typename... Rest>
        struct Locator<Pos, Start, Target, First, Rest...> : 
            Locator<Pos + 1, Start, Target, Rest...> {};
        
        template <int Pos, typename Target, typename... Rest>
        struct Locator<Pos, Pos, Target, Target, Rest...> {
            enum { Position = Pos };
        };
        
        template <int Pos, typename Target>
        struct Locator<Pos, Pos, Target> {
            enum { Position = -1 };
        };

        template <int Index, typename...> struct TypeExtractor;
        
        template <int Index, typename First, typename... Rest>
        struct TypeExtractor<Index, First, Rest...> : 
            TypeExtractor<Index - 1, Rest...> {};
        
        template <typename First, typename... Rest>
        struct TypeExtractor<0, First, Rest...> {
            using Result = First;
        };
    }

    template <typename... Elements>
    struct TypeCollection {
        static constexpr int length = Internal::Counter<0, Elements...>::Value;
        
        template <typename T>
        static constexpr bool has_type = Internal::Finder<T, Elements...>::Found;
        
        template <typename T, int From = 0>
        static constexpr int find_position = 
            Internal::Locator<From, From, T, Elements...>::Position;
        
        template <int N>
        using get_type_at = typename Internal::TypeExtractor<N, Elements...>::Result;
        
        template <typename T>
        static constexpr TypeCollection<T, Elements...> add_front{};
        
        template <typename T>
        static constexpr TypeCollection<Elements..., T> add_back{};
    };
}

#endif // NEW_TYPELIST_H
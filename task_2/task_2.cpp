// task_2.cpp
#include "TypeList.hpp"
#include <iostream>
#include <string>

int main() {
    constexpr TypeSystem::TypeCollection<int, double, std::string, float, int> t1;

    std::cout << t1.length << std::endl;
    std::cout << t1.has_type<std::string> << std::endl;
    std::cout << "index " << t1.find_position<int, 2> << std::endl;

    using t1_type = TypeSystem::TypeCollection<int, double, std::string, float, int>;
    std::cout << std::is_same_v<t1_type::get_type_at<3>, int> << std::endl;

    auto xj9 = t1.add_back<std::exception>;
    std::cout << t1.has_type<std::exception> << std::endl;
    std::cout << xj9.has_type<std::exception> << " " << xj9.find_position<std::exception> << std::endl;

    auto bebrhype = t1.add_front<std::exception>;
    std::cout << t1.has_type<std::exception> << std::endl;
    std::cout << bebrhype.has_type<std::exception> << " " << bebrhype.find_position<std::exception> << std::endl;
}
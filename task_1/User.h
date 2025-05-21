#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <stdexcept>

class Group;

class User {
public:
    friend std::ostream& operator<<(std::ostream& os, const User& u) {
        os << "User ID: " << u.id << ", Name: " << u.name;
        return os;
    }
    User() = default;
    ~User() = default;
    User(const std::string& uname, unsigned int uid) : name(uname), id(uid) {}
    User& operator=(const User& other);
    User(const User& other);

    Group* GetGroupRef() const;
    Group& GetGroup() const;
    unsigned int GetId() const { return id; }

    std::string name;
private:
    unsigned int id{};
    Group* group_ref_ = nullptr;
};

#endif // USER_H

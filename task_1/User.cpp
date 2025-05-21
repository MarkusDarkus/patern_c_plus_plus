#include "User.h"

User& User::operator=(const User& other) {
    if (this != &other) {
        group_ref_ = other.GetGroupRef();
        name = other.name;
        id = other.id;
    }
    return *this;
}

Group* User::GetGroupRef() const {
    return group_ref_;
}

Group& User::GetGroup() const {
    if (!group_ref_) throw std::logic_error("User not in any group");
    return *group_ref_;
}

User::User(const User& other) : id(other.id), group_ref_(other.group_ref_), name(other.name) {}

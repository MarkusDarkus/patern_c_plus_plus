#include "Group.h"
#include "User.h"
#include <iostream>

bool Group::HasUser(unsigned int uid) const {
    return user_map_.count(uid) > 0;
}

void Group::InsertUser(std::shared_ptr<User>& u) {
    if (HasUser(u->GetId())) {
        throw std::logic_error("User already present");
    }
    user_map_[u->GetId()] = u;
}

void Group::DeleteUser(unsigned int uid) {
    if (!HasUser(uid)) {
        throw std::logic_error("No such user");
    }
    user_map_.erase(uid);
}

std::shared_ptr<User>& Group::FetchUser(unsigned int uid) {
    if (!HasUser(uid)) {
        throw std::logic_error("No such user");
    }
    return user_map_[uid];
}

void Group::DisplayUsers(bool tabs) const {
    char sep = tabs ? '\t' : ' ';
    for (const auto& [id, user_ptr] : user_map_) {
        std::cout << sep << *user_ptr << std::endl;
    }
}

void Group::TransferUsers(Group& dest) {
    if (&dest == this) {
        throw std::logic_error("Cannot transfer to self");
    }
    for (auto& [id, user_ptr] : user_map_) {
        dest.InsertUser(user_ptr);
    }
    // user_map_.clear(); // optional: uncomment to clear after transfer
}

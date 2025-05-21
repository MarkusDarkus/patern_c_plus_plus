#include "Observer.h"
#include "User.h"
#include "Group.h"
#include <iostream>

bool Observer::UserExists(unsigned int uid) const {
    for (const auto& [gid, group] : groups_) {
        if (group.HasUser(uid)) return true;
    }
    return false;
}

bool Observer::GroupExists(unsigned int gid) const {
    return groups_.count(gid) > 0;
}

void Observer::AddUser(unsigned int uid, std::string uname) {
    if (uid == 0) {
        std::cerr << "User ID must be positive!" << std::endl;
        return;
    }
    if (UserExists(uid)) {
        std::cerr << "User already exists" << std::endl;
        return;
    }
    auto ptr = std::make_shared<User>(uname, uid);
    groups_[0].InsertUser(ptr);
}

void Observer::RemoveUser(unsigned int uid) {
    if (!UserExists(uid)) {
        std::cerr << "User not found" << std::endl;
        return;
    }
    for (auto& [gid, group] : groups_) {
        if (group.HasUser(uid)) {
            group.DeleteUser(uid);
            return;
        }
    }
}

void Observer::ShowAllUsers() const {
    for (const auto& [gid, group] : groups_) {
        group.DisplayUsers();
    }
}

void Observer::ShowUser(unsigned int uid) {
    if (!UserExists(uid)) {
        std::cerr << "User not found" << std::endl;
        return;
    }
    for (auto& [gid, group] : groups_) {
        if (group.HasUser(uid)) {
            std::cout << *group.FetchUser(uid) << std::endl;
        }
    }
}

void Observer::AddGroup(unsigned int gid) {
    if (gid == 0) {
        std::cerr << "Group ID must be positive!" << std::endl;
        return;
    }
    if (GroupExists(gid)) {
        std::cerr << "Group already exists" << std::endl;
        return;
    }
    groups_[gid] = Group(gid);
}

void Observer::RemoveGroup(unsigned int gid) {
    if (gid == 0) {
        std::cerr << "Group ID must be positive!" << std::endl;
        return;
    }
    if (!GroupExists(gid)) {
        std::cerr << "Group not found" << std::endl;
        return;
    }
    groups_[gid].TransferUsers(groups_[0]);
    groups_.erase(gid);
}

void Observer::ShowAllGroups() const {
    for (const auto& [gid, group] : groups_) {
        std::cout << group << std::endl;
    }
}

void Observer::ShowGroup(unsigned int gid) {
    if (!GroupExists(gid)) {
        std::cerr << "Group not found" << std::endl;
        return;
    }
    std::cout << groups_[gid] << std::endl;
}

void Observer::AssignUserToGroup(unsigned int uid, unsigned int gid) {
    if (!UserExists(uid)) {
        std::cerr << "User not found" << std::endl;
        return;
    }
    if (!GroupExists(gid)) {
        std::cerr << "Group not found" << std::endl;
        return;
    }
    unsigned int current_gid = 0;
    for (const auto& [grp_id, group] : groups_) {
        if (group.HasUser(uid)) {
            current_gid = grp_id;
            break;
        }
    }
    if (gid == current_gid) {
        if (gid == 0)
            std::cerr << "User is not in any group" << std::endl;
        else
            std::cerr << "User already in this group" << std::endl;
        return;
    }
    groups_[gid].InsertUser(groups_[current_gid].FetchUser(uid));
    groups_[current_gid].DeleteUser(uid);
}

void Observer::RemoveUserFromGroup(unsigned int uid) {
    AssignUserToGroup(uid, 0);
}
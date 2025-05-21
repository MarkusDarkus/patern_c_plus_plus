#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "Group.h"

class Observer {
public:
    Observer() {
        groups_.emplace(0, Group(0)); // group 0: ungrouped
    }

    void AddUser(unsigned int uid, std::string uname);
    void RemoveUser(unsigned int uid);
    void ShowAllUsers() const;
    void ShowUser(unsigned int uid);
    void AddGroup(unsigned int gid);
    void RemoveGroup(unsigned int gid);
    void ShowAllGroups() const;
    void ShowGroup(unsigned int gid);
    void AssignUserToGroup(unsigned int uid, unsigned int gid);
    void RemoveUserFromGroup(unsigned int uid);
    bool UserExists(unsigned int uid) const;
    bool GroupExists(unsigned int gid) const;

private:
    std::unordered_map<unsigned int, Group> groups_;
};

#endif // OBSERVER_H

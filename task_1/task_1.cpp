#include "Observer.h"
#include "Group.h"
#include "User.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>

bool IsValidNumber(const std::string& s) {
    return std::all_of(s.begin(), s.end(), ::isdigit) &&
           std::stoul(s) <= std::numeric_limits<unsigned int>::max();
}

int main() {
    Observer obs;
    std::string input, token;
    std::map<std::string, int> cmd_args = {
        {"addUser", 2}, {"removeUser", 1}, {"showAllUsers", 0}, {"showUser", 1},
        {"addGroup", 1}, {"removeGroup", 1}, {"showAllGroups", 0}, {"showGroup", 1},
        {"assignUserToGroup", 2}, {"removeUserFromGroup", 1}
    };

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::vector<std::string> parts;
        while (getline(iss, token, ' ')) {
            parts.push_back(token);
        }
        if (parts.empty() || !cmd_args.count(parts[0]) || (parts.size() - 1) < cmd_args[parts[0]]) {
            std::cerr << "Invalid command" << std::endl;
            continue;
        }
        if (cmd_args[parts[0]] > 0 && !IsValidNumber(parts[1])) {
            std::cerr << "Invalid id" << std::endl;
            continue;
        }
        if (parts[0] == "addUser") {
            obs.AddUser(std::stoul(parts[1]), parts[2]);
        } else if (parts[0] == "removeUser") {
            obs.RemoveUser(std::stoul(parts[1]));
        } else if (parts[0] == "showAllUsers") {
            obs.ShowAllUsers();
        } else if (parts[0] == "showUser") {
            obs.ShowUser(std::stoul(parts[1]));
        } else if (parts[0] == "addGroup") {
            obs.AddGroup(std::stoul(parts[1]));
        } else if (parts[0] == "removeGroup") {
            obs.RemoveGroup(std::stoul(parts[1]));
        } else if (parts[0] == "showAllGroups") {
            obs.ShowAllGroups();
        } else if (parts[0] == "showGroup") {
            obs.ShowGroup(std::stoul(parts[1]));
        } else if (parts[0] == "assignUserToGroup") {
            if (!IsValidNumber(parts[2])) {
                std::cerr << "Invalid id" << std::endl;
                continue;
            }
            obs.AssignUserToGroup(std::stoul(parts[1]), std::stoul(parts[2]));
        } else if (parts[0] == "removeUserFromGroup") {
            obs.RemoveUserFromGroup(std::stoul(parts[1]));
        } else {
            std::cerr << "Unknown command" << std::endl;
        }
    }
}

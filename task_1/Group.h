#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <unordered_map>
#include <memory>

class User;

class Group {
public:
    friend std::ostream& operator<<(std::ostream& os, const Group& g) {
        if (g.group_id_ != 0)
            os << "Group ID: " << g.group_id_ << "\n";
        else
            os << "Ungrouped Users:\n";
        g.DisplayUsers(true);
        return os;
    }

    Group() = default;
    explicit Group(unsigned int gid) : group_id_(gid) {}
    ~Group() = default;
    Group(const Group&) = default;
    Group& operator=(const Group&) = default;

    bool HasUser(unsigned int uid) const;
    void InsertUser(std::shared_ptr<User>& u);
    void DeleteUser(unsigned int uid);
    std::shared_ptr<User>& FetchUser(unsigned int uid);
    unsigned int GetId() const { return group_id_; }
    void DisplayUsers(bool tabs = false) const;
    void TransferUsers(Group& dest);

private:
    unsigned int group_id_{};
    std::unordered_map<unsigned int, std::shared_ptr<User>> user_map_;
};

#endif // GROUP_H

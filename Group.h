//
// Created by Max on 05/05/2023.
//

#ifndef WET_1_GROUP_H
#define WET_1_GROUP_H

#include "wet1util.h"
#include "User.h"
#include "Node.h"
#include <memory>

//TODO:consider inheritance from User
class Group {
public:
    // Constructor
    explicit Group(int groupId);
    //Destructor
    ~Group();
    bool isVipGroup() const;
    int getTotalViews() const;
    int getId() const;
    int getSize() const;
    Genre getFavoriteGenre() const;
    void updateViewsSolo(Genre genre);
    void updateViews(const int* newMemberViewsByGenre);
    void removeUserViews(int* viewsByGenre);
    void updateGroupViews(Genre genre);
    int getViewsByGenre(Genre genre);
    int* getViewsByGenre();

    void insertUser(const std::shared_ptr<User>& user);
    void removeUser(int userId);
    void updateUsersBeforeDelete();


private:
    int groupId;
    bool isVip;
    int numVipUsers;
    int size;
    int groupViews;
    int groupViewsByGenre[4];
    int soloViewsByGenre[4];
    Node<int, std::shared_ptr<User>>* users;
    //rewrite node to unique ptr
//    std::unique_ptr<Node<int, std::shared_ptr<User>>> users;



};


#endif //WET_1_GROUP_H

//
// Created by Max on 05/05/2023.
//

#ifndef WET_1_GROUP_H
#define WET_1_GROUP_H

#include "wet1util.h"
#include "User.h"
#include "Node.h"
#include <memory>

using std::shared_ptr;

class Group {
public:
    /*
     * Constructors
     */
    explicit Group(int groupId);
    ~Group();
    Group(const Group&) = delete;
    Group& operator=(const Group&) = delete;
    /*
     * Getters
     */
    bool isVipGroup() const;
    int getTotalGroupViews() const;
    int getId() const;
    int getSize() const;
    Genre getFavoriteGenre() const;
    int getCounterByGenre(Genre genre) const;
    int* getCounterByGenre();

    /*
     * Setters
     */
    void soloWatch(Genre genre);
    void updateViews(const int* newMemberViewsByGenre);
    void removeUserViews(const shared_ptr<User>& user);
    void groupWatch(Genre genre);
    void insertUser(const shared_ptr<User>& user);
    void removeUser(int userId);
    void updateUsersBeforeDelete();

private:
    int m_groupId;
    bool m_isVip;
    int m_numVipUsers;
    int m_size;
    //counter by genre
    int m_groupViewsCounter[4];
    //total views by genre
    int m_viewsByGenre[4];
    int m_soloViewsByGenre[4];
    Node<int, shared_ptr<User>>* m_users;
};

#endif //WET_1_GROUP_H


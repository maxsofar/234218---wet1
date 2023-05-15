//
// Created by Max on 05/05/2023.
//

#ifndef WET_1_USER_H
#define WET_1_USER_H

#include "wet1util.h"

class User {
public:
    /*
     * Constructors
     */
    User(int userId, bool isVip);
    ~User() = default;
    User(const User&) = delete;
    User& operator=(const User&) = delete;
    /*
     * Getters
     */
    bool isVipUser() const;
    int getGroupId() const;
    int getViews() const;
    int getViewsByGenre(Genre genre) const;
    int* getViewsByGenre();
    bool isInGroup() const;
    int getId() const;
    int getTotalViewsBeforeJoined() const;
    int getViewsBeforeJoined(Genre genre) const;
    /*
     * Setters
     */
    void watchMovie(Genre genre);
    void assignGroup(int groupId, const int* groupViewsByGenre, int groupViews);
    void updateViewsAfterGroupDelete(const int* groupViewsByGenre, int groupViews);

private:
    int m_userId;
    int m_groupId;
    bool m_isVip;
    bool m_isInGroup;
    /*
     * The following fields hold the user's solo views.
     */
    int m_views;
    int m_viewsByGenre[4];
    /*
     * The following fields hold the views of the group the user is in before he joined the group.
     * When the group is deleted, the user's views are updated according to these fields.
     */
    int m_totalViewsBeforeJoined;
    int m_groupViewsBeforeJoined[4];
};


#endif //WET_1_USER_H

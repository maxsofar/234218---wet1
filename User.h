//
// Created by Max on 05/05/2023.
//

#ifndef WET_1_USER_H
#define WET_1_USER_H

#include "wet1util.h"
//#include "Group.h"



class User {
public:
    // Constructor
    User(int userId, bool isVip);
    //Destructor
    ~User() = default;
    bool isVipUser() const;
    int getGroupId() const;
    int getTotalViews() const;
    int getViewsByGenre(Genre genre) const;
    int* getViewsByGenre();
    bool isInGroup() const;
    void watchMovie(Genre genre);
    void setInGroup(int id, const int* groupViewsByGenre, int groupViews);
    int getViewsBeforeJoined();
    int getViewsBeforeJoinedByGenre(Genre genre);
    void updateViewsAfterGroupDelete(int* groupViewsByGenre, int groupViews);

    const int getId();

private:
    int userId;
    int groupId;
    bool isVip;
    int totalViews;
    int totalViewsBeforeJoined;
    int viewsByGenre[4];
    int groupViewsBeforeJoined[4];
    //TODO: consider deleting and using  groupId == 0 instead
    bool inGroup;
};


#endif //WET_1_USER_H

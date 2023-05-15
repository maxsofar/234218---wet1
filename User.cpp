//
// Created by Max on 05/05/2023.
//

#include "User.h"


User::User(int userId, bool isVip) : userId(userId), groupId(0), isVip(isVip), totalViews(0), totalViewsBeforeJoined(0),
                                    viewsByGenre{0}, groupViewsBeforeJoined{0}, inGroup(false) {}

bool User::isVipUser() const
{
    return isVip;
}

int User::getTotalViews() const
{
    return totalViews;
}

int User::getViewsByGenre(Genre genre) const
{
    return viewsByGenre[static_cast<int>(genre)];
}

bool User::isInGroup() const
{
    return inGroup;
}


void User::watchMovie(Genre genre)
{
    totalViews++;
    viewsByGenre[static_cast<int>(genre)]++;
}

void User::setInGroup(int id, const int* groupViewsByGenre, int groupViews)
{
    this->groupId = id;
    this->totalViewsBeforeJoined = groupViews;
    if (groupViewsByGenre == nullptr) {
        for (int i = 0; i < 4; ++i) {
                    this->groupViewsBeforeJoined[i] = 0;
        }
        inGroup = false;
    } else {
        for (int i = 0; i < 4; ++i) {
            this->groupViewsBeforeJoined[i] = groupViewsByGenre[i];
        }
        inGroup = true;
    }

}

//{
//    this->groupId = group->getId();
//    this->totalViewsBeforeJoined = group->getTotalViews();
//    for (int i = 0; i < 4; ++i) {
//        this->groupViewsBeforeJoined[i] = group->getViewsByGenre(static_cast<Genre>(i));
//    }
//    inGroup = true;
//}

int* User::getViewsByGenre()
{
    return viewsByGenre;
}

int User::getGroupId() const
{
    return groupId;
}

int User::getViewsBeforeJoined()
{
    return totalViewsBeforeJoined;
}

int User::getViewsBeforeJoinedByGenre(Genre genre)
{
    return groupViewsBeforeJoined[static_cast<int>(genre)];
}

const int User::getId()
{
    return userId;
}

void User::updateViewsAfterGroupDelete(int* groupViewsByGenre, int groupViews)
{
    for (int i = 0; i < 4; ++i) {
        this->viewsByGenre[i] += groupViewsByGenre[i] - this->groupViewsBeforeJoined[i];
    }

    this->totalViews += groupViews - this->totalViewsBeforeJoined;
}


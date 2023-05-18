//
// Created by Max on 05/05/2023.
//

#include "User.h"

User::User(int userId, bool isVip) : m_userId(userId), m_groupId(0), m_isVip(isVip), m_isInGroup(false), m_views(0),
m_viewsByGenre{0}, m_totalGroupViewsBeforeJoined(0), m_groupCounterBeforeJoined{0}, m_groupViewsBeforeJoined{0},
m_groupSizeBeforeJoined(0) {}

bool User::isVipUser() const
{
    return m_isVip;
}

int User::getGroupId() const
{
    return m_groupId;
}

int User::getViews() const
{
    return m_views;
}

int User::getViewsByGenre(Genre genre) const
{
    return m_viewsByGenre[static_cast<int>(genre)];
}

bool User::isInGroup() const
{
    return m_isInGroup;
}

int User::getId() const
{
    return m_userId;
}

void User::watchMovie(Genre genre)
{
    m_views++;
    m_viewsByGenre[static_cast<int>(genre)]++;
}

void User::assignGroup(int groupId, const int* groupViewsCounter, int groupSize)
{
    m_groupId = groupId;
    m_groupSizeBeforeJoined = groupSize;

    if (groupViewsCounter == nullptr) {
        for (int &i: this->m_groupCounterBeforeJoined)
            i = 0;
        m_isInGroup = false;
        m_totalGroupViewsBeforeJoined = 0;
    } else {
        for (int i = 0; i < 4; ++i) {
            m_groupCounterBeforeJoined[i] = groupViewsCounter[i];
            m_totalGroupViewsBeforeJoined += groupViewsCounter[i];
        }
        m_isInGroup = true;
    }
}

int* User::getViewsByGenre()
{
    return m_viewsByGenre;
}

void User::updateViewsAfterGroupDelete(const int* groupViewsCounter)
{
    for (int i = 0; i < 4; ++i) {
        m_viewsByGenre[i] += groupViewsCounter[i] - m_groupCounterBeforeJoined[i];
        m_views += groupViewsCounter[i] - m_groupCounterBeforeJoined[i];
    }

}

int *User::getGroupViewsBeforeJoined()
{
    return m_groupViewsBeforeJoined;
}

int User::getGroupSizeBeforeJoined() const
{
    return m_groupSizeBeforeJoined;
}

int User::getGroupCounterBeforeJoined(Genre genre) const
{
    return m_groupCounterBeforeJoined[static_cast<int>(genre)];
}

int User::getTotalGroupViewsBeforeJoined() const
{
    return m_totalGroupViewsBeforeJoined;
}


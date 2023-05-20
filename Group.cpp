//
// Created by Max on 05/05/2023.
//

#include "Group.h"

using userSharedPtr = std::shared_ptr<User>;

Group::Group(int groupId) : m_groupId(groupId), m_isVip(false), m_numVipUsers(0), m_size(0), m_groupViewsCounter{0}, m_viewsByGenre{0},
                            m_soloViewsByGenre{0}{}

bool Group::isVipGroup() const
{
    return m_isVip;
}

int Group::getTotalGroupViews() const
{
    int totalViews = 0;
    for (int i : m_groupViewsCounter) {
        totalViews += i;
    }

    return totalViews;
}

int Group::getId() const
{
    return m_groupId;
}

int Group::getSize() const
{
    return m_size;
}

Genre Group::getFavoriteGenre() const
{
    auto favoriteGenre = static_cast<Genre>(0);
    int maxViews = 0;
    for (int i = 0; i < 4; ++i) {
        int totalViews = m_viewsByGenre[i] + m_soloViewsByGenre[i];
        if (totalViews > maxViews) {
            maxViews = m_viewsByGenre[i] + m_soloViewsByGenre[i];
            favoriteGenre = static_cast<Genre>(i);
        }
    }

    return favoriteGenre;
}

void Group::soloWatch(Genre genre)
{
    m_soloViewsByGenre[static_cast<int>(genre)]++;
}

void Group::updateViews(const int* newMemberViewsByGenre)
{
    for (int i = 0; i < 4; ++i) {
        m_soloViewsByGenre[i] += newMemberViewsByGenre[i];
    }
}

void Group::removeUserViews(const userSharedPtr& user)
{
    int* viewsByGenre = user->getViewsByGenre();
    for (int i = 0; i < 4; ++i) {
        m_soloViewsByGenre[i] -= viewsByGenre[i];
        int viewsGained = m_groupViewsCounter[i] - user->getGroupCounterBeforeJoined(static_cast<Genre>(i));
        m_viewsByGenre[i] -= viewsGained;
    }
}

void Group::groupWatch(Genre genre)
{
    m_groupViewsCounter[static_cast<int>(genre)]++;
    m_viewsByGenre[static_cast<int>(genre)] += m_size;
}

void Group::insertUser(const std::shared_ptr<User>& user)
{
    int userId = user->getId();
    m_users.insert(userId, user);

    if (user->isVipUser()) {
        m_isVip = true;
        m_numVipUsers++;
    }
    m_size++;
}

void Group::removeUser(int userId) {
    Node<int, userSharedPtr>* userNode = m_users.find(userId, m_users.getRoot());
    if (userNode->getValue()->isVipUser()) {
        m_numVipUsers--;
        if (m_numVipUsers == 0) {
            m_isVip = false;
        }
    }
    m_users.remove(userId);
    m_size--;
}

void Group::updateUsersBeforeDelete()
{
    m_users.updateUsersBeforeDelete(m_users.getRoot(), m_groupViewsCounter);
}

int Group::getCounterByGenre(Genre genre) const
{
    return m_groupViewsCounter[static_cast<int>(genre)];
}

int *Group::getCounterByGenre()
{
    return m_groupViewsCounter;
}


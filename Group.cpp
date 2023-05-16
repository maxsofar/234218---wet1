//
// Created by Max on 05/05/2023.
//

#include "Group.h"

Group::Group(int groupId) : m_groupId(groupId), m_isVip(false), m_numVipUsers(0), m_size(0), m_views(0), m_viewsByGenre{0},
                            m_soloViewsByGenre{0}, m_users{nullptr}{}

Group::~Group()
{
    Node<int, std::shared_ptr<User>>* curr = m_users;
    while (curr != nullptr) {
        Node<int, std::shared_ptr<User>>* temp = curr;
        curr = curr->getRight();
        delete temp;
    }
}

bool Group::isVipGroup() const
{
    return m_isVip;
}

int Group::getTotalViews() const
{
    return m_views;
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
    Genre favoriteGenre = Genre::NONE;
    int maxViews = 0;
    for (int i = 0; i < 4; ++i) {
        int totalViews = m_viewsByGenre[i] + m_soloViewsByGenre[i];
        if (totalViews > maxViews) {
            maxViews = m_viewsByGenre[i] + m_soloViewsByGenre[i];
            favoriteGenre = static_cast<Genre>(i);
        }
        else if (totalViews == maxViews)
            favoriteGenre = static_cast<Genre>(0);
    }

    return favoriteGenre;
}

int Group::getViewsByGenre(Genre genre) const
{
    return m_viewsByGenre[static_cast<int>(genre)];
}

int* Group::getViewsByGenre()
{
    return m_viewsByGenre;
}

void Group::updateViewsSolo(Genre genre)
{
    m_soloViewsByGenre[static_cast<int>(genre)]++;
}

void Group::updateViews(const int* newMemberViewsByGenre)
{
    m_size++;
    for (int i = 0; i < 4; ++i) {
        m_soloViewsByGenre[i] += newMemberViewsByGenre[i];
    }
}

void Group::removeUserViews(int* viewsByGenre)
{
    for (int i = 0; i < 4; ++i) {
        m_soloViewsByGenre[i] -= viewsByGenre[i];
    }
}

void Group::updateGroupViews(Genre genre)
{
    m_views++;
    m_viewsByGenre[static_cast<int>(genre)]++;
}

void Group::insertUser(const std::shared_ptr<User>& user)
{
    if (m_users == nullptr) {
        m_users = new Node<int, std::shared_ptr<User>>(user->getId(), user);
    } else {
        Node<int, std::shared_ptr<User>>* curr = m_users;
        while (curr->getRight() != nullptr) {
            curr = curr->getRight();
        }
        curr->setRight(new Node<int, std::shared_ptr<User>>(user->getId(), user));
    }

    if (user->isVipUser()) {
        m_isVip = true;
        m_numVipUsers++;
    }
}

void Group::removeUser(int userId)
{
    Node<int, std::shared_ptr<User>>* curr = m_users;
    Node<int, std::shared_ptr<User>>* prev = nullptr;

    while (curr != nullptr) {
        if (curr->getKey() == userId) {
            if (curr->getValue()->isVipUser()) {
                m_numVipUsers--;
                if (m_numVipUsers == 0) {
                    m_isVip = false;
                }
            }

            if (prev == nullptr) {
                m_users = curr->getRight();
            } else {
                prev->setRight(curr->getRight());
            }
            delete curr;
            m_size--;
            break;
        }
        prev = curr;
        curr = curr->getRight();
    }
}

void Group::updateUsersBeforeDelete()
{
    Node<int, std::shared_ptr<User>>* curr = m_users;
    while (curr != nullptr) {
        curr->getValue()->updateViewsAfterGroupDelete(m_viewsByGenre, m_views);
        curr->getValue()->assignGroup(0, nullptr, 0);
        curr = curr->getRight();
    }
}


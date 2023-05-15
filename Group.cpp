//
// Created by Max on 05/05/2023.
//

#include "Group.h"

Group::Group(int groupId) : groupId(groupId), isVip(false), numVipUsers(0), size(0), groupViews(0), groupViewsByGenre{0},
                            soloViewsByGenre{0}, users{nullptr}{}

int Group::getSize() const
{
    return size;
}

bool Group::isVipGroup() const
{
    return isVip;
}

int Group::getId() const
{
    return groupId;
}


void Group::updateViewsSolo(Genre genre)
{
    soloViewsByGenre[static_cast<int>(genre)]++;
}

void Group::updateViews(const int* newMemberViewsByGenre)
{
    size++;
    for (int i = 0; i < 4; ++i) {
        soloViewsByGenre[i] += newMemberViewsByGenre[i];
    }
}

void Group::removeUserViews(int* viewsByGenre)
{
    for (int i = 0; i < 4; ++i) {
        soloViewsByGenre[i] -= viewsByGenre[i];
    }
    size--;
}


void Group::updateGroupViews(Genre genre)
{
    groupViews++;
    groupViewsByGenre[static_cast<int>(genre)]++;
}

Genre Group::getFavoriteGenre() const
{
    Genre favoriteGenre = Genre::NONE;
    int maxViews = 0;
    for (int i = 0; i < 4; ++i) {
        int totalViews = groupViewsByGenre[i] +  soloViewsByGenre[i];
        if (totalViews > maxViews) {
            maxViews = groupViewsByGenre[i];
            favoriteGenre = static_cast<Genre>(i);
        }
        else if (totalViews == maxViews)
            favoriteGenre = static_cast<Genre>(0);
//        if (groupViewsByGenre[i] +  soloViewsByGenre[i] > maxViews) {
//            maxViews = groupViewsByGenre[i];
//            favoriteGenre = static_cast<Genre>(i);
//        }
    }

    return favoriteGenre;
}

int Group::getTotalViews() const
{
    return groupViews;
}

int Group::getViewsByGenre(Genre genre)
{
    return groupViewsByGenre[static_cast<int>(genre)];
}

int* Group::getViewsByGenre()
{
    return groupViewsByGenre;
}

void Group::insertUser(User *user)
{
    if (users == nullptr) {
        users = new Node<int, User*>(user->getId(), user);
    } else {
        Node<int, User*>* curr = users;
        while (curr->getRight() != nullptr) {
            curr = curr->getRight();
        }
        curr->setRight(new Node<int, User*>(user->getId(), user));
    }
    if (user->isVipUser()) {
        isVip = true;
        numVipUsers++;
    }
}

void Group::removeUser(int userId)
{
    Node<int, User*>* curr = users;
    Node<int, User*>* prev = nullptr;
    while (curr != nullptr) {
        if (curr->getKey() == userId) {
            if (curr->getValue()->isVipUser()) {
                numVipUsers--;
                if (numVipUsers == 0) {
                    isVip = false;
                }
            }

            if (prev == nullptr) {
                users = curr->getRight();
            } else {
                prev->setRight(curr->getRight());
            }
            break;
        }
        prev = curr;
        curr = curr->getRight();
    }


}

void Group::updateUsersBeforeDelete()
{
    Node<int, User*>* curr = users;
    while (curr != nullptr) {
        curr->getValue()->updateViewsAfterGroupDelete(groupViewsByGenre, groupViews);

        curr->getValue()->setInGroup(0, nullptr, 0);
        curr = curr->getRight();
    }
}

Group::~Group()
{
    Node<int, User*>* curr = users;
    while (curr != nullptr) {
        Node<int, User*>* temp = curr;
        curr = curr->getRight();
        delete temp;
    }
}



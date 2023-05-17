//
// Created by Max on 08/05/2023.
//

#include "Movie.h"

Movie::Movie(int movieId, Genre genre, int views, bool vipOnly) : m_movieId(movieId), m_views(views), m_rating(0),
m_vipOnly(vipOnly), m_genre(genre), m_numOfRatings(0){}

Genre Movie::getGenre() const
{
    return m_genre;
}

bool Movie::isVipOnly() const
{
    return m_vipOnly;
}

int Movie::getId() const
{
    return m_movieId;
}

void Movie::updateRating(int i)
{
    m_rating = (m_rating * m_numOfRatings + i) / (m_numOfRatings + 1);
    m_numOfRatings++;
}

void Movie::updateViews(int numOfViews)
{
    m_views += numOfViews;
}

//reversed comparison to use inorder traversal in BST
bool Movie::operator<(const Movie& other) const
{
    if (this->m_rating > other.m_rating)
        return true;
    else if (this->m_rating == other.m_rating)
    {
        if (this->m_views > other.m_views)
            return true;
        else if (this->m_views == other.m_views)
        {
            //moveId should be in ascending order
            if (this->m_movieId < other.m_movieId)
                return true;
        }
    }
    return false;
}

bool Movie::operator>(const Movie& other) const
{
    if (this->m_rating < other.m_rating)
        return true;
    else if (this->m_rating == other.m_rating)
    {
        if (this->m_views < other.m_views)
            return true;
        else if (this->m_views == other.m_views)
        {
            //moveId should be in ascending order
            if (this->m_movieId > other.m_movieId)
                return true;
        }
    }
    return false;
}

bool Movie::operator==(const Movie& other) const
{
    if (this->m_movieId == other.m_movieId)
        return true;
    return false;
}


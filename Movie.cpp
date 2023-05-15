//
// Created by Max on 08/05/2023.
//

#include "Movie.h"

Movie::Movie(int movieId, Genre genre, int views, bool vipOnly) : movieId(movieId), genre(genre), views(views),
                                                                 vipOnly(vipOnly), rating(0) {}

bool Movie::isVipOnly() const
{
    return vipOnly;
}

//TODO: how to round the number?
void Movie::updateRating(int i)
{
    if (rating == 0)
        rating = i;
    else
        rating = (rating + i) / 2;
}

Genre Movie::getGenre() const
{
    return genre;
}

//reversed comparison to perform the descending order
//TODO: check if the comparison is correct or change the in-order order
bool Movie::operator<(const Movie& other) const
{
    if (this->rating > other.rating)
        return true;
    else if (this->rating == other.rating)
    {
        if (this->views > other.views)
            return true;
        else if (this->views == other.views)
        {
            //moveId should be in ascending order
            if (this->movieId < other.movieId)
                return true;
        }
    }
    return false;
}

bool Movie::operator>(const Movie& other) const
{
    if (this->rating < other.rating)
        return true;
    else if (this->rating == other.rating)
    {
        if (this->views < other.views)
            return true;
        else if (this->views == other.views)
        {
            //moveId should be in ascending order
            if (this->movieId > other.movieId)
                return true;
        }
    }
    return false;
}

void Movie::updateViews(int numOfViews)
{
    views += numOfViews;
}

bool Movie::operator==(const Movie& other) const
{
    if (this->movieId == other.movieId)
        return true;
    return false;
}

int Movie::getId() const
{
    return movieId;
}


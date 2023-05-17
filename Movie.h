//
// Created by Max on 08/05/2023.
//

#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H

#include "wet1util.h"

class Movie {
public:
    /*
     * Constructors
     */
    Movie(int movieId, Genre genre, int views, bool vipOnly);
    ~Movie() = default;
    Movie(const Movie&) = default;
    Movie& operator=(const Movie&) = default;
    /*
     * Getters
     */
    Genre getGenre() const;
    bool isVipOnly() const;
    int getId() const;
    /*
     * Setters
     */
    void updateRating(int i);
    void updateViews(int numOfViews);
    /*
     * Overloaded operators
     */
    bool operator<(const Movie& other) const;
    bool operator>(const Movie& other) const;
    bool operator==(const Movie& other) const;

private:
    int m_movieId;
    int m_views;
    int m_rating;
    bool m_vipOnly;
    Genre m_genre;
    int m_numOfRatings;
};

#endif //WET1_MOVIE_H


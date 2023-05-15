//
// Created by Max on 08/05/2023.
//

#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H

#include "wet1util.h"
#include "wet1util.h"

class Movie {
public:
    // Constructor
    Movie(int movieId, Genre genre, int views, bool vipOnly);
    // Destructor
    ~Movie() = default;
    Genre getGenre() const;
    bool isVipOnly() const;
    void updateRating(int i);
    bool operator<(const Movie& other) const;
    bool operator>(const Movie& other) const;
    bool operator==(const Movie& other) const;
    void updateViews(int numOfViews);

private:
    int movieId;
    Genre genre;
    int views;
    bool vipOnly;
    int rating;
};


#endif //WET1_MOVIE_H

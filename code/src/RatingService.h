#ifndef RATING_SERVICE_H
#define RATING_SERVICE_H

#include <string>
#include "User.h"

class RatingService {
public:
    // Calculates average rating for a movie
    static double getAverageRating(int movieCode, int& count);

    // Adds or updates rating for a user
    static void giveOrUpdateRating(int movieCode, const User& user);
};

#endif

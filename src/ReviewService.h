#ifndef REVIEW_SERVICE_H
#define REVIEW_SERVICE_H

#include "User.h"

class ReviewService {
public:
    // Display all reviews for a movie
    static void showReviews(int movieCode);

    // User: add or update own review
    static void addOrUpdateReview(int movieCode, const User& user);

    // User: delete own review
    static void deleteOwnReview(int movieCode, const User& user);

    // Admin: delete any review
    static void deleteAnyReview(int movieCode);
};

#endif

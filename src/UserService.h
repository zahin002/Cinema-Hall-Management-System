#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "User.h"

class UserService {
public:
    void userMenu(const User& user);
    void viewMovies();
    void showMovieDetails(int movieCode);
    void showTrendingMovies();
    void showMovieDetails(int movieCode, const User& user);
    void showGenreRecommendations(int currentMovieCode, const string& genre);
    void giveOrUpdateRating(int movieCode, const User& user);
     void addOrUpdateReview(int movieCode, const User& user);

    void filterMovies();
    void searchMovieByName();
    void viewShowtimes();
    void bookSeat(const User& user);
    void recommendSeat();
    static void cancelTicket();
private:
    double getAverageRating(int movieCode, int& count);
    void showReviews(int movieCode);

};

#endif

#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H
#include <string>
#include "User.h"

class MenuService {
public:
    // Displays the main user menu (user / admin / guest)
    void showUserMenu(const User& user);

private:
    // Movie browsing related sub-menus
    void browseMoviesMenu(const User& user);

    // Allows user to select a movie to view detailed information
    void selectMovieForDetails(const User& user);
};

#endif

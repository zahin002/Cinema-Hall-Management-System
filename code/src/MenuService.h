#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H
#include <string>
#include "User.h"

class MenuService {
public:
    // Displays the main user menu (user / admin / guest)
    static void showUserMenu(const User& user);

    // Movie browsing related sub-menus
    static void browseMoviesMenu(const User& user, bool isAdmin);
    
    // Allows user to select a movie to view detailed information
    static void selectMovieForDetails(const User& user);
};

#endif

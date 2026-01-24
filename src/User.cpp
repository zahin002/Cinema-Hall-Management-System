#include "User.h"

/*
 * User
 * ----
 * Represents a system user in the CINE++ application.
 * Stores authentication credentials and role information.
 *
 * Timeline:
 * - Week 1: User authentication and basic security handling
 */

/*
 * Week 1:
 * Default constructor.
 * Allows creation of empty User objects during
 * file loading or container operations.
 */
User::User() {}

/*
 * Week 1:
 * Parameterized constructor.
 * Initializes user credentials and role at creation time.
 */
User::User(string email, string password, string role) {
    this->email = email;
    this->password = password;
    this->role = role;
}

/*
 * Week 1:
 * Returns the user's email.
 * Used as the primary identifier during login.
 */
string User::getEmail() const {
    return email;
}

/*
 * Week 1:
 * Returns the encrypted password.
 * Used only for comparison during authentication.
 */
string User::getPassword() const {
    return password;
}

/*
 * Week 1:
 * Returns the user's role (e.g., admin or user).
 * Determines accessible menus and permissions.
 */
string User::getRole() const {
    return role;
}

/*
 * Week 1:
 * Performs a very simple character-based encryption.
 * Each character is shifted forward by 2 positions.
 * This avoids storing plain-text passwords.
 */
string User::encryptPassword(const string& password) {
    string encrypted = password;
    for (char &c : encrypted) {
        c = c + 2;
    }
    return encrypted;
}

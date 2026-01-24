#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

/*
 * User
 * ----
 * Data model representing a system user in CINE++.
 * Stores authentication credentials and role information.
 *
 * Timeline:
 * - Week 1: User authentication and role-based access
 */
class User {
private:
    /*
     * Week 1:
     * Email address of the user.
     * Acts as the unique identifier during login.
     */
    string email;

    /*
     * Week 1:
     * Encrypted password.
     * Plain-text passwords are never stored.
     */
    string password; // stored encrypted

    /*
     * Week 1:
     * Role of the user.
     * Determines accessible features in the system.
     */
    string role;     // "admin" or "user"

public:
    /*
     * Week 1:
     * Default constructor.
     * Required for object creation during file loading.
     */
    User();

    /*
     * Week 1:
     * Parameterized constructor.
     * Initializes email, encrypted password, and role.
     */
    User(string email, string password, string role);

    /*
     * Week 1:
     * Returns the user's email.
     */
    string getEmail() const;

    /*
     * Week 1:
     * Returns the encrypted password.
     * Used only for authentication comparison.
     */
    string getPassword() const;

    /*
     * Week 1:
     * Returns the user's role.
     * Controls menu access and permissions.
     */
    string getRole() const;

    /*
     * Week 1:
     * Encrypts a password using a simple character shift.
     * Ensures passwords are not stored in plain text.
     */
    static string encryptPassword(const string& password);
};

#endif

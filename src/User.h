#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
private:

    string email;
    string password; // stored encrypted
    string role;     // "admin" or "user"

public:

    User();

    User(string email, string password, string role);

    string getEmail() const;

    string getPassword() const;

    string getRole() const;

    /*
     Week 1:
     Encrypts a password using a simple character shift.
     Ensures passwords are not stored in plain text. */
    static string encryptPassword(const string& password);

};

#endif

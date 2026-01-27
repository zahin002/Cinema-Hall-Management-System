#include "User.h"


User::User() {}

User::User(string email, string password, string role) {
    this->email = email;
    this->password = password;
    this->role = role;
}

string User::getEmail() const {
    return email;
}


string User::getPassword() const {
    return password;
}

string User::getRole() const {
    return role;
}

/*
 * Performs a very simple character-based encryption.
 * Each character is shifted forward by 2 positions. */

string User::encryptPassword(const string& password) {
    string encrypted = password;
    for (char &c : encrypted) {
        c = c + 2;
    }
    return encrypted;
}
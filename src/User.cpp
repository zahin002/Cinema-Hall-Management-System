#include "User.h"
#include <functional>   // for std::hash
#include <string>

using namespace std;

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
 * Password hashing using std::hash
 * Ensures passwords are not reversible
 */ 

string User::encryptPassword(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}





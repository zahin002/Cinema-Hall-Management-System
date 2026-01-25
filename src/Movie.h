#ifndef MOVIE_H
#define MOVIE_H

#include <string>
using namespace std;

class Movie {
private:
    int code;              // unique movie code (e.g. 101)
    string title;
    string genre;
    int duration;
    string language;

public:
    Movie();
    Movie(int code, string title, string genre, int duration, string language);

    int getCode() const;
    string getTitle() const;
    string getGenre() const;
    int getDuration() const;
    string getLanguage() const;
};

#endif

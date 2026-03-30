#include "Movie.h"

Movie::Movie() {}

Movie::Movie(int code, string title, string genre, int duration, string language) {
    this->code = code;
    this->title = title;
    this->genre = genre;
    this->duration = duration;
    this->language = language;
}

int Movie::getCode() const { 
    return code; 
}

string Movie::getTitle() const { 
    return title; 
}

string Movie::getGenre() const { 
    return genre; 
}

int Movie::getDuration() const { 
    return duration; 
}

string Movie::getLanguage() const { 
    return language; 
}

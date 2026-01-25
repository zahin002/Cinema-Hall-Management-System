#include "Showtime.h"

Showtime::Showtime() {}

Showtime::Showtime(int movieCode, string date, string time, int hallNo) {
    this->movieCode = movieCode;
    this->date = date;
    this->time = time;
    this->hallNo = hallNo;
}

int Showtime::getMovieCode() const {
    return movieCode;
}

string Showtime::getDate() const {
    return date;
}

string Showtime::getTime() const {
    return time;
}

int Showtime::getHallNo() const {
    return hallNo;
}

#ifndef SHOWTIME_H
#define SHOWTIME_H

#include <string>
using namespace std;

class Showtime {
private:
    int movieCode;        // references Movie.code
    string date;
    string time;
    int hallNo;

public:
    Showtime();
    Showtime(int movieCode, string date, string time, int hallNo);

    int getMovieCode() const;
    string getDate() const;
    string getTime() const;
    int getHallNo() const;
};

#endif
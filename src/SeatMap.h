#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>
using namespace std;

/*
 * SeatMap
 * -------
 * Represents the seating layout for a specific show.
 * Manages seat availability, booking operations,
 * and intelligent seat recommendation logic. */
class SeatMap {
private:
 
    int rows;
    int cols;

    /*
     * 2D grid representing seat status.
     * 'O' = available, 'X' = booked
     */

    vector<vector<char>> seats;

public:
    SeatMap(int rows = 0, int cols = 0);

    static SeatMap createForHall(int hallNo);

    void initialize();

    void display() const;

    bool bookSeat(int row, int col);

    int getRows() const;

    int getCols() const;

    vector<vector<char>> getSeats() const;

    void setSeats(const vector<vector<char>>& seats);

    bool isSeatAvailable(int row, int col) const;

    pair<int,int> recommendBestSeat() const;
    
    vector<pair<int,int>> recommendBestSeats_Custom(int count) const;
};

#endif

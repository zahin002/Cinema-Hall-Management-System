#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>
#include <utility>   

using namespace std;

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

    void releaseSeat(int row, int col);

    bool isSeatAvailable(int row, int col) const;

    int getRows() const;
    int getCols() const;

    vector<vector<char>> getSeats() const;
    void setSeats(const vector<vector<char>>& seats);

    pair<int,int> recommendBestSeat() const;
    vector<pair<int,int>> recommendBestSeats_Custom(int count) const;
};

#endif

#include "TicketService.h"
#include "TerminalColors.h"
#include "FileManager.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <ctime>
#include <cstdlib>

using namespace std;


string TicketService::generateTicketId() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }

    return "TCK" + to_string(time(nullptr)) + "_" + to_string(rand() % 1000);
}

void TicketService::printTicket(
    const string& ticketId,
    const string& customerName,
    const string& movieTitle,
    const string& showDate,
    const string& showTime,
    int hallNo,
    const vector<pair<int,int>>& selectedSeats,
    double basePrice,
    double discountTk,
    const string& discountLabel,
    double finalPrice
) {
    cout << "\n";
    cout << BOLD << CYAN
         << "===============================================\n"
         << RESET;

    cout << BOLD << YELLOW
         << "               🎟  CINE++ TICKET\n"
         << RESET;

    cout << BOLD << CYAN
         << "===============================================\n"
         << RESET;

    cout << left;
    cout << BOLD << setw(15) << "Ticket ID" << ": " << RESET << ticketId << endl;
    cout << BOLD << setw(15) << "Name"      << ": " << RESET << customerName << endl;
    cout << BOLD << setw(15) << "Movie"     << ": " << RESET << movieTitle << endl;

    cout << BOLD << setw(15) << "Showtime"  << ": " << RESET
         << showDate << " | " << showTime << endl;

    cout << BOLD << setw(15) << "Hall No"   << ": " << RESET << hallNo << endl;

    cout << BOLD << setw(15) << "Seats"     << ": " << RESET;
    for (auto& s : selectedSeats)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;

    cout << "-----------------------------------------------\n";
    cout << setw(15) << "Base Price" << ": " << basePrice << " Tk\n";

    if (discountTk > 0) {
        cout << setw(15) << discountLabel
             << ": -" << discountTk << " Tk\n";
    }

    cout << BOLD << RED
         << setw(15) << "Total Paid" << ": "
         << finalPrice << " Tk\n"
         << RESET;

    cout << BOLD << CYAN
         << "===============================================\n"
         << RESET;

    cout << GREEN
         << " 🎬  Enjoy Your Movie at CINE++  🎬\n"
         << RESET;

    cout << BOLD << CYAN
         << "===============================================\n"
         << RESET;
}

void TicketService::saveTicket(
    const string& ticketId,
    const string& customerName,
    const string& movieTitle,
    const string& showDate,
    const string& showTime,
    int hallNo,
    const vector<pair<int,int>>& selectedSeats,
    double finalPrice
) {
    stringstream record;

    record << ticketId << "|"
           << customerName << "|"
           << movieTitle << "|"
           << showDate << "|"
           << showTime << "|"
           << hallNo << "|";

    for (auto& s : selectedSeats)
        record << char('A' + s.first) << s.second + 1 << ",";

    record << "|" << finalPrice;

    FileManager::saveTicketToFile(record.str());
}


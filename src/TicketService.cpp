#include "TicketService.h"
#include "TerminalColors.h"
#include "FileManager.h"
#include "SeatMap.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;

/* ================= TICKET ID ================= */

string TicketService::generateTicketId() {
    static bool seeded = false;
    if (!seeded) {
        srand(::time(nullptr));
        seeded = true;
    }
    return "TCK" + to_string(::time(nullptr)) + "_" + to_string(rand() % 1000);
}

/* ================= EXISTENCE OF TICKET ID ================= */

bool TicketService::ticketExists(const string& ticketId) {
    ifstream file("tickets.txt"); 
    string line;

    while (getline(file, line)) {
        if (line.find(ticketId) != string::npos) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}


/* ================= PRINT TICKET ================= */

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
    cout << BOLD << CYAN << "===============================================\n" << RESET;
    cout << BOLD << YELLOW << "               🎟  CINE++ TICKET\n" << RESET;
    cout << BOLD << CYAN << "===============================================\n" << RESET;

    cout << left;
    cout << BOLD << setw(15) << "Ticket ID" << ": " << RESET << ticketId << endl;
    cout << BOLD << setw(15) << "Name" << ": " << RESET << customerName << endl;
    cout << BOLD << setw(15) << "Movie" << ": " << RESET << movieTitle << endl;
    cout << BOLD << setw(15) << "Showtime" << ": " << RESET
         << showDate << " | " << showTime << endl;
    cout << BOLD << setw(15) << "Hall No" << ": " << RESET << hallNo << endl;

    cout << BOLD << setw(15) << "Seats" << ": " << RESET;
    for (auto& s : selectedSeats)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;

    cout << "-----------------------------------------------\n";
    cout << setw(15) << "Base Price" << ": " << basePrice << " Tk\n";

    if (discountTk > 0)
        cout << setw(15) << discountLabel << ": -" << discountTk << " Tk\n";

    cout << BOLD << RED << setw(15) << "Total Paid" << ": "
         << finalPrice << " Tk\n" << RESET;

    cout << BOLD << CYAN << "===============================================\n" << RESET;
    cout << GREEN << " 🎬  Enjoy Your Movie at CINE++  🎬\n" << RESET;
    cout << BOLD << CYAN << "===============================================\n" << RESET;
}

/* ================= SAVE TICKET ================= */

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

/* ================= REFUND CALC ================= */

int TicketService::calculateRefundPercent(
    const string& showDate,
    const string& showTime
) {
    int year, month, day, hour, minute;
    char c;

    stringstream(showDate) >> year >> c >> month >> c >> day;
    stringstream(showTime) >> hour >> c >> minute;

    tm showTM{};
    showTM.tm_year = year - 1900;
    showTM.tm_mon = month - 1;
    showTM.tm_mday = day;
    showTM.tm_hour = hour;
    showTM.tm_min = minute;

    time_t showEpoch = mktime(&showTM);
    time_t nowEpoch = ::time(nullptr);

    double diffMin = difftime(showEpoch, nowEpoch) / 60.0;

    if (diffMin <= 0) return -1;
    if (diffMin >= 360) return 80;
    if (diffMin >= 120) return 50;
    if (diffMin >= 30) return 20;
    return -1;
}

/* ================= CANCEL TICKET ================= */

void TicketService::cancelTicket(const string& ticketId) {

    vector<string> tickets = FileManager::loadAllTickets();

    for (string& record : tickets) {

        stringstream ss(record);
        vector<string> parts;
        string temp;

        while (getline(ss, temp, '|'))
            parts.push_back(temp);

        if (parts.size() < 8) continue;
        if (parts[0] != ticketId) continue;

        string status = (parts.size() >= 9) ? parts[8] : "ACTIVE";
        if (status == "CANCELLED") {
            cout << "Ticket already cancelled.\n";
            return;
        }

        string name = parts[1];
        string movie = parts[2];
        string showDate = parts[3];
        string showTime = parts[4];
        int hallNo = stoi(parts[5]);
        string seatStr = parts[6];
        double paid = stod(parts[7]);

        vector<pair<int,int>> seats;
        stringstream ss2(seatStr);
        string s;
        while (getline(ss2, s, ',')) {
            if (s.empty()) continue;
            seats.push_back({s[0] - 'A', stoi(s.substr(1)) - 1});
        }

        SeatMap map = FileManager::loadOrCreateSeatMap(hallNo, showDate, showTime);
        for (auto& p : seats)
            map.releaseSeat(p.first, p.second);

        FileManager::saveSeatMap(
            FileManager::getSeatMapFilename(hallNo, showDate, showTime),
            map
        );

        int refundPercent = calculateRefundPercent(showDate, showTime);
        if (refundPercent == -1) {
            cout << BOLD << RED
                 << "❌ Cancellation not allowed (show started or <30 min).\n"
                 << RESET;
            return;
        }

        double refundAmt = paid * refundPercent / 100.0;
        record += "|CANCELLED";

        time_t now = ::time(nullptr);
        string refundTime = ctime(&now);
        refundTime.pop_back();

        stringstream refundLog;
        refundLog << ticketId << "|" << refundAmt << "|"
                  << refundPercent << "%|" << refundTime;

        FileManager::logRefund(refundLog.str());

        printCancelledTicket(
            ticketId, name, movie,
            showDate, showTime, hallNo,
            seats, paid, refundPercent, refundAmt
        );

        FileManager::overwriteAllTickets(tickets);
        return;
    }

    cout << "Invalid Ticket ID.\n";
}

/* ================= CANCELLED TICKET PRINT ================= */

void TicketService::printCancelledTicket(
    const string& ticketId,
    const string& customerName,
    const string& movieTitle,
    const string& showDate,
    const string& showTime,
    int hallNo,
    const vector<pair<int,int>>& seats,
    double paidAmount,
    int refundPercent,
    double refundAmount
) {
    cout << "\n";
    cout << BOLD << RED << "=====================================\n" << RESET;
    cout << BOLD << YELLOW << "     ❌ CINE++ CANCELLED TICKET\n" << RESET;
    cout << BOLD << RED << "=====================================\n" << RESET;

    cout << " Ticket ID   : " << ticketId << endl;
    cout << " Name        : " << customerName << endl;
    cout << " Movie       : " << movieTitle << endl;
    cout << " Showtime    : " << showDate << " | " << showTime << endl;
    cout << " Hall No     : " << hallNo << endl;

    cout << " Seats       : ";
    for (auto& s : seats)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;

    cout << " Paid Amount : " << paidAmount << " Tk\n";
    cout << " Refund Rule : " << refundPercent << "%\n";

    cout << BOLD << GREEN
         << " Refund Amt  : " << refundAmount << " Tk\n"
         << RESET;

    cout << BOLD << RED << " STATUS      : CANCELLED\n" << RESET;
    cout << BOLD << RED << "=====================================\n" << RESET;
}

# 🎬 CINE++
### Cinema Hall Management System

A Feature-Rich File-Based Cinema Hall Management System

CINE++ is a console-based cinema hall management system developed in C++, built using Object-Oriented Programming (OOP) and file-based persistence.

It simulates a real-world cinema ticketing system including booking, pricing, seat management, reviews, and refunds — all without using a database or GUI.

---

## 📂 Project Structure

```
CINE++/
│
├── data/
│   ├── movies.txt
│   ├── showtimes.txt
│   ├── users.txt
│   ├── tickets.txt
│   ├── ratings.txt
│   ├── reviews.txt
│   ├── refunds.txt
│   ├── guest_counter.txt
│   └── seats/
│       ├── show_1.txt
│       ├── show_2.txt
│       └── ...
│
├── src/
│   ├── main.cpp
│   ├── CinemaSystem.cpp / .h
│   ├── AdminService.cpp / .h
│   ├── UserService.cpp / .h
│   ├── MenuService.cpp / .h
│   ├── Movie.cpp / .h
│   ├── Showtime.cpp / .h
│   ├── SeatMap.cpp / .h
│   ├── TicketService.cpp / .h
│   ├── PricingEngine.cpp / .h
│   ├── ReviewService.cpp / .h
│   ├── RatingService.cpp / .h
│   ├── SnackShop.cpp / .h
│   ├── FileManager.cpp / .h
│   ├── User.cpp / .h
│   └── TerminalColors.h
│
├── cine (compiled executable)
├── README.md
└── .gitignore
```

## ▶️ How to Compile & Run

From the `src` directory:

`g++ *.cpp -o cine`

Run the program:

`./cine`

## 🚀 Core Features

### 👤 User System
- Signup & Login system  
- File-based credential storage  
- Role-based access (Admin / User / Guest)  
- Guest tracking system  
- Menu-driven console interface  

---

### 🎬 Movie & Showtime Management
- Add, remove, and view movies  
- Manage movie details:
  - Genre
  - Duration
  - Language  
- Assign showtimes to movies  
- Dynamic show filtering  

---

### 🪑 Seat Management System
- 2D seat grid (hall-based)  
- Real-time seat updates  
- File-based seat storage per show  
- Smart seat allocation logic  
- Group seat handling  

---

### 🎟️ Ticket Booking System
- Multi-seat booking  
- Ticket generation with:
  - Unique Ticket ID  
  - Booking details  
- Persistent ticket storage  
- Invoice-style output  

---

### 💰 Advanced Pricing Engine
- Dynamic pricing rules:
  - Time-based pricing (morning / weekend)  
  - Group discounts  
  - Global discount override system  
- Automatic bill calculation  
- Discount application logic  

---
### 🔁 Refund System
- Ticket cancellation functionality  
- Automatic seat release after cancellation  
- Refund record storage  

---

### ⭐ Review & Rating System
- Users can:
  - Rate movies  
  - Submit reviews  
- Stored in file-based system  
- Used for system insights and improvements  

---

### 🍿 Snack Shop Module
- Additional purchase system  
- Integrated with ticket booking flow  

---

### 🛠️ Admin Functionalities
- Manage movies and showtimes  
- Control seat data  
- View system data (users, bookings, etc.)  
- Monitor performance using stored data  

---

### ⚙️ System Highlights
- Fully file-based system (no database)  
- Modular architecture (service-based design)   
- Scalable and extendable system  

---


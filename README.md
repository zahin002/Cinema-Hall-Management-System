# 🎬 CINE++
### Cinema Hall Management System

CINE++ is a **console-based cinema hall management system** developed in **C++** using **Object-Oriented Programming (OOP)** and **file-based persistence**.  
The project is built incrementally as part of the **Software Project**, following a weekly development approach.

---

## 📂 Project Structure
Cinema-Hall-Management-System/
│
├── data/
│ ├── movies.txt
│ ├── showtimes.txt
│ ├── users.txt
│ └── seats/
│ ├── show_1.txt
│ ├── show_2.txt
│ └── show_3.txt
│
├── src/
│ ├── main.cpp
│ ├── CinemaSystem.cpp
│ ├── CinemaSystem.h
│ ├── AdminPanel.cpp
│ ├── AdminPanel.h
│ ├── UserPanel.cpp
│ ├── UserPanel.h
│ ├── Movie.cpp
│ ├── Movie.h
│ ├── Showtime.cpp
│ ├── Showtime.h
│ ├── SeatMap.cpp
│ ├── SeatMap.h
│ ├── FileManager.cpp
│ └── FileManager.h
│
├── README.md
└── .gitignore

## 🧠 Technologies & Concepts Used
- C++
- Object-Oriented Programming (OOP)
- File Handling (`fstream`)
- Modular Design
- Input Validation
- Menu-driven Console Application

---

## ✅ Implemented Features

## ⚙️ Core Functionalities

### 👨‍💼 Admin Functionalities
- Add new movies
- Remove existing movies
- View all available movies
- Manage showtimes
- Control seat availability data

---

### 👤 User Functionalities
- View available movies and showtimes
- Select seats for booking
- Book tickets
- Apply **discount-based ticket pricing**
- Generate and view **ticket invoices**

---

### 💰 Ticket Pricing & Invoice
- Supports **discount-based ticket pricing**
- Automatically calculates final ticket price after applying discounts
- Generates a **ticket invoice** 

---

## ▶️ How to Compile & Run

From the `src` directory:

`g++ main.cpp CinemaSystem.cpp AdminService.cpp UserService.cpp User.cpp FileManager.cpp Movie.cpp Showtime.cpp SeatMap.cpp PricingEngine.cpp -o cine`

Run the program:

`./cine`



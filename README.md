# 🎬 CINE++
### Cinema Hall Management System (SPL-1)

CINE++ is a **console-based cinema hall management system** developed in **C++** using **Object-Oriented Programming (OOP)** and **file-based persistence**.  
The project is built incrementally as part of the **Software Project**, following a weekly development approach.

---

## 🧠 Technologies & Concepts Used
- C++
- Object-Oriented Programming (OOP)
- File Handling (`fstream`)
- Modular Design
- Input Validation
- Menu-driven Console Application

---

## ✅ Implemented Features

### 🔐 Authentication System
- User signup and login
- Password encryption
- Role-based access (`admin`, `user`)
- File-based user storage

---

### 🎥 Movie Management (Admin)
- Add movies
- Auto-generated unique movie codes
- Prevent duplicate movie codes
- View movie list
- Safe movie deletion (blocked if showtimes exist)

---

### ⏰ Showtime Management (Admin)
- Add showtimes using movie codes
- Prevent showtime creation for non-existing movies
- View all showtimes
- Movie title resolved using movie code

---

### 💺 Seat Map Management
- Create seat maps per showtime
- View seat maps
- Persistent seat storage
- Updated seat map shown after booking

---

### 🎟 Seat Booking (User & Guest)
- Single and multiple seat booking
- Atomic booking (all-or-nothing)
- Seat availability validation
- Booking confirmation with updated seat map

---

### ⭐ Best Seat Recommendation
- Center-priority seat selection
- Group seat recommendation
- Row traversal strategy
- Fallback handling when seats unavailable

---

### 👤 Guest Mode
- Guest booking without account
- Phone-based login
- Bangladeshi phone number validation:
- Guest can book seats like regular users

---

### 🔎 Movie Filtering (User)
- Filter movies by **Genre**
- Filter movies by **Language**
- Combined filtering supported

---

## ▶️ How to Compile & Run

From the `src` directory:

`g++ main.cpp CinemaSystem.cpp AdminService.cpp UserService.cpp User.cpp FileManager.cpp Movie.cpp Showtime.cpp SeatMap.cpp -o cine`

Run the program:

`./cine`



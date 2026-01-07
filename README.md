# 🎮 TumblePop (C++ / SFML)

## 📌 Overview
**TumblePop** is a 2D arcade-style game developed in **C++** using the **SFML library** as a **First Semester Programming Fundamentals project**.

The goal of this project was to implement a complete playable game **strictly using basic programming concepts**, without using any **Object-Oriented Programming (OOP)** features, as per course restrictions.

---

## 🛠️ Technologies Used
- **Language:** C++
- **Graphics & Audio:** SFML (Simple and Fast Multimedia Library)
- **IDE:** Any C++ compatible IDE (Visual Studio / Code::Blocks)
- **Platform:** Windows

---

## 🚫 Project Constraints
This project was developed under the following academic limitations:

- ❌ No classes or objects  
- ❌ No inheritance or polymorphism  
- ❌ No structs or advanced data abstractions  
- ❌ No STL containers like `vector`, `map`, etc.  

✔ Only **Programming Fundamentals** were used.

---

## ✅ Concepts Implemented (Programming Fundamentals)

### 🔹 Core Programming Concepts
- Variables and constants
- Conditional statements (`if`, `else`, `switch`)
- Loops (`for`, `while`)
- Functions (modular programming)
- Arrays
- Pointers
- File handling (`fstream`)
- Random number generation
- Mathematical calculations

---

### 🔹 Dynamic Memory Allocation
Dynamic memory allocation was used to manage game entities efficiently:
- `new` and `delete` operators
- Runtime allocation of enemies and game objects
- Manual memory management without STL

This helped in understanding **heap memory**, **pointer manipulation**, and **memory cleanup**.

---

### 🔹 Game Logic Implementation
- Enemy spawning logic
- Player movement and collision detection
- Score handling
- Multiple enemy types using constants
- Animation handling using sprite sheets
- Sound effects and background music

---

### 🔹 Graphics & Rendering
- Window creation using SFML
- Sprite loading and rendering
- Texture management
- Frame-based animation
- Keyboard input handling

---

## 👾 Enemy Types
The game includes multiple enemy variations, each handled using **constants and conditional logic**:
- Ghost
- Skeleton
- Invisible Enemy
- Chelnov
- Minion

Each enemy has different properties such as height, animation frames, and behavior.

---

## 📂 File Structure
📁 Project<br>
├── tumblepop.cpp<br>
├── assets/<br>
│ ├── textures/<br>
│ ├── sounds/<br>
│ └── fonts/<br>
└── README.md

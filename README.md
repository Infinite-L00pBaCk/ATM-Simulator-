# 🏧 ATM Simulator

<p align="center">
  <img src="https://img.shields.io/badge/Language-C++17-blue?style=for-the-badge&logo=cplusplus" />
  <img src="https://img.shields.io/badge/Paradigm-OOP-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge" />
</p>

<p align="center">
  A fully-featured console-based ATM simulator built in C++17 — supporting PIN authentication, deposits, withdrawals, fund transfers, transaction history, and more.
</p>

<p align="center">
  <strong>Pinnacle Labs Internship — Task Project</strong><br/>
  <em>Author: Priyam Prakash</em>
</p>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Usage](#-usage)
- [Class Design](#-class-design)
- [Technologies](#-technologies)

---

## 🔍 Overview

This project simulates the core functionality of a real-world ATM machine using Object-Oriented Programming in C++. All account data and transaction records are persisted to local files, so balances and history survive between sessions.

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔐 **PIN Authentication** | 4-digit PIN with 3-attempt lockout |
| 💰 **Check Balance** | View current account balance instantly |
| 💸 **Cash Withdrawal** | Custom amount withdrawal with Rs. 25,000 daily limit |
| ⚡ **Quick Cash** | Pre-set amounts: Rs. 500 / 1,000 / 2,000 / 5,000 / 10,000 |
| 🏦 **Deposit Funds** | Cash deposit with instant balance update |
| 🔄 **Fund Transfer** | Transfer money between accounts |
| 📄 **Mini Statement** | Last 5 transactions at a glance |
| 🔑 **Change PIN** | Update your PIN securely from the ATM menu |
| 🧾 **Receipt Generation** | Auto-generated text receipts saved to `receipts/` folder |
| 📊 **Transaction History** | Full audit trail stored in `transactions.dat` |
| 🛠️ **Admin Mode** | Create test accounts by typing `ADMIN` at the card prompt |

---

## 📁 Project Structure

```
atm-simulator/
│
├── main.cpp              # Entry point & main ATM loop
│
├── ATMAccount.h          # Account class declaration
├── ATMAccount.cpp        # Account class implementation
│
├── Transaction.h         # Transaction record declaration
├── Transaction.cpp       # Transaction record implementation
│
├── ATMachine.h           # ATM controller class declaration
├── ATMachine.cpp         # ATM controller class implementation
│
├── README.md             # Project documentation
│
├── accounts.dat          # [auto-generated] Persistent account data
├── transactions.dat      # [auto-generated] Transaction audit log
└── receipts/             # [auto-generated] Receipt text files
```

> **Note:** `accounts.dat`, `transactions.dat`, and `receipts/` are generated at runtime and are excluded from version control via `.gitignore`.

---

## 🚀 Getting Started

### Prerequisites

- A C++ compiler with **C++17** support:
  - **Linux/macOS:** `g++` (GCC 7+)
  - **Windows:** MinGW-w64, MSVC, or Clang

### Compile

```bash
g++ -o atm_simulator main.cpp ATMAccount.cpp Transaction.cpp ATMachine.cpp -std=c++17
```

### Run

```bash
# Linux / macOS
./atm_simulator

# Windows
atm_simulator.exe
```

---

## 🖥️ Usage

```
1. Launch the ATM simulator
2. Type ADMIN → create a test account (note the account number & PIN)
3. Enter your account number at the card prompt
4. Authenticate with your 4-digit PIN (3 attempts before lockout)
5. Choose an operation from the session menu
6. Select 0 to eject your card
7. Type EXIT to shut down the ATM
```

### Sample Session Menu

```
  ================================================
    [1] Check Balance
    [2] Withdraw Cash
    [3] Quick Cash
    [4] Deposit Funds
    [5] Fund Transfer
    [6] Mini Statement
    [7] Change PIN
    [8] Print Last Receipt
    [0] Eject Card
  ================================================
```

---

## 🏗️ Class Design

### `ATMAccount`
Manages individual bank accounts — stores holder name, PIN (private), balance, and account status. Handles deposits, withdrawals, and PIN changes with proper validation.

### `Transaction`
A lightweight struct representing a single transaction event. Supports serialization to and deserialization from pipe-delimited strings for file persistence.

### `ATMachine`
The core controller. Manages all connected accounts, handles user sessions, enforces daily withdrawal limits, writes transaction logs, and generates receipts.

---

## 🛠️ Technologies

- **Language:** C++17
- **Paradigm:** Object-Oriented Programming (OOP)
- **Storage:** File-based persistence (pipe-delimited `.dat` files)
- **Platform:** Cross-platform — Windows, Linux, macOS

---

## 📜 License

Developed as part of the **Pinnacle Labs Internship Programme**.  
Feel free to use or adapt this project for educational purposes.

---

<p align="center">Made with ☕ and C++ by <strong>Priyam Prakash</strong></p>

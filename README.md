# ATM Simulator

**Pinnacle Labs Internship — Task Project**
**Author:** Priyam Prakash

---

## Overview

A C++ console application that simulates an Automated Teller Machine (ATM). Users can check balances, withdraw cash, deposit funds, transfer money, and view transaction history — all secured with PIN-based authentication and daily withdrawal limits.

## Features

| Feature | Description |
|---|---|
| **PIN Authentication** | 4-digit PIN with 3-attempt lockout |
| **Check Balance** | View current account balance |
| **Cash Withdrawal** | Custom amount with Rs. 25,000 daily limit |
| **Quick Cash** | Pre-set amounts (Rs. 500 / 1000 / 2000 / 5000 / 10000) |
| **Deposit Funds** | Cash deposit with instant balance update |
| **Fund Transfer** | Transfer between accounts |
| **Mini Statement** | Last 5 transactions at a glance |
| **Change PIN** | Update PIN securely from the ATM |
| **Receipt Generation** | Automatic text-file receipts in `receipts/` folder |
| **Transaction History** | Full audit trail in `transactions.dat` |
| **Admin Mode** | Create test accounts (type `ADMIN` at card prompt) |

## Build & Run

### Prerequisites
- C++ compiler with C++17 support (g++, MinGW, MSVC)

### Compile
```bash
g++ -o atm_simulator.exe main.cpp ATMAccount.cpp Transaction.cpp ATMachine.cpp -std=c++17
```

### Run
```bash
./atm_simulator.exe
```

## Usage

1. Launch the ATM simulator
2. Type `ADMIN` to create a test account (note down the account number and PIN)
3. Enter your account number at the card prompt
4. Authenticate with your 4-digit PIN
5. Use the menu to perform banking operations
6. Select `0` to eject your card
7. Type `EXIT` to shut down the ATM

## Project Structure

```
ATM simulator/
├── ATMAccount.h          # Account class declaration
├── ATMAccount.cpp        # Account class implementation
├── Transaction.h         # Transaction record declaration
├── Transaction.cpp       # Transaction record implementation
├── ATMachine.h           # ATM controller declaration
├── ATMachine.cpp         # ATM controller implementation
├── main.cpp              # Entry point & session loop
├── README.md             # This file
├── accounts.dat          # [auto] Persistent account data
├── transactions.dat      # [auto] Transaction audit log
└── receipts/             # [auto] Receipt text files
```

## Technologies

- **Language:** C++17
- **Paradigm:** Object-Oriented Programming
- **Storage:** File-based persistence (pipe-delimited `.dat` files)
- **Platform:** Cross-platform (Windows/Linux/macOS)

## License

Developed for Pinnacle Labs Internship Programme.

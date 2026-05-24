// ============================================================================
//  ATMAccount.cpp — ATM Account Class Implementation
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#include "ATMAccount.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

// ---- Constructors ----

ATMAccount::ATMAccount()
    : accountNumber(""), holderName(""), pin(""), balance(0.0),
      active(false), creationDate("") {}

ATMAccount::ATMAccount(const std::string& accNo, const std::string& name,
                       const std::string& pinCode, double initialDeposit)
    : accountNumber(accNo), holderName(name), pin(pinCode),
      balance(initialDeposit), active(true) {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    creationDate = oss.str();
}

// ---- Getters ----

std::string ATMAccount::getAccountNumber() const { return accountNumber; }
std::string ATMAccount::getHolderName() const { return holderName; }
double ATMAccount::getBalance() const { return balance; }
bool ATMAccount::isActive() const { return active; }
std::string ATMAccount::getCreationDate() const { return creationDate; }

// ---- PIN Operations ----

bool ATMAccount::verifyPin(const std::string& inputPin) const {
    return pin == inputPin;
}

bool ATMAccount::changePin(const std::string& oldPin, const std::string& newPin) {
    if (pin != oldPin) {
        std::cout << "\n  [!] Current PIN is incorrect.\n";
        return false;
    }
    if (newPin.length() != 4 || newPin.find_first_not_of("0123456789") != std::string::npos) {
        std::cout << "\n  [!] New PIN must be exactly 4 digits.\n";
        return false;
    }
    if (oldPin == newPin) {
        std::cout << "\n  [!] New PIN cannot be the same as the current PIN.\n";
        return false;
    }
    pin = newPin;
    return true;
}

// ---- Account Operations ----

bool ATMAccount::deposit(double amount) {
    if (amount <= 0) {
        std::cout << "\n  [!] Deposit amount must be positive.\n";
        return false;
    }
    if (!active) {
        std::cout << "\n  [!] Cannot deposit to an inactive account.\n";
        return false;
    }
    balance += amount;
    return true;
}

bool ATMAccount::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "\n  [!] Withdrawal amount must be positive.\n";
        return false;
    }
    if (!active) {
        std::cout << "\n  [!] Cannot withdraw from an inactive account.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "\n  [!] Insufficient balance! Available: Rs. "
                  << std::fixed << std::setprecision(2) << balance << "\n";
        return false;
    }
    balance -= amount;
    return true;
}

// ---- Serialization ----

std::string ATMAccount::serialize() const {
    std::ostringstream oss;
    oss << accountNumber << "|" << holderName << "|" << pin << "|"
        << std::fixed << std::setprecision(2) << balance << "|"
        << (active ? "1" : "0") << "|" << creationDate;
    return oss.str();
}

ATMAccount ATMAccount::deserialize(const std::string& line) {
    ATMAccount acc;
    std::istringstream iss(line);
    std::string balStr, activeStr;
    std::getline(iss, acc.accountNumber, '|');
    std::getline(iss, acc.holderName, '|');
    std::getline(iss, acc.pin, '|');
    std::getline(iss, balStr, '|');
    std::getline(iss, activeStr, '|');
    std::getline(iss, acc.creationDate);
    acc.balance = std::stod(balStr);
    acc.active = (activeStr == "1");
    return acc;
}

// ---- Display ----

void ATMAccount::displayAccountInfo() const {
    std::cout << "\n  +============================================+\n";
    std::cout << "  |           ACCOUNT INFORMATION               |\n";
    std::cout << "  +============================================+\n";
    std::cout << "  |  Account No  : " << std::left << std::setw(27) << accountNumber << "|\n";
    std::cout << "  |  Holder Name : " << std::left << std::setw(27) << holderName << "|\n";
    std::cout << "  |  Balance     : Rs. " << std::left << std::setw(23)
              << std::fixed << std::setprecision(2) << balance << "|\n";
    std::cout << "  |  Status      : " << std::left << std::setw(27)
              << (active ? "ACTIVE" : "INACTIVE") << "|\n";
    std::cout << "  |  Opened On   : " << std::left << std::setw(27) << creationDate << "|\n";
    std::cout << "  +============================================+\n";
}

void ATMAccount::displayBalanceBox() const {
    std::cout << "\n  +============================================+\n";
    std::cout << "  |             BALANCE ENQUIRY                 |\n";
    std::cout << "  +============================================+\n";
    std::cout << "  |  Account No  : " << std::left << std::setw(27) << accountNumber << "|\n";
    std::cout << "  |  Holder      : " << std::left << std::setw(27) << holderName << "|\n";
    std::cout << "  |                                            |\n";
    std::cout << "  |  Available Balance:                        |\n";
    std::cout << "  |          Rs. " << std::left << std::setw(30)
              << std::fixed << std::setprecision(2) << balance << "|\n";
    std::cout << "  |                                            |\n";
    std::cout << "  +============================================+\n";
}

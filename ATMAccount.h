// ============================================================================
//  ATMAccount.h — ATM Account Class Declaration
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#ifndef ATMACCOUNT_H
#define ATMACCOUNT_H

#include <string>

class ATMAccount {
private:
    std::string accountNumber;
    std::string holderName;
    std::string pin;           // 4-digit PIN for authentication
    double balance;
    bool active;               // Whether the account is open/active
    std::string creationDate;

public:
    // Constructors
    ATMAccount();
    ATMAccount(const std::string& accNo, const std::string& name,
               const std::string& pinCode, double initialDeposit);

    // Getters
    std::string getAccountNumber() const;
    std::string getHolderName() const;
    double getBalance() const;
    bool isActive() const;
    std::string getCreationDate() const;

    // PIN operations
    bool verifyPin(const std::string& inputPin) const;
    bool changePin(const std::string& oldPin, const std::string& newPin);

    // Account operations
    bool deposit(double amount);
    bool withdraw(double amount);

    // Serialization for file storage
    std::string serialize() const;
    static ATMAccount deserialize(const std::string& line);

    // Display
    void displayAccountInfo() const;
    void displayBalanceBox() const;
};

#endif // ATMACCOUNT_H

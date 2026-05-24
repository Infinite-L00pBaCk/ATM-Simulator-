// ============================================================================
//  ATMachine.h — ATM Controller Class Declaration
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#ifndef ATMACHINE_H
#define ATMACHINE_H

#include <string>
#include <vector>
#include "ATMAccount.h"
#include "Transaction.h"

class ATMachine {
private:
    std::string atmName;
    std::vector<ATMAccount> accounts;
    int nextAccountId;

    // Session state
    int currentAccountIndex;
    double dailyWithdrawnAmount;
    std::string lastWithdrawalDate;

    // File paths
    std::string accountsFile;
    std::string transactionsFile;

    // Internal helpers
    std::string generateAccountNumber();
    int findAccountIndex(const std::string& accNo) const;
    std::string getCurrentDate() const;

    // File I/O
    void saveAccounts();
    void loadAccounts();
    void appendTransaction(const Transaction& txn);
    std::vector<Transaction> loadTransactions() const;
    std::vector<Transaction> getTransactionsForAccount(const std::string& accNo) const;

    // Withdrawal limit
    void resetDailyLimitIfNeeded();
    bool checkWithdrawalLimit(double amount);

public:
    ATMachine(const std::string& name = "Pinnacle ATM");

    // Session Management
    bool startSession();
    void endSession();
    void sessionMenu();
    bool isSessionActive() const;

    // ATM Operations
    void checkBalance();
    void withdrawCash();
    void quickCash();
    void depositFunds();
    void fundTransfer();
    void miniStatement();
    void changePin();
    void printLastReceipt();

    // Admin
    void adminCreateAccount();

    // Receipt
    void generateReceipt(const std::string& operationType, double amount,
                         double balanceAfter, const std::string& extra = "");

    // Display
    void displayWelcomeScreen() const;
    void displaySessionMenu() const;
};

#endif // ATMACHINE_H

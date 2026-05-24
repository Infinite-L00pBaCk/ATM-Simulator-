// ============================================================================
//  Transaction.h — Transaction Record Structure
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER_IN,
    TRANSFER_OUT,
    BALANCE_ENQUIRY,
    PIN_CHANGE,
    ACCOUNT_CREATION
};

struct Transaction {
    std::string accountNumber;
    TransactionType type;
    double amount;
    double balanceAfter;
    std::string timestamp;
    std::string description;

    // Default constructor
    Transaction();

    // Parameterized constructor
    Transaction(const std::string& accNo, TransactionType t, double amt,
                double balAfter, const std::string& desc);

    // Get current timestamp as string
    static std::string getCurrentTimestamp();

    // Convert TransactionType to readable string
    static std::string typeToString(TransactionType t);

    // Convert string back to TransactionType
    static TransactionType stringToType(const std::string& s);

    // Serialize to a single line for file storage
    std::string serialize() const;

    // Deserialize from a stored line
    static Transaction deserialize(const std::string& line);

    // Display-friendly format
    std::string display() const;

    // Compact format for mini-statement
    std::string displayCompact() const;
};

#endif // TRANSACTION_H

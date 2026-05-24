// ============================================================================
//  Transaction.cpp — Transaction Record Implementation
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#include "Transaction.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Default constructor
Transaction::Transaction()
    : accountNumber(""), type(TransactionType::DEPOSIT),
      amount(0.0), balanceAfter(0.0), timestamp(""), description("") {}

// Parameterized constructor
Transaction::Transaction(const std::string& accNo, TransactionType t, double amt,
                         double balAfter, const std::string& desc)
    : accountNumber(accNo), type(t), amount(amt),
      balanceAfter(balAfter), description(desc) {
    timestamp = getCurrentTimestamp();
}

// Get current timestamp as formatted string
std::string Transaction::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Convert TransactionType enum to readable string
std::string Transaction::typeToString(TransactionType t) {
    switch (t) {
        case TransactionType::DEPOSIT:          return "DEPOSIT";
        case TransactionType::WITHDRAWAL:       return "WITHDRAWAL";
        case TransactionType::TRANSFER_IN:      return "TRANSFER_IN";
        case TransactionType::TRANSFER_OUT:     return "TRANSFER_OUT";
        case TransactionType::BALANCE_ENQUIRY:  return "BALANCE_ENQUIRY";
        case TransactionType::PIN_CHANGE:       return "PIN_CHANGE";
        case TransactionType::ACCOUNT_CREATION: return "ACCOUNT_CREATION";
        default:                                return "UNKNOWN";
    }
}

// Convert string back to TransactionType enum
TransactionType Transaction::stringToType(const std::string& s) {
    if (s == "DEPOSIT")          return TransactionType::DEPOSIT;
    if (s == "WITHDRAWAL")       return TransactionType::WITHDRAWAL;
    if (s == "TRANSFER_IN")      return TransactionType::TRANSFER_IN;
    if (s == "TRANSFER_OUT")     return TransactionType::TRANSFER_OUT;
    if (s == "BALANCE_ENQUIRY")  return TransactionType::BALANCE_ENQUIRY;
    if (s == "PIN_CHANGE")       return TransactionType::PIN_CHANGE;
    if (s == "ACCOUNT_CREATION") return TransactionType::ACCOUNT_CREATION;
    return TransactionType::DEPOSIT; // fallback
}

// Serialize transaction to a pipe-delimited string for file storage
std::string Transaction::serialize() const {
    std::ostringstream oss;
    oss << accountNumber << "|"
        << typeToString(type) << "|"
        << std::fixed << std::setprecision(2) << amount << "|"
        << std::fixed << std::setprecision(2) << balanceAfter << "|"
        << timestamp << "|"
        << description;
    return oss.str();
}

// Deserialize a pipe-delimited string back into a Transaction object
Transaction Transaction::deserialize(const std::string& line) {
    Transaction txn;
    std::istringstream iss(line);
    std::string typeStr, amountStr, balStr;

    std::getline(iss, txn.accountNumber, '|');
    std::getline(iss, typeStr, '|');
    std::getline(iss, amountStr, '|');
    std::getline(iss, balStr, '|');
    std::getline(iss, txn.timestamp, '|');
    std::getline(iss, txn.description);

    txn.type = stringToType(typeStr);
    txn.amount = std::stod(amountStr);
    txn.balanceAfter = std::stod(balStr);

    return txn;
}

// Format transaction for full console display
std::string Transaction::display() const {
    std::ostringstream oss;
    oss << "  " << timestamp
        << "  |  " << std::left << std::setw(16) << typeToString(type)
        << "  |  Rs. " << std::right << std::setw(12) << std::fixed << std::setprecision(2) << amount
        << "  |  Bal: Rs. " << std::setw(12) << std::fixed << std::setprecision(2) << balanceAfter
        << "  |  " << description;
    return oss.str();
}

// Compact format for mini-statement (last 5 transactions)
std::string Transaction::displayCompact() const {
    std::ostringstream oss;
    std::string dateOnly = timestamp.substr(0, 10); // Extract YYYY-MM-DD
    std::string sign = "";
    if (type == TransactionType::DEPOSIT || type == TransactionType::TRANSFER_IN ||
        type == TransactionType::ACCOUNT_CREATION) {
        sign = "+";
    } else if (type == TransactionType::WITHDRAWAL || type == TransactionType::TRANSFER_OUT) {
        sign = "-";
    }
    oss << "  " << dateOnly
        << "  " << std::left << std::setw(14) << typeToString(type)
        << "  " << sign << "Rs. " << std::right << std::setw(10) << std::fixed << std::setprecision(2) << amount
        << "   Bal: Rs. " << std::setw(10) << std::fixed << std::setprecision(2) << balanceAfter;
    return oss.str();
}

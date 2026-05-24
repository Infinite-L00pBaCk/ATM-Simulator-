// ============================================================================
//  ATMachine.cpp — ATM Controller Implementation (Part 1: Core + File I/O)
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#include "ATMachine.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <direct.h>

// ============================================================================
// Constructor & Helpers
// ============================================================================

ATMachine::ATMachine(const std::string& name)
    : atmName(name), nextAccountId(1001), currentAccountIndex(-1),
      dailyWithdrawnAmount(0.0), lastWithdrawalDate(""),
      accountsFile("C:/Users/Priyam Prakash/OneDrive/Desktop/banking System/accounts.dat"),
      transactionsFile("C:/Users/Priyam Prakash/OneDrive/Desktop/banking System/transactions.dat") {
    loadAccounts();
    // Support both PNB-xxxx (bank) and ATM-xxxx (atm-created) account numbers
    for (const auto& acc : accounts) {
        std::string accNo = acc.getAccountNumber();
        // Extract numeric suffix after the prefix (PNB- or ATM-)
        std::string numPart;
        size_t dashPos = accNo.find('-');
        if (dashPos != std::string::npos && dashPos + 1 < accNo.size()) {
            numPart = accNo.substr(dashPos + 1);
        } else {
            numPart = accNo.substr(4);
        }
        try {
            int id = std::stoi(numPart);
            if (id >= nextAccountId) nextAccountId = id + 1;
        } catch (...) {
            // Skip if account number doesn't have a numeric suffix
        }
    }
}

std::string ATMachine::generateAccountNumber() {
    std::ostringstream oss;
    oss << "ATM-" << nextAccountId++;
    return oss.str();
}

int ATMachine::findAccountIndex(const std::string& accNo) const {
    for (int i = 0; i < static_cast<int>(accounts.size()); ++i) {
        if (accounts[i].getAccountNumber() == accNo) return i;
    }
    return -1;
}

std::string ATMachine::getCurrentDate() const {
    std::time_t now = std::time(nullptr);
    std::tm* lt = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(lt, "%Y-%m-%d");
    return oss.str();
}

bool ATMachine::isSessionActive() const {
    return currentAccountIndex >= 0;
}

// ============================================================================
// File I/O
// ============================================================================

void ATMachine::saveAccounts() {
    std::ofstream out(accountsFile, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "  [ERROR] Could not open " << accountsFile << "\n";
        return;
    }
    for (const auto& acc : accounts) out << acc.serialize() << "\n";
    out.close();
}

void ATMachine::loadAccounts() {
    std::ifstream in(accountsFile);
    if (!in.is_open()) return;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) accounts.push_back(ATMAccount::deserialize(line));
    }
    in.close();
}

void ATMachine::appendTransaction(const Transaction& txn) {
    std::ofstream out(transactionsFile, std::ios::app);
    if (!out.is_open()) {
        std::cerr << "  [ERROR] Could not open " << transactionsFile << "\n";
        return;
    }
    out << txn.serialize() << "\n";
    out.close();
}

std::vector<Transaction> ATMachine::loadTransactions() const {
    std::vector<Transaction> txns;
    std::ifstream in(transactionsFile);
    if (!in.is_open()) return txns;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) txns.push_back(Transaction::deserialize(line));
    }
    in.close();
    return txns;
}

std::vector<Transaction> ATMachine::getTransactionsForAccount(const std::string& accNo) const {
    std::vector<Transaction> all = loadTransactions();
    std::vector<Transaction> filtered;
    for (const auto& t : all) {
        if (t.accountNumber == accNo) filtered.push_back(t);
    }
    return filtered;
}

// ============================================================================
// Withdrawal Limit
// ============================================================================

void ATMachine::resetDailyLimitIfNeeded() {
    std::string today = getCurrentDate();
    if (lastWithdrawalDate != today) {
        dailyWithdrawnAmount = 0.0;
        lastWithdrawalDate = today;
    }
}

bool ATMachine::checkWithdrawalLimit(double amount) {
    const double DAILY_LIMIT = 25000.00;
    resetDailyLimitIfNeeded();
    if (dailyWithdrawnAmount + amount > DAILY_LIMIT) {
        std::cout << "\n  [!] Daily withdrawal limit exceeded!\n";
        std::cout << "  [i] Daily Limit   : Rs. " << std::fixed << std::setprecision(2) << DAILY_LIMIT << "\n";
        std::cout << "  [i] Already Used  : Rs. " << std::fixed << std::setprecision(2) << dailyWithdrawnAmount << "\n";
        std::cout << "  [i] Remaining     : Rs. " << std::fixed << std::setprecision(2) << (DAILY_LIMIT - dailyWithdrawnAmount) << "\n";
        return false;
    }
    return true;
}

// ============================================================================
// Receipt Generation
// ============================================================================

void ATMachine::generateReceipt(const std::string& opType, double amount,
                                 double balAfter, const std::string& extra) {
    _mkdir("receipts");
    std::time_t now = std::time(nullptr);
    std::tm* lt = std::localtime(&now);
    std::ostringstream fname;
    fname << "receipts/receipt_" << std::put_time(lt, "%Y%m%d_%H%M%S") << ".txt";

    std::ofstream out(fname.str());
    if (!out.is_open()) return;

    std::string accNo = accounts[currentAccountIndex].getAccountNumber();
    std::string name = accounts[currentAccountIndex].getHolderName();

    out << "========================================\n";
    out << "          PINNACLE ATM RECEIPT          \n";
    out << "========================================\n";
    out << "  Date/Time  : " << std::put_time(lt, "%Y-%m-%d %H:%M:%S") << "\n";
    out << "  ATM ID     : " << atmName << "\n";
    out << "  Account    : " << accNo << "\n";
    out << "  Name       : " << name << "\n";
    out << "----------------------------------------\n";
    out << "  Transaction: " << opType << "\n";
    if (amount > 0)
        out << "  Amount     : Rs. " << std::fixed << std::setprecision(2) << amount << "\n";
    if (!extra.empty())
        out << "  Details    : " << extra << "\n";
    out << "  Balance    : Rs. " << std::fixed << std::setprecision(2) << balAfter << "\n";
    out << "----------------------------------------\n";
    out << "  Thank you for using Pinnacle ATM!\n";
    out << "========================================\n";
    out.close();

    std::cout << "  [i] Receipt saved: " << fname.str() << "\n";
}

// ============================================================================
// Display Helpers
// ============================================================================

void ATMachine::displayWelcomeScreen() const {
    std::cout << "\n";
    std::cout << "  +============================================================+\n";
    std::cout << "  |                                                            |\n";
    std::cout << "  |              PINNACLE ATM SIMULATOR                        |\n";
    std::cout << "  |              Automated Teller Machine v2.0                 |\n";
    std::cout << "  |                                                            |\n";
    std::cout << "  |         Connected to: Pinnacle National Bank               |\n";
    std::cout << "  |                                                            |\n";
    std::cout << "  |              Developed by: Priyam Prakash                  |\n";
    std::cout << "  |              Pinnacle Labs Internship                      |\n";
    std::cout << "  |                                                            |\n";
    std::cout << "  +============================================================+\n";
}

void ATMachine::displaySessionMenu() const {
    std::cout << "\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |              ATM MAIN MENU                  |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |                                             |\n";
    std::cout << "  |   [1]  Check Balance                       |\n";
    std::cout << "  |   [2]  Withdraw Cash                       |\n";
    std::cout << "  |   [3]  Quick Cash                          |\n";
    std::cout << "  |   [4]  Deposit Funds                       |\n";
    std::cout << "  |   [5]  Fund Transfer                       |\n";
    std::cout << "  |   [6]  Mini Statement                      |\n";
    std::cout << "  |   [7]  Change PIN                          |\n";
    std::cout << "  |   [8]  Print Last Receipt                  |\n";
    std::cout << "  |   [0]  Eject Card / Logout                 |\n";
    std::cout << "  |                                             |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "\n  Select an option: ";
}

// ============================================================================
// Session Management
// ============================================================================

bool ATMachine::startSession() {
    // Reload accounts from bank on each session start
    // so new accounts created in the banking system are available
    accounts.clear();
    loadAccounts();

    std::string accNo;
    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |         INSERT CARD / ENTER ACCOUNT         |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |  Enter your Bank Account Number             |\n";
    std::cout << "  |  (e.g. PNB-1001) or ADMIN / EXIT:           |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  >> ";
    std::cin >> accNo;

    // Convert to uppercase for comparison
    std::string upper = accNo;
    for (auto& c : upper) c = toupper(c);

    if (upper == "EXIT") return false;

    if (upper == "ADMIN") {
        adminCreateAccount();
        return true; // Return to welcome, not exit
    }

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        std::cout << "\n  [!] Account not found. Please check your account number.\n";
        return true;
    }
    if (!accounts[idx].isActive()) {
        std::cout << "\n  [!] This account is inactive. Please contact your bank.\n";
        return true;
    }

    // PIN Authentication with 3-attempt lockout
    std::string pin;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    std::cout << "\n  Welcome, " << accounts[idx].getHolderName() << "!\n";

    while (attempts < MAX_ATTEMPTS) {
        std::cout << "  Enter PIN (" << (MAX_ATTEMPTS - attempts) << " attempts remaining): ";
        std::cin >> pin;
        if (accounts[idx].verifyPin(pin)) {
            currentAccountIndex = idx;
            dailyWithdrawnAmount = 0.0;
            lastWithdrawalDate = getCurrentDate();
            std::cout << "\n  [OK] Authentication successful!\n";
            sessionMenu();
            return true;
        }
        attempts++;
        if (attempts < MAX_ATTEMPTS) {
            std::cout << "  [!] Incorrect PIN. Try again.\n";
        }
    }

    std::cout << "\n  +============================================+\n";
    std::cout << "  |  [!] CARD BLOCKED — Too many failed        |\n";
    std::cout << "  |      attempts. Please contact your bank.   |\n";
    std::cout << "  +============================================+\n";
    return true;
}

void ATMachine::endSession() {
    if (currentAccountIndex >= 0) {
        std::cout << "\n  +============================================+\n";
        std::cout << "  |   Thank you for using Pinnacle ATM!        |\n";
        std::cout << "  |   Please collect your card.                |\n";
        std::cout << "  +============================================+\n";
    }
    currentAccountIndex = -1;
    dailyWithdrawnAmount = 0.0;
}

void ATMachine::sessionMenu() {
    int choice;
    do {
        displaySessionMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n  [!] Invalid input. Please enter a number (0-8).\n";
            continue;
        }

        switch (choice) {
            case 1: checkBalance();   break;
            case 2: withdrawCash();   break;
            case 3: quickCash();      break;
            case 4: depositFunds();   break;
            case 5: fundTransfer();   break;
            case 6: miniStatement();  break;
            case 7: changePin();      break;
            case 8: printLastReceipt(); break;
            case 0: endSession();     break;
            default:
                std::cout << "\n  [!] Invalid choice. Please select 0-8.\n";
        }
    } while (choice != 0);
}

// ============================================================================
// ATM Operations
// ============================================================================

void ATMachine::checkBalance() {
    accounts[currentAccountIndex].displayBalanceBox();

    Transaction txn(accounts[currentAccountIndex].getAccountNumber(),
                    TransactionType::BALANCE_ENQUIRY, 0.0,
                    accounts[currentAccountIndex].getBalance(),
                    "Balance enquiry at ATM");
    appendTransaction(txn);

    generateReceipt("BALANCE ENQUIRY", 0.0,
                    accounts[currentAccountIndex].getBalance());
}

void ATMachine::withdrawCash() {
    double amount;
    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |            CASH WITHDRAWAL                  |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  Available Balance: Rs. " << std::fixed << std::setprecision(2)
              << accounts[currentAccountIndex].getBalance() << "\n";
    std::cout << "  Daily Limit Remaining: Rs. " << std::fixed << std::setprecision(2)
              << (25000.00 - dailyWithdrawnAmount) << "\n";
    std::cout << "\n  Enter withdrawal amount: Rs. ";
    std::cin >> amount;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n  [!] Invalid amount entered.\n";
        return;
    }

    if (!checkWithdrawalLimit(amount)) return;

    if (accounts[currentAccountIndex].withdraw(amount)) {
        dailyWithdrawnAmount += amount;
        Transaction txn(accounts[currentAccountIndex].getAccountNumber(),
                        TransactionType::WITHDRAWAL, amount,
                        accounts[currentAccountIndex].getBalance(),
                        "ATM cash withdrawal");
        appendTransaction(txn);
        saveAccounts();

        std::cout << "\n  +============================================+\n";
        std::cout << "  |  [OK] Please collect your cash!            |\n";
        std::cout << "  +============================================+\n";
        std::cout << "  |  Amount      : Rs. " << std::left << std::setw(23)
                  << std::fixed << std::setprecision(2) << amount << "|\n";
        std::cout << "  |  New Balance : Rs. " << std::left << std::setw(23)
                  << std::fixed << std::setprecision(2)
                  << accounts[currentAccountIndex].getBalance() << "|\n";
        std::cout << "  +============================================+\n";

        generateReceipt("WITHDRAWAL", amount,
                        accounts[currentAccountIndex].getBalance());
    }
}

void ATMachine::quickCash() {
    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |              QUICK CASH                     |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |                                             |\n";
    std::cout << "  |   [1]  Rs.   500                           |\n";
    std::cout << "  |   [2]  Rs.  1000                           |\n";
    std::cout << "  |   [3]  Rs.  2000                           |\n";
    std::cout << "  |   [4]  Rs.  5000                           |\n";
    std::cout << "  |   [5]  Rs. 10000                           |\n";
    std::cout << "  |   [0]  Cancel                              |\n";
    std::cout << "  |                                             |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  Select: ";

    int choice;
    std::cin >> choice;

    double amounts[] = {0, 500, 1000, 2000, 5000, 10000};
    if (choice < 1 || choice > 5) {
        if (choice != 0) std::cout << "\n  [!] Invalid selection.\n";
        return;
    }

    double amount = amounts[choice];
    if (!checkWithdrawalLimit(amount)) return;

    if (accounts[currentAccountIndex].withdraw(amount)) {
        dailyWithdrawnAmount += amount;
        Transaction txn(accounts[currentAccountIndex].getAccountNumber(),
                        TransactionType::WITHDRAWAL, amount,
                        accounts[currentAccountIndex].getBalance(),
                        "Quick cash withdrawal");
        appendTransaction(txn);
        saveAccounts();

        std::cout << "\n  +============================================+\n";
        std::cout << "  |  [OK] Please collect your cash!            |\n";
        std::cout << "  +============================================+\n";
        std::cout << "  |  Amount      : Rs. " << std::left << std::setw(23)
                  << std::fixed << std::setprecision(2) << amount << "|\n";
        std::cout << "  |  New Balance : Rs. " << std::left << std::setw(23)
                  << std::fixed << std::setprecision(2)
                  << accounts[currentAccountIndex].getBalance() << "|\n";
        std::cout << "  +============================================+\n";

        generateReceipt("QUICK CASH", amount,
                        accounts[currentAccountIndex].getBalance());
    }
}

void ATMachine::depositFunds() {
    double amount;
    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |            DEPOSIT FUNDS                    |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  Enter deposit amount: Rs. ";
    std::cin >> amount;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n  [!] Invalid amount entered.\n";
        return;
    }

    if (accounts[currentAccountIndex].deposit(amount)) {
        Transaction txn(accounts[currentAccountIndex].getAccountNumber(),
                        TransactionType::DEPOSIT, amount,
                        accounts[currentAccountIndex].getBalance(),
                        "ATM cash deposit");
        appendTransaction(txn);
        saveAccounts();

        std::cout << "\n  +============================================+\n";
        std::cout << "  |  [OK] Deposit successful!                  |\n";
        std::cout << "  +============================================+\n";
        std::cout << "  |  Deposited   : Rs. " << std::left << std::setw(23)
                  << std::fixed << std::setprecision(2) << amount << "|\n";
        std::cout << "  |  New Balance : Rs. " << std::left << std::setw(23)
                  << std::fixed << std::setprecision(2)
                  << accounts[currentAccountIndex].getBalance() << "|\n";
        std::cout << "  +============================================+\n";

        generateReceipt("DEPOSIT", amount,
                        accounts[currentAccountIndex].getBalance());
    }
}

void ATMachine::fundTransfer() {
    std::string toAccNo;
    double amount;
    std::string myAccNo = accounts[currentAccountIndex].getAccountNumber();

    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |            FUND TRANSFER                    |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  Enter Recipient Account Number: ";
    std::cin >> toAccNo;

    if (toAccNo == myAccNo) {
        std::cout << "\n  [!] Cannot transfer to the same account.\n";
        return;
    }

    int toIdx = findAccountIndex(toAccNo);
    if (toIdx == -1) {
        std::cout << "\n  [!] Recipient account not found.\n";
        return;
    }
    if (!accounts[toIdx].isActive()) {
        std::cout << "\n  [!] Recipient account is inactive.\n";
        return;
    }

    std::cout << "  Recipient: " << accounts[toIdx].getHolderName() << "\n";
    std::cout << "  Available Balance: Rs. " << std::fixed << std::setprecision(2)
              << accounts[currentAccountIndex].getBalance() << "\n";
    std::cout << "  Enter Transfer Amount: Rs. ";
    std::cin >> amount;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n  [!] Invalid amount entered.\n";
        return;
    }

    if (accounts[currentAccountIndex].withdraw(amount)) {
        accounts[toIdx].deposit(amount);

        Transaction txnOut(myAccNo, TransactionType::TRANSFER_OUT, amount,
                           accounts[currentAccountIndex].getBalance(),
                           "Transfer to " + toAccNo);
        appendTransaction(txnOut);

        Transaction txnIn(toAccNo, TransactionType::TRANSFER_IN, amount,
                          accounts[toIdx].getBalance(),
                          "Transfer from " + myAccNo);
        appendTransaction(txnIn);
        saveAccounts();

        std::cout << "\n  +============================================+\n";
        std::cout << "  |  [OK] Transfer successful!                 |\n";
        std::cout << "  +============================================+\n";
        std::cout << "  |  Sent         : Rs. " << std::left << std::setw(22)
                  << std::fixed << std::setprecision(2) << amount << "|\n";
        std::cout << "  |  To Account   : " << std::left << std::setw(26) << toAccNo << "|\n";
        std::cout << "  |  Your Balance : Rs. " << std::left << std::setw(22)
                  << std::fixed << std::setprecision(2)
                  << accounts[currentAccountIndex].getBalance() << "|\n";
        std::cout << "  +============================================+\n";

        generateReceipt("FUND TRANSFER", amount,
                        accounts[currentAccountIndex].getBalance(),
                        "To: " + toAccNo);
    }
}

void ATMachine::miniStatement() {
    std::string accNo = accounts[currentAccountIndex].getAccountNumber();
    std::vector<Transaction> txns = getTransactionsForAccount(accNo);

    std::cout << "\n  +------------------------------------------------------------------------+\n";
    std::cout << "  |  MINI STATEMENT - " << std::left << std::setw(51) << accNo << "|\n";
    std::cout << "  |  " << std::left << std::setw(69)
              << accounts[currentAccountIndex].getHolderName() << "|\n";
    std::cout << "  +------------------------------------------------------------------------+\n";

    if (txns.empty()) {
        std::cout << "  |  No transactions found.                                               |\n";
    } else {
        // Show last 5 transactions
        int start = (txns.size() > 5) ? txns.size() - 5 : 0;
        for (int i = start; i < static_cast<int>(txns.size()); ++i) {
            std::cout << "  |" << txns[i].displayCompact() << "\n";
        }
    }
    std::cout << "  +------------------------------------------------------------------------+\n";
    std::cout << "  |  Current Balance: Rs. " << std::left << std::setw(47)
              << std::fixed << std::setprecision(2)
              << accounts[currentAccountIndex].getBalance() << "|\n";
    std::cout << "  +------------------------------------------------------------------------+\n";
}

void ATMachine::changePin() {
    std::string currentPin, newPin, confirmPin;
    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |              CHANGE PIN                     |\n";
    std::cout << "  +--------------------------------------------+\n";

    std::cout << "  Enter Current PIN: ";
    std::cin >> currentPin;
    std::cout << "  Enter New 4-digit PIN: ";
    std::cin >> newPin;
    std::cout << "  Confirm New PIN: ";
    std::cin >> confirmPin;

    if (newPin != confirmPin) {
        std::cout << "\n  [!] New PINs do not match.\n";
        return;
    }

    if (accounts[currentAccountIndex].changePin(currentPin, newPin)) {
        Transaction txn(accounts[currentAccountIndex].getAccountNumber(),
                        TransactionType::PIN_CHANGE, 0.0,
                        accounts[currentAccountIndex].getBalance(),
                        "PIN changed successfully");
        appendTransaction(txn);
        saveAccounts();

        std::cout << "\n  +============================================+\n";
        std::cout << "  |  [OK] PIN changed successfully!            |\n";
        std::cout << "  |  Please remember your new PIN.             |\n";
        std::cout << "  +============================================+\n";
    }
}

void ATMachine::printLastReceipt() {
    std::cout << "\n  [i] Receipts are automatically saved to the 'receipts/' folder.\n";
    std::cout << "  [i] Each transaction generates a timestamped receipt file.\n";
}

// ============================================================================
// Admin Operations
// ============================================================================

void ATMachine::adminCreateAccount() {
    std::string name, pin, confirmPin;
    double initialDeposit;

    std::cout << "\n  +--------------------------------------------+\n";
    std::cout << "  |      ADMIN — CREATE NEW ACCOUNT             |\n";
    std::cout << "  +--------------------------------------------+\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "  Enter Full Name: ";
    std::getline(std::cin, name);
    if (name.empty()) { std::cout << "\n  [!] Name cannot be empty.\n"; return; }

    do {
        std::cout << "  Set 4-digit PIN: ";
        std::getline(std::cin, pin);
        if (pin.length() != 4 || pin.find_first_not_of("0123456789") != std::string::npos) {
            std::cout << "  [!] PIN must be exactly 4 digits.\n";
            continue;
        }
        std::cout << "  Confirm PIN: ";
        std::getline(std::cin, confirmPin);
        if (pin != confirmPin) std::cout << "  [!] PINs do not match.\n";
    } while (pin.length() != 4 || pin != confirmPin ||
             pin.find_first_not_of("0123456789") != std::string::npos);

    std::cout << "  Initial Deposit (min Rs. 500.00): Rs. ";
    std::cin >> initialDeposit;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n  [!] Invalid amount.\n";
        return;
    }
    if (initialDeposit < 500.0) {
        std::cout << "\n  [!] Minimum deposit is Rs. 500.00\n";
        return;
    }

    std::string accNo = generateAccountNumber();
    ATMAccount newAcc(accNo, name, pin, initialDeposit);
    accounts.push_back(newAcc);

    Transaction txn(accNo, TransactionType::ACCOUNT_CREATION, initialDeposit,
                    initialDeposit, "Account created with initial deposit");
    appendTransaction(txn);
    saveAccounts();

    std::cout << "\n  +============================================+\n";
    std::cout << "  |     ACCOUNT CREATED SUCCESSFULLY!           |\n";
    std::cout << "  +============================================+\n";
    std::cout << "  |  Account No  : " << std::left << std::setw(27) << accNo << "|\n";
    std::cout << "  |  Holder      : " << std::left << std::setw(27) << name << "|\n";
    std::cout << "  |  Balance     : Rs. " << std::left << std::setw(23)
              << std::fixed << std::setprecision(2) << initialDeposit << "|\n";
    std::cout << "  +============================================+\n";
    std::cout << "\n  [i] Remember your Account Number and PIN!\n";
}

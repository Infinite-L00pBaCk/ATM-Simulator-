// ============================================================================
//  main.cpp — Entry Point & Welcome Loop
//  ATM Simulator | Pinnacle Labs Internship
//  Author: Priyam Prakash
// ============================================================================

#include <iostream>
#include "ATMachine.h"

// Clear the console screen (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    ATMachine atm("Pinnacle ATM - Branch 001");
    bool running = true;

    clearScreen();
    atm.displayWelcomeScreen();

    // Main ATM loop: Welcome → Insert Card → Session → Repeat
    while (running) {
        std::cout << "\n  ================================================\n";
        std::cout << "    Please insert your card to begin.\n";
        std::cout << "    Use your Bank Account No. (e.g. PNB-1001)\n";
        std::cout << "    Type ADMIN to create a new account.\n";
        std::cout << "    Type EXIT to shut down the ATM.\n";
        std::cout << "  ================================================\n";

        running = atm.startSession();
    }

    std::cout << "\n  +============================================================+\n";
    std::cout << "  |                                                            |\n";
    std::cout << "  |          ATM SYSTEM SHUTTING DOWN...                       |\n";
    std::cout << "  |          Thank you for using Pinnacle ATM!                 |\n";
    std::cout << "  |                                                            |\n";
    std::cout << "  +============================================================+\n\n";

    return 0;
}

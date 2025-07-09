#include "../include/ThreadManager.h"
#include <iostream>
#include <string>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        std::cin.clear();
    }
#endif

    ThreadManager manager;
    std::string command;
    std::string current_file;

    std::cout << "Color Sorter v4.2 (Rule validation)\n";
    std::cout << "Available commands:\n"
              << "  set_rule <rule>   (e.g. \"З < С < К\" or \"G < B < R\")\n"
              << "  load <filename>   - Set input file\n"
              << "  start             - Start processing\n"
              << "  stop              - Stop processing\n"
              << "  results           - Show sorted results\n"
              << "  status            - Show processing status\n"
              << "  exit              - Exit program\n";

    while (true) {
        std::cout << "> " << std::flush;

        if (!std::getline(std::cin, command)) {
            if (std::cin.eof()) {
                std::cout << "Exiting...\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Input error occurred. Please try again.\n";
            continue;
        }

        if (command.empty()) continue;

        if (command.find("set_rule ") == 0) {
            try {
                std::string rule = command.substr(9);
                manager.setRule(rule);
                std::cout << "Rule set: " << rule << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
        else if (command.find("load ") == 0) {
            current_file = command.substr(5);
            std::cout << "File set: " << current_file << "\n";
        }
        else if (command == "start") {
            if (current_file.empty()) {
                std::cout << "No file loaded. Use 'load' first.\n";
            } else {
                try {
                    manager.startProcessing(current_file);
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
            }
        }
        else if (command == "stop") {
            if (manager.isProcessing()) {
                manager.stopProcessing();
            } else {
                std::cout << "No active processing\n";
            }
        }
        else if (command == "results") {
            if (!manager.isRuleSet()) {
                std::cout << "Sorting rule is not set. Use 'set_rule' first.\n";
            } else {
                const auto& result = manager.getResult();
                if (result.empty()) {
                    std::cout << "No results available\n";
                } else {
                    for (const auto& obj : result) {
                        std::cout << colorToString(obj.m_color) << ": " << obj.m_payload << "\n";
                    }
                }
            }
        }
        else if (command == "status") {
            std::cout << "Processing: " << (manager.isProcessing() ? "ACTIVE" : "INACTIVE") << "\n";
            std::cout << "Rule set: " << (manager.isRuleSet() ? "YES" : "NO") << "\n";
        }
        else if (command == "exit") {
            if (manager.isProcessing()) {
                manager.stopProcessing();
            }
            break;
        }
        else {
            std::cout << "Unknown command. Try: set_rule, load, start, stop, results, exit\n";
        }
    }

    return 0;
}

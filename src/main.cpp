#include "../include/ThreadManager.h"
#include <iostream>
#include <string>
#include <limits>

int main() {
    ThreadManager manager;
    std::string command;
    std::string current_file;

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
                    std::cout << "Processing started\n";
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
            }
        }
        else if (command == "stop") {
            if (manager.isProcessing()) {
                manager.stopProcessing();
                std::cout << "Processing stopped\n";
            } else {
                std::cout << "No active processing\n";
            }
        }
        else if (command == "results") {
            const auto& result = manager.getResult();
            if (result.empty()) {
                std::cout << "No results available\n";
            } else {
                for (const auto& obj : result) {
                    std::cout << colorToString(obj.m_color) << ": "
                              << obj.m_payload << "\n";
                }
            }
        }
        else if (command == "status") {
            std::cout << "Processing: "
                      << (manager.isProcessing() ? "ACTIVE" : "INACTIVE")
                      << "\n";
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

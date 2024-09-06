#include "text_editor.h"
#include <iostream>
#include <string>

void printHelp() {
    std::cout << "Available commands: \n"
            << " i <text> - Insert text at cursor\n"
            << " d <n> - Delete n characters from cursor\n"
            << " m <row> <col> - Move cursor\n"
            << " g <line> - Go to line\n"
            << " p - Print current text\n"
            << " u - Undo\n"
            << " r - Redo\n"
            << " f <text> - Find text\n"
            << " s <old> <new> - Replace text\n"
            << " o <filename> - Open file\n"
            << " w <filename> - Write to file\n"
            << " q - Quit\n"
            << " h - Show this help\n";
}

int main() {
    TextEditor editor;
    std::string command;

    std::cout << "Simple Text Editor. type 'h' for help.\n";

    while (true) {
        std::cout << "> ";
        std::cin >> command;
        try {
            if (command == "i") {
                std::string text;
                std::getline(std::cin >> std::ws, text);
                editor.insertText(text);
            } else if (command == "d") {
                int count;
                std::cin >> count;
                editor.deleteText(count);
            } else if (command == "m") {
                int row, col;
                std::cin >> row >> col;
                editor.moveCursor(row, col);
            } else if (command == "g") {
                int line;
                std::cin >> line;
                editor.goToLine(line);
            } else if (command == "p") {
                std::cout << editor.getText() << std::endl;
            } else if (command == "u") {
                editor.undo();
            } else if (command == "r") {
                editor.redo();
            } else if (command == "f"){
                std::string searchStr;
                std::getline(std::cin >> std::ws, searchStr);
                auto positions = editor.find(searchStr);
                std::cout << "Found at positions: ";
                for (auto pos : positions) std::cout << pos << " ";
                std::cout << std::endl;
            } else if (command == "s") {
                std::string oldStr, newStr;
                std::cin >> oldStr >> newStr;
                editor.replace(oldStr, newStr);
            } else if (command == "o") {
                std::string filename;
                std::cin >> filename;
                editor.loadFile(filename);
            } else if (command == "w") {
                std::string filename;
                std::cin >> filename;
                editor.saveFile(filename);
            } else if (command == "q") {
                break;
            } else if (command == "h") {
                printHelp();
            } else {
                std::cout << "Unknown command. Type 'h' for help.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch(...) {
            std::cerr << "Unknown error occurred." << std::endl;
        }
    }
    return 0;
}

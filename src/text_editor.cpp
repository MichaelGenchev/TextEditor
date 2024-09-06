#include "text_editor.h"
#include "command.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>

class InsertCommand : public Command {
    TextEditor& editor;
    std::string text;
    size_t position;

public:
    InsertCommand(TextEditor& editor, const std::string& t, size_t pos): editor(editor), text(t), position(pos) {}
    void execute() override { editor.insertTextAt(text, position); }
    void undo() override { editor.deleteTextAt(text.length(), position) ; }
};

class DeleteCommand : public Command {
    TextEditor& editor;
    std::string deletedText;
    size_t position;
public: 
    DeleteCommand(TextEditor& e, size_t count, size_t pos) : editor(e), position(pos) {
        deletedText = editor.getTextAt(pos, count);
    }
    void execute() override { editor.deleteTextAt(deletedText.length(), position); }
    void undo() override { editor.insertTextAt(deletedText, position); }
};

TextEditor::TextEditor() : viewportStart(0), viewportHeight(25), wordWrapEnabled(false) {}

void TextEditor::insertChar(char c) {
    size_t pos = cursor.getGlobalPosition(text);
    if (pos <= text.length()) {
        std::string str(1, c);
        executeCommand(std::make_unique<InsertCommand>(*this, str, pos));
    } else {
        std::cout << "Error: Invalid cursor position for insertion." << std::endl;
    }
}

void TextEditor::deleteChar() {
    if (cursor.getGlobalPosition(text) > 0) {
        executeCommand(std::make_unique<DeleteCommand>(*this, 1, cursor.getGlobalPosition(text) - 1));
        cursor.moveLeft(text);
    }
}

void TextEditor::newLine() {
    insertChar('\n');
}

void TextEditor::moveCursor(int rowDelta, int colDelta) {
    if (rowDelta < 0) {
        for (int i = 0; i > rowDelta; --i) cursor.moveUp(text);
    } else {
        for (int i = 0; i < rowDelta; ++i) cursor.moveDown(text);
    }

    if (colDelta < 0) {
        for (int i = 0; i > colDelta; --i) cursor.moveLeft(text);
    } else {
        for (int i = 0; i < colDelta; ++i) cursor.moveRight(text);
    }
}

void TextEditor::goToLine(size_t lineNumber) {
    size_t currentLine = cursor.getRow();
    moveCursor(static_cast<int>(lineNumber) - static_cast<int>(currentLine), 0);
    cursor.setPosition(text, lineNumber, 0);
}

void TextEditor::insertText(const std::string& str) {
    size_t pos = cursor.getGlobalPosition(text);
    if (pos <= text.length()) {
        try {
            executeCommand(std::make_unique<InsertCommand>(*this, str, pos));
        } catch (const std::exception& e) {
            std::cerr << "Error in executeCommand: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Error: Invalid cursor position for insertion." << std::endl;
    }
}

void TextEditor::deleteText(size_t count) {
    size_t pos = cursor.getGlobalPosition(text);
    if ( pos >= count) {
        executeCommand(std::make_unique<DeleteCommand>(*this, count, pos - count));
        for (size_t i = 0; i < count; ++i){
            cursor.moveLeft(text);
        }
    }
}

std::string TextEditor::getText() const {
    std::string content = text.to_string();
    size_t pos = 0;
    while ((pos = content.find("\\n", pos)) != std::string::npos) {
        content.replace(pos, 2, "\n");
        pos += 1;
    }
    return content;
}

std::string TextEditor::getLine(size_t lineNumber) const {
    size_t start = 0;
    for (size_t i = 0; i < lineNumber; ++i) {
        start = text.substring(start, text.length()).find('\n') + 1 + start;
    }
    size_t end = text.substring(start, text.length()).find('\n');
    if (end == std::string::npos) {
        end = text.length();
    } else {
        end += start;
    }
    return text.substring(start, end);
}

void TextEditor::undo() {
    if (!undoStack.empty()) {
        auto command = std::move(undoStack.back());
        undoStack.pop_back();
        command->undo();
        redoStack.push_back(std::move(command));
    }
}

void TextEditor::redo() {
    if (!redoStack.empty()) {
        auto command = std::move(redoStack.back());
        redoStack.pop_back();
        command->execute();
        undoStack.push_back(std::move(command));
    }
}

std::vector<size_t> TextEditor::find(const std::string& searchStr) const {
    std::vector<size_t> positions;
    size_t pos = 0;
    while ((pos = text.to_string().find(searchStr, pos)) != std::string::npos) {
        positions.push_back(pos);
        pos += searchStr.length();
    }
    return positions;
}

void TextEditor::replace(const std::string& searchStr, const std::string& replaceStr) {
    auto positions = find(searchStr);
    for (auto it = positions.rbegin(); it != positions.rend(); ++it) {
        executeCommand(std::make_unique<DeleteCommand>(*this, searchStr.length(), *it));
        executeCommand(std::make_unique<InsertCommand>(*this, replaceStr, *it));
    }
}

void TextEditor::loadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::cout << "File content read: " << content << std::endl;
        text = Rope(content);
        cursor = Cursor();
        undoStack.clear();
        redoStack.clear();
    } else {
        throw std::runtime_error("Unable to open file");
    }
}

void TextEditor::saveFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (file) {
        file << text.to_string();
    } else {
        throw std::runtime_error("Unable to save file");
    }
}

void TextEditor::setViewportHeight(size_t height) {
    viewportHeight = height;
}

void TextEditor::scrollDown() {
    size_t totalLines = std::count(text.to_string().begin() , text.to_string().end(), '\n') + 1;
    if (viewportStart + viewportHeight < totalLines) {
        ++viewportStart;
    }
}


std::vector<std::string> TextEditor::getViewportContent() const {
    std::vector<std::string> lines;
    size_t start = 0;
    for (size_t i = 0; i < viewportStart; ++i) {
        start = text.substring(start, text.length()).find('\n') + 1 + start;
    }
    for (size_t i = 0; i < viewportHeight; ++i) {
        size_t end = text.substring(start, text.length()).find('\n');
        if (end == std::string::npos){
            lines.push_back(text.substring(start, text.length()));
            break;
        } else {
            lines.push_back(text.substring(start, start + end));
            start += end + 1;
        }
    }
    return lines;
}

size_t TextEditor::getCurrentLine() const {
    return cursor.getRow();
}

size_t TextEditor::getCurrentColumn() const {
    return cursor.getCol();
}


size_t TextEditor::getTotalLines() const {
    return std::count(text.to_string().begin(), text.to_string().end(), '\n') + 1;
}

void TextEditor::setWordWrap(bool enable) {
    wordWrapEnabled = enable;
}


std::vector<std::string> TextEditor::getWrappedLines(size_t startLine, size_t endLine, size_t maxWidth) const {
    std::vector<std::string> wrappedLines;
    for (size_t i = startLine; i <= endLine; ++i){
        std::string line = getLine(i);
        if (wordWrapEnabled && line.length() > maxWidth) {
            size_t start = 0;
            while (start < line.length()) {
                size_t end = start + maxWidth;
                if (end < line.length()) {
                    size_t wrapPoint = line.rfind(' ', end);
                    if (wrapPoint == std::string::npos || wrapPoint <= start) {
                        wrapPoint = end;
                    }
                    wrappedLines.push_back(line.substr(start, wrapPoint - start));
                    start = wrapPoint + 1;
                } else {
                    wrappedLines.push_back(line.substr(start));
                    break;
                }
            }
        } else {
            wrappedLines.push_back(line);
        }
    }
    return wrappedLines;
}

void TextEditor::executeCommand(std::unique_ptr<Command> command) {
    command->execute();
    std::cout << "After executing command i have to do stack things" << std::endl;
    undoStack.push_back(std::move(command));
    redoStack.clear();
}

// Helper methods for Command classes

void TextEditor::insertTextAt(const std::string& str, size_t position) {
    text.insert(position, str);
    std::cout << "AFTER text.insert i will set the position of the cursorrr" << std::endl;
    cursor.setPosition(text, cursor.getRow(), cursor.getCol() + str.length());
}

void TextEditor::deleteTextAt(size_t count, size_t position) {
    text.remove(position, position + count);
    cursor.setPosition(text, cursor.getRow(), cursor.getCol() - count);
}

std::string TextEditor::getTextAt(size_t position, size_t count) const {
    return text.substring(position, position + count);
}
#ifndef TEXT_EDITOR_H

#define TEXT_EDITOR_H

#include "rope.h"
#include "cursor.h"
#include "command.h"

#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Command;


class TextEditor {
private:
    Rope text;
    Cursor cursor;
    std::vector<std::unique_ptr<Command> > undoStack;
    std::vector<std::unique_ptr<Command> > redoStack;
    size_t viewportStart;
    size_t viewportHeight;
    bool wordWrapEnabled;


public:

    TextEditor();

    // basic editing operations
    void insertChar(char c);
    void deleteChar();
    void newLine();

    // Cursor movement
    void moveCursor(int rowDelta, int colDelta);
    void goToLine(size_t lineNumber);

    // Text manipulation
    void insertText(const std::string& str);
    void deleteText(size_t count);
    std::string getText() const;
    std::string getLine(size_t lineNumber) const;

    // Undo/ Redo
    void undo();
    void redo();

    // Search and Replace
    std::vector<size_t> find(const std::string& searchStr) const;
    void replace(const std::string& searchStr, const std::string& replaceStr);

    // File operations
    void loadFile(const std::string& filename);
    void saveFile(const std::string& filename) const;

    // Viewport operations
    void setViewportHeight(size_t height);
    void scrollUp();
    void scrollDown();
    std::vector<std::string> getViewportContent() const;

    // Utility methods
    size_t getCurrentLine() const;
    size_t getCurrentColumn() const;
    size_t getTotalLines() const;

    // Wordwrapping
    void setWordWrap(bool enable);
    std::vector<std::string> getWrappedLines(size_t startLine, size_t endLine, size_t maxWidth) const;

    void insertTextAt(const std::string& str, size_t position);
    void deleteTextAt(size_t count, size_t position);
    std::string getTextAt(size_t position, size_t count) const;

    void debugPrint() const {
        std::cout << "Text content: '" << text.to_string() << "'" << std::endl;
        std::cout << "Cursor position: Row " << cursor.getRow() << ", Col " << cursor.getCol() << std::endl;
        std::cout << "Text length: " << text.length() << std::endl;
    }
private:

    void executeCommand(std::unique_ptr<Command> command);
    // Helper methods for Command classes
};


#endif 
#ifndef CURSOR_H
#define CURSOR_H

#include <cstddef>
#include "rope.h"

class Cursor {
private:
    size_t row; 
    size_t col;
    size_t preferredCol;

    // Helper method to get the length of a specific line 
    size_t getLineLength(const Rope& text, size_t lineNumber) const;

    void updateColPosition(const Rope& text);
public:
    Cursor();

    // Movement methods
    void moveUp(const Rope& text);
    void moveDown(const Rope& text);
    void moveLeft(const Rope& text);
    void moveRight(const Rope& text);


    //Getters
    size_t getRow() const;
    size_t getCol() const;

    // Method to set cursor position directly
    void setPosition(const Rope& text, size_t newRow, size_t newCol);

    size_t getGlobalPosition(const Rope& text) const;
};

#endif
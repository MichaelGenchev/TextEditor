#include "cursor.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>


Cursor::Cursor() : row(0), col(0), preferredCol(0) {}

size_t Cursor::getRow() const { return row; }
size_t Cursor::getCol() const { return col; }
void Cursor::moveUp(const Rope& text) {
    if (row > 0) {
        --row;
        updateColPosition(text);
    }
}

void Cursor::moveDown(const Rope& text) {
    size_t totalLines = std::count(text.to_string().begin(), text.to_string().end(), '\n') + 1;
    if (row < totalLines - 1) {
        ++row;
        updateColPosition(text);
    }
}

void Cursor::moveLeft(const Rope& text) {
    if (col > 0) {
        --col;
        preferredCol = col;
    } else if (row > 0) {
        --row;
        col = getLineLength(text, row);
        preferredCol = col;
    }
}

void Cursor::moveRight(const Rope& text) {
    size_t lineLength = getLineLength(text, row);
    if (col < lineLength) {
        ++col;
        preferredCol = col;
    } else {
        size_t totalLines = std::count(text.to_string().begin(), text.to_string().end(), '\n') + 1;
        if (row < totalLines - 1){
            ++row;
            col = 0;
            preferredCol = 0;
        }
    }
}


void Cursor::setPosition(const Rope& text, size_t newRow, size_t newCol) {
    size_t totalLines = text.countLines();
    if (newRow >= totalLines) {
        throw std::out_of_range("Row is out of range");
    }

    size_t lineLength = getLineLength(text, newRow);
    if (newCol > lineLength) {
        throw std::out_of_range("Column is out of range");
    }

    row = newRow;
    col = newCol;
    preferredCol = newCol;
}

size_t Cursor::getGlobalPosition(const Rope& text) const {
    size_t position = 0;
    for (size_t i = 0; i < row; ++i) {
        position = text.substring(position, text.length()).find('\n') + 1 + position;
    }
    return position + col;
}

size_t Cursor::getLineLength(const Rope& text, size_t lineNumber) const {
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
    return end - start;
}

void Cursor::updateColPosition(const Rope& text) {
    size_t lineLength = getLineLength(text, row);
    col = std::min(preferredCol, lineLength);
}
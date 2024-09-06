#include "rope.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>


// Node constructor

Rope::Node::Node(const std::string& str) : left(nullptr), right(nullptr), data(str), weight(str.length()) {}

Rope::Rope(const std::string& s) : root(std::make_shared<Node>(s)) {}

// Helper func to get character at index

char Rope::index(std::shared_ptr<Node> node, size_t i) const {
    if (!node) return '\0';
    if (i < node->weight) {
        if (node->left) return index(node->left, i);
        return node->data[i];
    }
    return index(node->right, i - node->weight);
}

// Public indexing operator

char Rope::operator[](size_t i) const {
    if (i >= length()) throw std::out_of_range("Index out of range");
    return index(root, i);
}

// Helper func to concatenate two nodes

std::shared_ptr<Rope::Node> Rope::concat(std::shared_ptr<Node> left, std::shared_ptr<Node> right) {
    if (!left) return right;
    if (!right) return left;
    auto newNode = std::make_shared<Node>("");
    assert(newNode);  // Ensure allocation succeeded
    newNode->left = left;
    newNode->right = right;
    newNode->weight = left->weight;  // Weight should be the weight of the left subtree
    return newNode;
}

// Helper func to split a node ata a given index
std::pair<std::shared_ptr<Rope::Node>, std::shared_ptr<Rope::Node>> Rope::split(std::shared_ptr<Node> node, size_t i) {
    if (!node) return {nullptr, nullptr};
    if (i == 0) return {nullptr, node};

    size_t leftWeight = node->left ? node->left->weight : 0;

    if (i <= leftWeight) {
        auto [left, right] = split(node->left, i);
        node->left = right;
        node->weight = (right ? right->weight : 0) + node->data.length() + (node->right ? node->right->weight : 0);
        return {left, node};
    } else if (i < node->weight) {
        size_t splitPoint = i - leftWeight;
        auto newLeft = std::make_shared<Node>(node->data.substr(0, splitPoint));
        newLeft->left = node->left;
        newLeft->weight = leftWeight + splitPoint;
        
        node->data = node->data.substr(splitPoint);
        node->left = nullptr;
        node->weight -= i;
        
        return {newLeft, node};
    } else {
        auto [left, right] = split(node->right, i - node->weight);
        node->right = left;
        node->weight += (left ? left->weight : 0);
        return {node, right};
    }
}

// Helper func to insert a string at a given index
void Rope::insert(std::shared_ptr<Node>& node, size_t i, const std::string& str) {
    std::cout << "Inserting '" << str << "' at index " << i << std::endl;
    std::cout << "Current rope content: " << to_string() << std::endl;

    auto [left, right] = split(node, i);
    auto strNode = std::make_shared<Node>(str);
    strNode->weight = str.length();
    auto leftConcat = concat(left, strNode);
    node = concat(leftConcat, right);
    std::cout << "After insertion: " << to_string() << std::endl;

}

//Public insert func
void Rope::insert(size_t i, const std::string& str) {
    if (i > length()) throw std::out_of_range("Index out of range");
    insert(root, i , str);
}

// Helper func to remove a range of characters

void Rope::remove(std::shared_ptr<Node>& node, size_t i , size_t j) {
    auto [left, temp] = split(node, i);
    auto [_, right] = split(temp, j - i);
    node = concat(left, right);
}

// Public remove func

void Rope::remove(size_t i, size_t j) {
    if (i >= length() || j > length() || i > j) throw std::out_of_range("Invalid range");
    remove(root, i , j);
}


// Public substring func
std::string Rope::substring(std::shared_ptr<Node> node, size_t i, size_t j) const {
    if (!node) return "";
    if (i >= node->weight) return substring(node->right, i - node->weight, j - node->weight);
    if (j <= node->weight) {
        if (node->left) return substring(node->left, i, j);
        return node->data.substr(i, j - i);
    }
    return substring(node->left, i, node->weight) + substring(node->right, 0, j - node->weight);
}

std::string Rope::substring(size_t i, size_t j) const {
    if (i >= length() || j > length() || i > j) throw std::out_of_range("Invalid range");
    return substring(root, i, j);
}

// public length func
size_t Rope::length() const {
    return lengthRecursive(root);
}

size_t Rope::lengthRecursive(const std::shared_ptr<Node>& node) const {
    if (!node) return 0;
    return node->weight + lengthRecursive(node->right);
}

std::string Rope::to_string() const {
    std::string result;
    to_string_recursive(root, result);
    return result;
}

size_t Rope::countLines() const {
    return countLinesRecursive(root) + 1;
}

size_t Rope::countLinesRecursive(const std::shared_ptr<Node>& node) const {
    if (!node) return 0;
    size_t count = std::count(node->data.begin(), node->data.end(), '\n');
    return count + countLinesRecursive(node->left) + countLinesRecursive(node->right);
}

void Rope::to_string_recursive(const std::shared_ptr<Node>& node, std::string& result) const {
    if (!node) return;
    to_string_recursive(node->left, result);
    result += node->data;
    to_string_recursive(node->right, result);
}
// Helper func for rebalancing


std::shared_ptr<Rope::Node> Rope::rebalance_helper(const std::string& s, size_t start, size_t end) {
    if (start >= end) return nullptr;
    size_t mid = (start + end) / 2;
    auto node = std::make_shared<Node>(s.substr(start, mid - start));
    node->left = rebalance_helper(s, start, mid);
    node->right = rebalance_helper(s, mid, end);
    node->weight = mid - start + (node->left ? node->left->weight : 0);
    return node;
}


void Rope::rebalance() {
    std::string s = to_string();
    root = rebalance_helper(s, 0, s.length());
}
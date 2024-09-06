#ifndef ROPE_H
#define ROPE_H

#include <string>
#include <memory>

class Rope {
private:

    struct Node {
        std::shared_ptr<Node> left, right;
        std::string data;
        size_t weight;

        Node(const std::string& str);
    };

    std::shared_ptr<Node> root;

    // Helper functions

    char index(std::shared_ptr<Node> node, size_t i) const;
    std::shared_ptr<Node> concat(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
    std::pair<std::shared_ptr<Node>, std::shared_ptr<Node> > split(std::shared_ptr<Node> node, size_t i);
    void insert(std::shared_ptr<Node>& node, size_t i, const std::string& str);
    void remove(std::shared_ptr<Node>& node, size_t i, size_t j);
    std::string substring(std::shared_ptr<Node> node, size_t i, size_t j) const;
    std::shared_ptr<Node> rebalance_helper(const std::string& s, size_t start, size_t end);
    size_t lengthRecursive(const std::shared_ptr<Node>& node) const;
    void to_string_recursive(const std::shared_ptr<Node>& node, std::string& result) const;
    size_t countLinesRecursive(const std::shared_ptr<Node>& node) const;

public:

    size_t countLines() const;
    Rope(const std::string& s = "");

    //Public interface
    char operator[](size_t i) const;
    void insert(size_t i, const std::string& str);
    void remove(size_t i, size_t j);
    std::string substring(size_t i, size_t j) const;
    size_t length() const;

    // Additional methods
    std::string to_string() const;
    void rebalance();

};






#endif 
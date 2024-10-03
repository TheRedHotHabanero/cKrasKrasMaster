#include "cow.h"
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

template <typename T> struct SplayTreeNode {
    T value;
    std::unique_ptr<SplayTreeNode<T>> left, right;

    SplayTreeNode(const T &val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename CharT> class SplayTree {
  private:
    using Node = SplayTreeNode<COWString<CharT>>;
    std::unique_ptr<Node> root;

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> node) {
        auto leftChild = std::move(node->left);
        node->left = std::move(leftChild->right);
        leftChild->right = std::move(node);
        return leftChild;
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> node) {
        auto rightChild = std::move(node->right);
        node->right = std::move(rightChild->left);
        rightChild->left = std::move(node);
        return rightChild;
    }

    std::unique_ptr<Node> splay(std::unique_ptr<Node> node,
                                const COWString<CharT> &value) {
        if (!node)
            return nullptr;

        if (value < node->value) {
            if (!node->left)
                return node;
            if (value < node->left->value) {
                node->left = splay(std::move(node->left), value);
                node = rotateRight(std::move(node));
            }
            if (!node->left)
                return node;
            return rotateRight(std::move(node));
        } else if (value > node->value) {
            if (!node->right)
                return node;
            if (value > node->right->value) {
                node->right = splay(std::move(node->right), value);
                node = rotateLeft(std::move(node));
            }
            if (!node->right)
                return node;
            return rotateLeft(std::move(node));
        }
        return node;
    }

    void inOrderTraversal(Node *node, std::vector<COWString<CharT>> &result) {
        if (!node)
            return;
        inOrderTraversal(node->left.get(), result);
        result.push_back(node->value);
        inOrderTraversal(node->right.get(), result);
    }

  public:
    SplayTree() : root(nullptr) {}

    void insert(const COWString<CharT> &value) {
        root = splay(std::move(root), value);
        if (!root || !(root->value == value)) {
            auto newNode = std::make_unique<Node>(value);
            if (!root) {
                root = std::move(newNode);
            } else if (value < root->value) {
                newNode->right = std::move(root);
                root = std::move(newNode);
            } else {
                newNode->left = std::move(root);
                root = std::move(newNode);
            }
        }
    }

    COWString<CharT> concatenate() {
        std::vector<COWString<CharT>> result;
        inOrderTraversal(root.get(), result);
        COWString<CharT> concatenated;
        for (const auto &str : result) {
            concatenated = concatenated + str;
        }
        return concatenated;
    }
};

template <typename CharT> class StringTwine {
  private:
    SplayTree<CharT> tree;

    void concat(const COWString<CharT> &str) { tree.insert(str); }

  public:
    void append(const COWString<CharT> &str) { concat(str); }

    void prepend(const COWString<CharT> &str) { concat(str); }

    COWString<CharT> str() { return tree.concatenate(); }
};

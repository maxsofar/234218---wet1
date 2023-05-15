//
// Created by Max on 05/05/2023.
//

#ifndef WET1_TREE_H
#define WET1_TREE_H

#include <new>
#include "Node.h"

template <class Key, class Value>
class Tree {
public:
    /*
     * Constructors
     */
    Tree();
    ~Tree();
    Tree(const Tree& tree) = delete;
    Tree& operator=(const Tree& tree) = delete;
    /*
     * Methods
     */
    bool insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    int getSize() const;
    Node<Key, Value>* getRoot() const;
    void deleteTree(Node<Key, Value>* current);
    Node<Key, Value>* find(const Key& key, Node<Key, Value>* current) const;
    Node<Key, Value>* findMin(Node<Key, Value>* current) const;
    void inOrder(Node<Key, Value>* current, int* output, int& pos);
    Value& getMinNodeValue() const;

private:
    Node<Key, Value>* m_root;
    Node<Key, Value>* m_minNode;
    int size;
    /*
     * Private Methods
     */
    Node<Key, Value>* rotateLeft(Node<Key, Value>* current);
    Node<Key, Value>* rotateRight(Node<Key, Value>* current);
    Node<Key, Value>* balance(Node<Key, Value>* current);
    Node<Key, Value>* insert(Node<Key, Value>* nodeToInsert, Node<Key, Value>* current, bool* doesExist);
    Node<Key, Value>* remove(const Key& key, Node<Key, Value>* current, bool* doesExist);
    static int max(int a, int b);
};

template <class Key, class Value>
Tree<Key, Value>::Tree() : m_root(nullptr), m_minNode(nullptr), size(0) {}

template<class Key, class Value>
void Tree<Key, Value>::deleteTree(Node<Key, Value>* current)
{
    if (current == nullptr)
        return;

    deleteTree(current->getLeft());
    deleteTree(current->getRight());
    delete current;
}

template <class Key, class Value>
Tree<Key, Value>::~Tree()
{
    delete m_minNode;
    deleteTree(this->m_root);
}

template<class Key, class Value>
int Tree<Key, Value>::max(int a, int b)
{
    return (a > b) ? a : b;
}

template<class Key, class Value>
Node<Key, Value> *Tree<Key, Value>::findMin(Node<Key, Value> *current) const
{
    if (current == nullptr)
    {
        return nullptr;
    }
    if (current->getLeft() == nullptr)
    {
        return current;
    }
    return findMin(current->getLeft());
}

template<class Key, class Value>
Value& Tree<Key, Value>::getMinNodeValue() const
{
    return m_minNode->getValue();
}

template<class Key, class Value>
int Tree<Key, Value>::getSize() const
{
    return size;
}

//TODO: maybe write as more general and not specified for movies
template<class Key, class Value>
void Tree<Key, Value>::inOrder(Node<Key, Value>*current, int* output, int& pos)
{
    if (current == nullptr)
        return;

    inOrder(current->getLeft(), output, pos);
    output[pos] = current->getValue()->getId();
    pos++;
    inOrder(current->getRight(), output, pos);

}

template<class Key, class Value>
Node<Key, Value>* Tree<Key, Value>::find(const Key &key, Node<Key, Value>* current) const
{
    if (current == nullptr) {
        return nullptr;
    }
    if (current->getKey() == key) {
        return current;
    }
    if (current->getKey() > key) {
        return find(key, current->getLeft());
    }
    return find(key, current->getRight());
}

template<class Key, class Value>
Node<Key, Value> *Tree<Key, Value>::getRoot() const
{
    return this->m_root;
}

template<class Key, class Value>
Node<Key, Value>* Tree<Key, Value>::rotateLeft(Node<Key, Value>* current)
{
    Node<Key, Value>* rightSubTree = current->getRight();
    Node<Key, Value>* rightLeftSubTree = rightSubTree->getLeft();

    rightSubTree->setLeft(current);
    current->setRight(rightLeftSubTree);

    current->setHeight(max(current->getLeft()->getHeight(), current->getRight()->getHeight()) + 1);
    rightSubTree->setHeight(max(rightSubTree->getLeft()->getHeight(), rightSubTree->getRight()->getHeight()) + 1);

    return rightSubTree;
}

template<class Key, class Value>
Node<Key, Value> *Tree<Key, Value>::rotateRight(Node<Key, Value> *current)
{
    Node<Key, Value>* leftSubTree = current->getLeft();
    Node<Key, Value>* leftRightSubTree = leftSubTree->getRight();

    leftSubTree->setRight(current);
    current->setLeft(leftRightSubTree);

    current->setHeight(max(current->getLeft()->getHeight(), current->getRight()->getHeight()) + 1);
    leftSubTree->setHeight(max(leftSubTree->getLeft()->getHeight(), leftSubTree->getRight()->getHeight()) + 1);

    return leftSubTree;
}

template<class Key, class Value>
Node<Key, Value> *Tree<Key, Value>::balance(Node<Key, Value> *current)
{
    if (current == nullptr) {
        return current;
    }
    current->setHeight(max(current->getLeft()->getHeight(), current->getRight()->getHeight()) + 1);
    int balanceFactor = current->getBalanceFactor();

    // Left heavy
    if (balanceFactor > 1) {
        // Left-Right case
        if (current->getLeft()->getBalanceFactor() < 0) {
            current->setLeft(rotateLeft(current->getLeft()));
        }
        Node<Key, Value>* newRoot = rotateRight(current);
        return newRoot;
    }
    // Right heavy
    else if (balanceFactor < -1) {
        // Right-Left case
        if (current->getRight()->getBalanceFactor() > 0) {
            current->setRight(rotateRight(current->getRight()));
        }

        return rotateLeft(current);
    }

    return current;
}

template<class Key, class Value>
Node<Key, Value>* Tree<Key, Value>::insert(Node<Key, Value>* nodeToInsert ,Node<Key, Value>* current, bool* doesExist)
{
    if (current == nullptr) {
        this->size++;
        return nodeToInsert;
    }

    if (nodeToInsert->getKey() < current->getKey()) {
        current->setLeft(insert(nodeToInsert, current->getLeft(), doesExist));
    } else if (nodeToInsert->getKey() > current->getKey()) {
        current->setRight(insert(nodeToInsert, current->getRight(), doesExist));
    } else {
        *doesExist = true;
        return current;
    }

    current = balance(current);

    return current;
}

template <class Key, class Value>
bool Tree<Key, Value>::insert(const Key& key, const Value& value)
{
    bool doesExist = false;
    if (this->m_root == nullptr) {
        this->m_root = new Node<Key, Value>(key, value);
        this->m_minNode = new Node<Key, Value>(key, value);
        this->size++;
    }
    else if (key == this->m_minNode->getKey()) {
        return false;
    }
    else {
        auto* node = new Node<Key, Value>(key, value);
        if (key < this->m_minNode->getKey()) {
            delete this->m_minNode;
            this->m_minNode = new Node<Key, Value>(key, value);
        }
        this->m_root = insert(node, this->m_root, &doesExist);
    }
    return doesExist;
}

template <class Key, class Value>
Node<Key, Value>* Tree<Key, Value>::remove(const Key& key, Node<Key, Value>* current, bool* doesExist)
{
    if (current == nullptr) {
        *doesExist = false;
        return current;
    }

    if (key < current->getKey()) {
        current->setLeft(remove(key, current->getLeft(), doesExist));
    } else if (key > current->getKey()) {
        current->setRight(remove(key, current->getRight(), doesExist));
    }
    else
    {
        // Case 1: One or No child
        if (current->getLeft() == nullptr || current->getRight() == nullptr)
        {
            Node<Key, Value>* temp = current->getLeft() ? current->getLeft() : current->getRight();
            // No child
            if (temp == nullptr) {
                temp = current;
                current = nullptr;
            }
            // One child
            else {
                *current = *temp;
            }
            delete temp;
            this->size--;
        }
        // Case 2: Two children
        else
        {
            Node<Key, Value>* temp = current->getRight();
            // Find node's successor to swap with
            while (temp->getLeft() != nullptr) {
                temp = temp->getLeft();
            }
            current->setKey(temp->getKey());
            current->setValue(temp->getValue());
            current->setRight(remove(temp->getKey(), current->getRight(), doesExist));
        }
    }

    current = balance(current);
    return current;
}

template <class Key, class Value>
bool Tree<Key, Value>::remove(const Key& key)
{
    bool doesExist = true;
    this->m_root = remove(key, this->m_root, &doesExist);
    if (key == this->m_minNode->getKey()) {
        delete this->m_minNode;
        Node<Key, Value>* temp = findMin(this->m_root);
        if (temp != nullptr) {
            this->m_minNode = new Node<Key, Value>(temp->getKey(), temp->getValue());
        }
    }
    return doesExist;
}

#endif //WET1_TREE_H


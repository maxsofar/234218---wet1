//
// Created by Max on 05/05/2023.
//

#ifndef WET1_NODE_H
#define WET1_NODE_H


template <class Key, class Value>
class Node {
public:
    // Constructor
    explicit Node(const Key& key, const Value& value);
    // Destructor
    ~Node();
    const Key& getKey() const;
    const Value& getValue() const;
    Node<Key,Value>* getLeft() const;
    Node<Key,Value>* getRight() const;
    int getHeight() const;
    void setLeft(Node* left);
    void setRight(Node* right);
    void setHeight(int height);
    void setValue(const Value& value);
    void setKey(const Key& key);
    int getBalanceFactor() const;
    Value& getValue();
private:
    Key key;
    Value value;
    Node<Key,Value>* left;
    Node<Key,Value>* right;
    int height;
};

template<class Key, class Value>
Node<Key, Value>::~Node()
{
    //delete value;
}


template <class Key, class Value>
Node<Key, Value>::Node(const Key& key, const Value& value) : key(key), value(value), left(nullptr), right(nullptr),
                                                             height(0) {}

template<class Key, class Value>
Value &Node<Key, Value>::getValue()
{
    return value;
}

template<class Key, class Value>
void Node<Key, Value>::setKey(const Key& newKey)
{
    this->key = newKey;
}

template<class Key, class Value>
void Node<Key, Value>::setRight(Node<Key,Value>* newRight)
{
    this->right = newRight;
}

template<class Key, class Value>
void Node<Key, Value>::setLeft(Node<Key,Value>* newLeft)
{
    this->left = newLeft;
}

template<class Key, class Value>
Node<Key,Value>* Node<Key, Value>::getRight() const
{
    return right;
}

template<class Key, class Value>
Node<Key,Value>* Node<Key, Value>::getLeft() const
{
    return left;
}


template<class Key, class Value>
int Node<Key, Value>::getBalanceFactor() const
{
    return (this->getLeft()->getHeight() - this->getRight()->getHeight());
}

template <class Key, class Value>
const Key& Node<Key, Value>::getKey() const
{
    return key;
}

template <class Key, class Value>
const Value& Node<Key, Value>::getValue() const
{
    return value;
}

template <class Key, class Value>
int Node<Key, Value>::getHeight() const
{
    if (this == nullptr)
    {
        return -1;
    }
    return height;
}

template <class Key, class Value>
void Node<Key, Value>::setHeight(int newHeight)
{
    this->height = newHeight;
}


template <class Key, class Value>
void Node<Key, Value>::setValue(const Value& newValue)
{
    this->value = newValue;
}

#endif //WET1_NODE_H

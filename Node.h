//
// Created by Max on 05/05/2023.
//

#ifndef WET1_NODE_H
#define WET1_NODE_H

template <class Key, class Value>
class Node {
public:
    /*
     * Constructors
     */
    explicit Node(const Key& key, const Value& value);
    /*
     * Default destructor, tree is responsible for deleting nodes
     */
    ~Node() = default;
    Node(const Node& node) = delete;
    Node& operator=(const Node& node) = default;
    /*
     * Getters
     */
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();
    Node<Key,Value>* getLeft() const;
    Node<Key,Value>* getRight() const;
    int getBalanceFactor() const;
    int getHeight() const;
    /*
     * Setters
     */
    void setLeft(Node* left);
    void setRight(Node* right);
    void setHeight(int height);
    void setValue(const Value& value);
    void setKey(const Key& key);

private:
    Key m_key;
    Value m_value;
    Node<Key,Value>* m_left;
    Node<Key,Value>* m_right;
    int m_height;
};

template <class Key, class Value>
Node<Key, Value>::Node(const Key& key, const Value& value) : m_key(key), m_value(value), m_left(nullptr), m_right(nullptr),
                                                             m_height(0) {}
template <class Key, class Value>
const Key& Node<Key, Value>::getKey() const
{
    return m_key;
}

template <class Key, class Value>
const Value& Node<Key, Value>::getValue() const
{
    return m_value;
}

template<class Key, class Value>
Value& Node<Key, Value>::getValue()
{
    return m_value;
}

template<class Key, class Value>
Node<Key,Value>* Node<Key, Value>::getRight() const
{
    return m_right;
}

template<class Key, class Value>
Node<Key,Value>* Node<Key, Value>::getLeft() const
{
    return m_left;
}

template<class Key, class Value>
int Node<Key, Value>::getBalanceFactor() const
{
    return (this->getLeft()->getHeight() - this->getRight()->getHeight());
}

template <class Key, class Value>
int Node<Key, Value>::getHeight() const
{
    if (this == nullptr)
    {
        return -1;
    }
    return m_height;
}

template<class Key, class Value>
void Node<Key, Value>::setRight(Node<Key,Value>* newRight)
{
    this->m_right = newRight;
}

template<class Key, class Value>
void Node<Key, Value>::setLeft(Node<Key,Value>* newLeft)
{
    this->m_left = newLeft;
}

template <class Key, class Value>
void Node<Key, Value>::setHeight(int newHeight)
{
    this->m_height = newHeight;
}

template <class Key, class Value>
void Node<Key, Value>::setValue(const Value& newValue)
{
    this->m_value = newValue;
}

template<class Key, class Value>
void Node<Key, Value>::setKey(const Key& newKey)
{
    this->m_key = newKey;
}

#endif //WET1_NODE_H

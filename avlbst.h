#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // bst insert
    if(this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    // key exists
    if(this->internalFind(new_item.first) != NULL) {
        this->internalFind(new_item.first)->setValue(new_item.second);
        return;
    }

    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*> (this->root_);
    AVLNode<Key, Value>* toInsert = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);

    while(parent != NULL) {
        if(toInsert->getKey() > parent->getKey()) {
            // if root has no right child, insert
            if(parent->getRight() == NULL) {
                parent->setRight(toInsert);
                toInsert->setParent(parent);
                break;
            }
            else parent = parent->getRight();
            
        } else {
            if(parent->getLeft() == NULL) {
                parent->setLeft(toInsert);
                toInsert->setParent(parent);
                break;
            } else parent = parent->getLeft();
        }
    }
    
    // avl part
    if(parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0);
        return;
    } else if(parent->getBalance() == 0) {
        if(parent->getLeft() == toInsert) parent->updateBalance(-1);
        else if(parent->getRight() == toInsert) parent->updateBalance(1);
        insertFix(parent, toInsert);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    if(parent == NULL || parent == this->root_) return;
    AVLNode<Key, Value>* grandparent = parent->getParent();
    if(grandparent == NULL) return;

    // parent is left child
    if(grandparent->getLeft() == parent) {
        grandparent->updateBalance(-1);
        if(grandparent->getBalance() == 0) return;
        // we dont know if we're done in this case, so check if theres a point where we get out of balance
        if(grandparent->getBalance() == -1) insertFix(grandparent, parent);
        // fix imbalances
        else if(grandparent->getBalance() == -2) {
            // zig zig
            if(parent->getLeft() == node) {
                parent->setBalance(0);
                grandparent->setBalance(0);
                rotateRight(grandparent);
            // zig zag
            } else {
                if(node->getBalance() == -1) {
                    node->setBalance(0);
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                } else if(node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else if(node->getBalance() == 1) {
                    node->setBalance(0);
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                }

                rotateLeft(parent);
                rotateRight(grandparent);
            }

            return;
        }
    }

    // parent is right child
    if(grandparent->getRight() == parent) {
        grandparent->updateBalance(1);
        if(grandparent->getBalance() == 0) return;
        // we dont know if we're done in this case, so check if theres a point where we get out of balance
        if(grandparent->getBalance() == 1) insertFix(grandparent, parent);
        // fix imbalances
        else if(grandparent->getBalance() == 2) {
            // zig zig
            if(parent->getRight() == node) {
                parent->setBalance(0);
                grandparent->setBalance(0);
                rotateLeft(grandparent);
            // zig zag
            } else {
                if(node->getBalance() == -1) {
                    node->setBalance(0);
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                } else if(node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else if(node->getBalance() == 1) {
                    node->setBalance(0);
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                }

                rotateRight(parent);
                rotateLeft(grandparent);
            }
            
            return;
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    // bst remove
    if(this->root_ == NULL) return;

    // only one node
    if(this->root_->getKey() == key && this->root_->getLeft() == NULL && this->root_->getRight() == NULL) {
        delete this->root_;
        this->root_ = NULL;
        return;
    }
    
    AVLNode<Key, Value>* toRemove = static_cast<AVLNode<Key, Value>*> (this->internalFind(key));
    if(toRemove == NULL) return;

    // 2 children
    // swap with predecessor first as it doesnt change the other cases
    if(toRemove->getRight() != NULL && toRemove->getLeft() != NULL) {
        AVLNode<Key, Value>* toRemovePredecessor = static_cast<AVLNode<Key, Value>*> (this->predecessor(toRemove));
        nodeSwap(toRemove, toRemovePredecessor);
    }

    AVLNode<Key, Value>* toRemoveParent = toRemove->getParent();
    int8_t diff;

    // 0 child case
    if(toRemove->getRight() == NULL && toRemove->getLeft() == NULL) {
        // root
        if(this->root_ == toRemove || toRemoveParent == NULL) {
            this->root_ = NULL;
        }
        else {
            // parent's left child
            if(toRemoveParent->getLeft() == toRemove) {
                toRemoveParent->setLeft(NULL);
                diff = 1;
                // delete toRemove;
            }
            // right child
            else {
                toRemoveParent->setRight(NULL);
                diff = -1;
                // delete toRemove;
            }
        }
    }

    // 1 child
    if((toRemove->getRight() == NULL && toRemove->getLeft() != NULL) || (toRemove->getRight() != NULL && toRemove->getLeft() == NULL)) {
        AVLNode<Key, Value>* child;

        if(toRemove->getRight() != NULL) {
            child = toRemove->getRight();
            diff = -1;
        }
        else {
            child = toRemove->getLeft();
            diff = 1;
        }

        if(toRemove == this->root_) {
            this->root_ = child;
            child->setParent(NULL);
            // delete toRemove;
        }
        else {
            if(toRemoveParent->getRight() == toRemove) {
                toRemoveParent->setRight(child);
                child->setParent(toRemoveParent);
                diff = -1;
                // delete toRemove;
            } else {
                toRemoveParent->setLeft(child);
                child->setParent(toRemoveParent);
                diff = 1;
                // delete toRemove;
            }
        }
    }

    delete toRemove;
    removeFix(toRemoveParent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff) {
    if(n == NULL) return;
    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* c;
    AVLNode<Key, Value>* g;
    int8_t ndiff;
    if(p != NULL) {
        if(p->getLeft() == n) ndiff = 1;
        else ndiff = -1;
    }

    if(diff == -1) {
        if(n->getBalance() + diff == -2) {
            c = n->getLeft();
            if(c->getBalance() == -1) {
                // zig zig case
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            } else if(c->getBalance() == 0) {
                // zig zig case
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            } else if(c->getBalance() == 1) {
                // zig zag case
                g = n->getRight();
                rotateLeft(c);
                rotateRight(n);

                if(g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                } else if(g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                } else if(g->getBalance() == -1) {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(p, ndiff);
            }
        } else if(n->getBalance() + diff == -1) {
            n->setBalance(-1);
            return;
        } else if(n->getBalance() + diff == 0) {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    } else if(diff == 1) {
        if(n->getBalance() + diff == 2) {
            c = n->getRight();
            if(c->getBalance() == 1) {
                // zig zig case
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            } else if(c->getBalance() == 0) {
                // zig zig case
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
                return;
            } else if(c->getBalance() == -1) {
                // zig zag case
                g = n->getLeft();
                rotateRight(c);
                rotateLeft(n);

                if(g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                } else if(g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                } else if(g->getBalance() == 1) {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(p, ndiff);
            }
        } else if(n->getBalance() + diff == 1) {
            n->setBalance(1);
            return;
        } else if(n->getBalance() + diff == 0) {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* imbalanced = node->getLeft(); // child causing the imbalance (new root)
    AVLNode<Key, Value>* subtreeRoot = imbalanced->getRight(); // root of the subtree changing parents

    if(this->root_ == node) {
        this->root_ = imbalanced;
        imbalanced->setParent(NULL);
    } else {
        AVLNode<Key,Value>* p = node->getParent();
        imbalanced->setParent(p);
        if(p->getLeft() == node) {
            p->setLeft(imbalanced);
        } else if(p->getRight() == node) {
            p->setRight(imbalanced);
        }
    }
    node->setParent(imbalanced);
    imbalanced->setRight(node);
    // change parents for subtree
    node->setLeft(subtreeRoot);
    if(subtreeRoot != NULL) subtreeRoot->setParent(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* imbalanced = node->getRight(); // child causing the imbalance (new root)
    AVLNode<Key, Value>* subtreeRoot = imbalanced->getLeft(); // root of the subtree changing parents

    if(this->root_ == node) {
        this->root_ = imbalanced;
        imbalanced->setParent(NULL);
    } else {
        AVLNode<Key,Value>* p = node->getParent();
        imbalanced->setParent(p);
        if(p->getLeft() == node) {
            p->setLeft(imbalanced);
        } else if(p->getRight() == node) {
            p->setRight(imbalanced);
        }
    }
    node->setParent(imbalanced);
    imbalanced->setLeft(node);
    // change parents for subtree
    node->setRight(subtreeRoot);
    if(subtreeRoot != NULL) subtreeRoot->setParent(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

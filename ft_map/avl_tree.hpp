#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <stdexcept>
#include "ft_pair/pair.hpp"
#include "map.hpp"
#include <iostream>
#include <memory>


namespace ft {
    enum NodeDirectory {
        LeftDir, RightDir, NoneDir
    };


    template<class K, class V>
    class Tree {
    public:
        Tree(const Tree & other) :
        _parent(other.parent), _left(other._left), _right(other._right), _height(other._height)
        {}

        Tree(Tree *parent, pair<K, V> value) : _parent(parent), _left(0), _right(0), _owner() ,_pair(value), _height(1) {}

        Tree(pair<K, V> value) : _parent(0), _left(0), _right(0), _pair(value), _height(1) {}

        Tree *findRoot() {
            if (_parent == 0)
                return this;
            Tree *tmp_parent = _parent;
            while (tmp_parent)
                if (tmp_parent->_parent)
                    tmp_parent = tmp_parent->_parent;
                else
                    break;
            return tmp_parent;
        }

        Tree<K, V>* insert(const pair<K, V> & p) {
            Comp & c = _owner.value_comp();
            Tree *&appendNode = (c(this->_pair.first, p.first)) ? _right : _left;
            if (appendNode == 0 || appendNode->getData().first == p.first) {

                if (appendNode->getData().first == p.first) {
                    std::cout << "replace" << std::endl;
                    _pair.second = p.second;
                    return appendNode;
                }

                _owner::allocator_type allocator = _owner.

                appendNode = allocator.allocate(1);
                allocator.construct(appendNode, this, p);
                appendNode->recursiveHeightUpdate();
                appendNode->recursiveBalance();

                return appendNode;
            } else
                return appendNode->insert(p);
        }

        Tree *find(K key) {
            if (_pair.first == key)
                return this;

            // TODO: rewrite it. Must use binary search instead of this
            if (_left && _left->find(key))
                return _left->find(key);

            if (_right && _right->find(key))
                return _right->find(key);

            return 0;
        }

        Tree *remove(K key) {
            Tree *delete_node = find(key);
            if (!delete_node)
                return findRoot();
            Tree *delete_parent = delete_node->_parent;


            /* Just leaf */
            if (delete_node->_left == 0 && delete_node->_right == 0) {
                delete_node->setParentToThisNull();
                if (delete_parent) {
                    delete_parent->recursiveHeightUpdate();
                    delete_parent->recursiveBalance();
                }
                delete delete_node; /* Possible 'delete this' ? */
                return delete_parent;
            }

            /* No _left subtree => only one node in _right subtree available */
            if (delete_node->_left == 0) {
                delete_node->setParentToThisToArg(delete_node->_right);
                delete_node->_right->_parent = delete_parent;
                if (delete_parent) {
                    delete_parent->recursiveHeightUpdate();
                    delete_parent->recursiveBalance();
                }
                Tree *result = delete_node->_right;
                delete delete_node;
                return result;
            }

            /* Find maximum node in _left subtree and make it new root of subtree (instead of deleted node) */

            Tree *new_node = delete_node->_left->findMaximum();
            Tree *new_node_parent = new_node->_parent;
            new_node->setParentToThisToArg(new_node->_left);
            if (new_node->_left)
                new_node->_left->_parent = new_node->_parent;
            delete_node->setParentToThisToArg(new_node);
            new_node->_parent = delete_parent;
            new_node->_left = delete_node->_left;
            new_node->_right = delete_node->_right;

            if (delete_node->_left) delete_node->_left->_parent = new_node;
            if (delete_node->_right) delete_node->_right->_parent = new_node;

            delete delete_node;

            if (delete_parent) {
                Tree *balance_node = (new_node_parent == delete_node) ? new_node : new_node_parent;
                balance_node->recursiveHeightUpdate();
                balance_node->balance();
                return delete_parent;
            }
            return new_node;
        }

        pair<K, V> &getData() {
            return _pair;
        }

    private:
        Tree * _left;
        Tree * _parent;
        Tree * _right;

        map<K, V, Comp, Allocator> & _owner;

        pair<K, V> _pair;
        int _height;

        bool balance() {
            Tree *node = this;
            if (balanceFactor() == 2) {
                if (_right && _right->balanceFactor() < 0)
                    bigLeftRotate(node);
                else
                    smallLeftRotate(node);
                return true;
            } else if (balanceFactor() == -2) {
                if (_left && _left->balanceFactor() > 0)
                    bigRightRotate(node);
                else
                    smallRightRotate(node);
                return true;
            }
            return false;
        }

        Tree *findMaximum() {
            return (_right) ? _right->findMaximum() : this;
        }

        void setParentToThisNull() {
            if (_parent) {
                if (_parent->_left == this)
                    _parent->_left = 0;
                else if (_parent->_right == this)
                    _parent->_right = 0;
            }
        }

        void setParentToThisToArg(Tree *new_node) {
            if (_parent) {
                if (_parent->_left == this)
                    _parent->_left = new_node;
                else if (_parent->_right == this)
                    _parent->_right = new_node;
            }
        }

        void correctHeight() {
            int left_height = (_left) ? _left->_height : 0;
            int right_height = (_right) ? _right->_height : 0;
            _height = std::max(right_height, left_height) + 1;
        }

        void recursiveHeightUpdate() {
            Tree *parent_ = _parent;
            correctHeight();
            while (parent_) {
                parent_->correctHeight();
                parent_ = parent_->_parent;
            }
        }

        void recursiveBalance() {
            Tree *parent_ = _parent;
            balance();
            while (parent_) {
                if (parent_->balance())
                    return;
                parent_ = parent_->_parent;
            }
        }

        int balanceFactor() {
            int left_height = (_left) ? _left->_height : 0;
            int right_height = (_right) ? _right->_height : 0;
            return right_height - left_height;
        }

        static void smallLeftRotate(Tree *&root) {
            Tree* a = root;
            Tree* parent = a->_parent;

            NodeDirectory direction = (parent) ? (parent->_left == a) ? LeftDir : RightDir : NoneDir;

            /* Get subtrees */
            Tree* b = a->_right;
            Tree* l = a->_left;
            Tree* c = b->_left;
            Tree* r = b->_right;

            /* Unbinding connections */
            b->_left = b->_right = a->_left = a->_right = b->_parent = a->_parent = 0;
            if (l) l->_parent = 0;
            if (c) c->_parent = 0;
            if (r) r->_parent = 0;

            /* Parent and child connections binding */
            a->_left = l;
            a->_right = c;
            if (l) l->_parent = a;
            if (c) c->_parent = a;
            b->_left = a;
            b->_right = r;
            if (r) r->_parent = b;
            a->_parent = b;
            if (direction != NoneDir)
                b->_parent = parent;
            else {  // Change root
                b->_parent = 0;
                root = b;
            }
            if (direction == LeftDir) parent->_left = b; else if (direction == RightDir) parent->_right = b;
            /* Height correction */
            a->correctHeight();
            b->correctHeight();
            b->recursiveHeightUpdate();
        }

        static void smallRightRotate(Tree *&root) {
            Tree* a = root;
            Tree* parent = a->_parent;

            NodeDirectory direction = (parent) ? (parent->_left == a) ? LeftDir : RightDir : NoneDir;

            /* Get subtrees */
            Tree* b = a->_left;
            Tree* l = b->_left;
            Tree* c = b->_right;
            Tree* r = a->_right;

            /* Unbinding connections */
            b->_left = b->_right = a->_left = a->_right = b->_parent = a->_parent = 0;
            if (l) l->_parent = 0;
            if (c) c->_parent = 0;
            if (r) r->_parent = 0;

            /* Parent and child connections binding */
            a->_left = c;
            a->_right = r;
            if (r) r->_parent = a;
            if (c) c->_parent = a;  /* node A connections */
            b->_left = l;
            b->_right = a;
            if (l) l->_parent = b;                        /* node B connections */
            a->_parent = b;
            if (direction != NoneDir)
                b->_parent = parent;
            else {  // Change root
                b->_parent = 0;
                root = b;
            }
            if (direction == LeftDir) parent->_left = b; else if (direction == RightDir) parent->_right = b;
            /* Height correction */
            a->correctHeight();
            b->correctHeight();
            b->recursiveHeightUpdate();
        }

        static void bigLeftRotate(Tree *&root) {
            smallRightRotate(root->_right);
            smallLeftRotate(root);
            root->recursiveHeightUpdate();
        }

        static void bigRightRotate(Tree *&root) {
            smallLeftRotate(root->_left);
            smallRightRotate(root);
            root->recursiveHeightUpdate();
        }
    };
}

#endif

#ifndef FT_CONTAINERS_FT_MAP_H
#define FT_CONTAINERS_FT_MAP_H


#include <functional>
#include <ft_pair/pair.hpp>
#include <memory>
#include <cstddef>
#include <stdexcept>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iostream>
#include <reverse_iterator.hpp>
// #include "avl_tree.hpp"

namespace ft {

    enum NodeDirectory {
        LeftDir, RightDir, NoneDir
    };


    template<class K, class V, class Owner>
    class Tree {
    public:

        typedef typename Owner::key_compare Comp;
        typedef typename Owner::allocator_type Allocator;

        Tree(const Tree & other) :
                _parent(other.parent), _left(other._left), _right(other._right), _height(other._height), _owner(other._owner)
        {}

        Tree(Tree *parent, pair<K, V> value) : _parent(parent), _left(0), _right(0), _pair(value), _height(1), _owner(parent->_owner) {}

        Tree(Owner & owner, pair<K, V> value) : _parent(0), _left(0), _right(0), _pair(value), _height(1), _owner(owner) {}

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

        Tree<K, V, Owner>* insert(const pair<K, V> & p) {
            Comp c = _owner.key_comp();
            Tree *&appendNode = (c(this->_pair.first, p.first)) ? _right : _left;

            if (appendNode == 0) {
                Allocator allocator = _owner.get_allocator();

                appendNode = new Tree<K, V, Owner>(this, p);
                appendNode->_pair.~pair();
                allocator.construct(&(appendNode->_pair), p);

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

        const pair<K, V> &getData() const {
            return _pair;
        }

        Tree<K,V,Owner>* findMinimum() {
            return (_left) ? _left->findMinimum() : this;
        }

        Tree<K,V,Owner> *findMaximum() {
            return (_right) ? _right->findMaximum() : this;
        }

        Tree * next() {
            Tree * node = this;
            if (_right != 0)
                return _right->findMinimum();

            Tree * parent_ = _parent;
            if (parent_ == 0)
                return 0;

            if (node == parent_->_left)
                return parent_;

            while (parent_ != 0 && node != parent_->_left)
            {
                node = parent_;
                parent_ = node->_parent;
            }

            return parent_;
        };

        Tree * prev() {
            Tree * node = this;
            if (_left != 0)
                return _left->findMaximum();

            Tree * parent = _parent;
            if (parent == 0)
                return 0;

            if (node == parent->_right)
                return parent;

            while (parent != 0 && node != parent->_right)
            {
                node = parent;
                parent = node->_parent;
            }

            return parent;
        };

        const Tree* getLeft() const { return _left; }
        const Tree* getRight() const { return _right; }
        Tree* getLeft() { return _left; }
        Tree* getRight() { return _right; }

    private:
        Tree * _left;
        Tree * _parent;
        Tree * _right;

        Owner & _owner;

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

    template<
            class Key,
            class T,
            class Compare = std::less <Key>,
            class Allocator = std::allocator < ft::pair<Key, T> >
    >
    class map {
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef pair<Key, T> value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef Compare key_compare;
        typedef Allocator allocator_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename Allocator::pointer pointer;
        typedef typename Allocator::const_pointer const_pointer;
        typedef Tree<Key, T, ft::map<Key, T, Compare, Allocator> > node_type;

        class value_compare {
        protected:
            value_compare(Compare & c) : comp(c) {}
            Compare comp;
        public:
            bool operator()( const value_type& lhs, const value_type& rhs ) const {
                return comp(lhs.first, rhs.first);
            }
        };

        map() : root(0), _size(0) {}

        explicit map( const Compare& comp, const Allocator& alloc = Allocator()) :
                _comparator(comp), _allocator(alloc), _size(0) {}

        template< class InputIt >
        map( InputIt first, InputIt last,
             const Compare& comp = Compare(),
             const Allocator& alloc = Allocator() ) : root(0), _size(0) {
            insert(first, last);
        }

        map( const map& other ) : root(0), _size(0) {
            insert(other.begin(), other.end());
        }

        ~map() {
            clear();
        }

        T& at( const Key& key ) {
            return const_cast<T&>(static_cast<const map &>(*this).at(key));
        }

        const T& at( const Key& key ) const {
            iterator it = find(key);
            if (it == end())
                throw std::out_of_range("Map doesn't contain value");
            return it->second;
        }

        Allocator get_allocator() { return _allocator; }

        Allocator get_allocator() const { return _allocator; }

        T& operator[](const Key & k) {
            if (!root)
                return checkRoot(k)->getData().second;

            node_type * node;
            node = root->find(k);

            if (node)
                return node->getData().second;

            ft::pair<Key, T> p = ft::make_pair(k, T());
            node = root->insert(p);
            _size++;
            root = root->findRoot();
            return node->getData().second;
        }

        class iterator {
        public:
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;
            typedef std::bidirectional_iterator_tag iterator_category;

            /* C++ named requirements: LegacyIterator */

            //  * C++ named requirements: CopyConstructible
            iterator(const iterator & other) : _current(other._current), _prev(other._prev) { }
            iterator(node_type* node) : _current(node), _prev(0)  { }
            iterator(node_type* node, node_type* prev) : _current(node), _prev(prev)  { }

            //  * C++ named requirements: CopyAssignable
            iterator& operator=(const iterator& other) {
                if (&other == this)
                    return *this;
                _current = other._current;
                _prev = other._prev;
                return *this;
            }

            // is dereferenceable
            ft::pair<Key, T>& operator*() {
                return _current->getData();
            }

            ft::pair<Key, T> * operator->() {
                return &(operator*());
            }

            //  is incrementable
            iterator& operator++() {
                _prev = _current;
                _current = _current->next();
                return *this;
            }

            iterator operator++(int) {
                iterator result = *this;
                _prev = _current;
                _current = _current->next();
                return result;
            }

            /* C++ named requirements: LegacyInputIterator */

            // C++ named requirements: EqualityComparable
            bool operator==(const iterator & other) {
                return _current == other._current && _prev == other._prev;
            }

            bool operator!=(const iterator & other) {
                return !((*this) == other);
            }

            /* LegacyForwardIterator */

            // C++ named requirements: DefaultConstructible
            iterator() : _current(0), _prev(0) {}

            /* C++ named requirements: LegacyBidirectionalIterator */
            iterator& operator--() {
                _current = _prev;
                if (_prev)
                    _prev = _prev->prev();
                return *this;
            }

            iterator operator--(int) {
                iterator result = *this;
                _current = _prev;
                if (_prev)
                    _prev = _prev->prev();
                return result;
            }

            //  * C++ named requirements: Destructible
            ~iterator() { }


        private:
            node_type* _current;
            node_type* _prev;
        };

        typedef reverse_iterator<iterator> reverse_iterator;
        typedef const reverse_iterator const_reverse_iterator;
        typedef const iterator const_iterator;

        iterator begin() {
            return static_cast<const map &>(*this).begin();
        }

        iterator end() {
            return static_cast<const map &>(*this).end();
        }

        const_iterator begin() const {
            if (!root) return iterator();
            return iterator(root->findMinimum());
        }

        const_iterator end() const {
            if (!root) return iterator();
            return iterator(0, root->findMaximum());
        }

        //TODO: implement reverse iterators
        reverse_iterator rbegin() {
            return static_cast<const map &>(*this).rbegin();
        }

        const_reverse_iterator rbegin() const {
            //TODO: Implement this
        }

        reverse_iterator rend() {
            return static_cast<const map &>(*this).rend();
        }

        const_reverse_iterator rend() const {
            //TODO: Implement this
        }

        bool empty() const {
            return (_size == 0);
        }

        size_type size() const {
            return _size;
        }

        size_type max_size() const {
            //TODO: Implement this
            std::numeric_limits<difference_type>::max();
        }
        
        void clear() {
            if (!root)
                return;
            erase(begin(), end());
            _size = 0;
        }

        pair<iterator, bool> insert( const value_type& value ) {
            size_type prev_size = _size;
            (*this)[value.first] = value.second;
            node_type * insert_pos = root->find(value.first);
            iterator result(insert_pos, insert_pos->prev());
            return make_pair(result, prev_size == _size);
        }

        iterator insert( iterator hint, const value_type& value ) {
            //TODO: Implement this.
        }

        template< class InputIt >
        void insert( InputIt first_, InputIt last_ ) {
            if (checkRoot(first_->first, first_->second) != 0)
                first_++;
            while (first_ != last_) {
                (*this)[first_->first] = first_->second;
                first_++;
            }
        }

        void erase( iterator pos ) {
            if (!root)
                return;
            node_type * remove_result = root->remove(pos->first);
            root = (remove_result) ? remove_result->findRoot() : 0;
            _size--;
        }

        void erase( iterator start, iterator end) {
            if (!root)
                return;
            size_t elements = std::distance(start, end);
            Key *keys = new Key[elements];
            size_t counter = 0;
            while (start != end) {
                keys[counter++] = start->first;
                start++;
            }
            for (int i = 0; i < elements; ++i) {
                node_type * remove_result = root->remove(keys[i]);
                root = (remove_result) ? remove_result->findRoot() : 0;
            }
            delete [] keys;
            _size -= elements;
        }

        void swap(map<Key, T, Compare, Allocator> & other) {
            std::swap(root, other.root);
            std::swap(_comparator, other._comparator);
            std::swap(_allocator, other._allocator);
            std::swap(_size, other._size);
        }

        size_type count( const Key& key ) const {
            if (!root)
                return 0;
            return root->find(key) != 0;
        }

        iterator find( const Key& key ) {
            if (!root)
                return iterator();
            node_type * item = root->find(key);
            if (item)
                return iterator(item, item->prev());
            return end();
        }

        const_iterator find( const Key& key ) const {
            if (!root)
                return iterator();
            node_type * item = root->find(key);
            if (item)
                return iterator(item, item->prev());
            return end();
        }

        pair<iterator,iterator> equal_range( const Key& key ) {
            pair<iterator,iterator> result = make_pair(lower_bound(key), upper_bound(key));
        }

        pair<const_iterator, const_iterator> equal_range( const Key& key ) const {
            pair<iterator,iterator> result = make_pair(lower_bound(key), upper_bound(key));
        }

        iterator lower_bound( const Key& key ) {
            return static_cast<const map &>(*this).lower_bound(key);
        }

        const_iterator lower_bound( const Key& key ) const {
            if (!root || root->findMaximum()->getData().first < key)
                return end();
            if (key < root->findMinimum()->getData().first)
                return begin();

            iterator it(root, root->prev());
            while (it != 0) {
                if (key == it->first)
                    return it;
                if (it->first > key) {
                    it--;
                    if (it->first < key)
                        return ++it;
                } else {
                    it++;
                    if (it->first >= key)
                        return it;
                }
            }
            return it;
        }

        iterator upper_bound( const Key& key ) {
            return static_cast<const map &>(*this).upper_bound(key);
        }

        const_iterator upper_bound( const Key& key ) const {
            if (!root || root->findMaximum()->getData().first <= key)
                return end();
            if (key < root->findMinimum()->getData().first)
                return begin();

            iterator it(root, root->prev());
            if (key == it->first)
                return ++it;
            while (it != 0) {
                if (it->first > key) {
                    it--;
                    if (it->first <= key)
                        return ++it;
                } else {
                    it++;
                    if (it->first > key)
                        return it;
                }
            }
            return it;
        }

        key_compare key_comp() const {
            return _comparator;
        }

        value_compare value_comp() const { return value_compare(); }

    private:
        node_type *root;
        Compare _comparator;
        Allocator _allocator;
        size_type _size;

        node_type * checkRoot(const Key & k, T value = T()) {
            if (!root) {
                root = new node_type(*this, ft::make_pair(k, value));
                _size++;
                return root;
            }
            return 0;
        }
    };
}


#endif
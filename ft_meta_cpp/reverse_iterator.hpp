#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

namespace ft {
    template<class Iter>
    class reverse_iterator {
        typedef reverse_iterator<Iter> r_iterator;
    public:
        reverse_iterator () : _iterator() {}
        reverse_iterator (const Iter & iterator) : _iterator(iterator) { }

        typedef typename Iter::value_type value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename Iter::pointer pointer;
        typedef typename Iter::reference reference;
        typedef typename Iter::iterator_category iterator_category;
        typedef Iter iterator_type;
        /* C++ named requirements: LegacyIterator */


        //  * C++ named requirements: CopyAssignable
        r_iterator& operator=(const r_iterator& other) {
            if (&other == this)
                return *this;
            _iterator = other._iterator;
            return *this;
        }

        // is dereferenceable
        typename Iter::value_type& operator*() {
            return *_iterator;
        }

        //  is incrementable
        r_iterator& operator++() {
            _iterator--;
            return *this;
        }

        r_iterator operator++(int) {
            r_iterator &result = *this;
            result._iterator--;
            return result;
        }

        /* C++ named requirements: LegacyInputIterator */

        // C++ named requirements: EqualityComparable
        bool operator==(const r_iterator & other) {
            return _iterator == other._iterator;
        }

        bool operator!=(const r_iterator & other) {
            return _iterator != other._iterator;
        }

        r_iterator& operator--() {
            _iterator++;
            return *this;
        }

        r_iterator operator--(int) {
            r_iterator &result = *this;
            _iterator++;
            return result;
        }

        /* C++ named requirements: LegacyRandomAccessIterator */
        r_iterator& operator+=(difference_type n) {
            _iterator -= n;
            return *this;
        }

        r_iterator& operator-=(difference_type n) {
            _iterator += n;
            return *this;
        }

        r_iterator operator+(difference_type n) {
            r_iterator result = *this;
            result._iterator -= n;
            return result;
        }

        r_iterator operator-(difference_type n) {
            r_iterator result = *this;
            result._iterator += n;
            return result;
        }

        difference_type operator-(r_iterator & other) {
            return other._iterator - _iterator;
        }

        typename Iter::value_type& operator[](int i) {
            return _iterator[-i - 1];
        }

        // The below rules imply that LegacyRandomAccessIterator also implements LessThanComparable.
        bool operator<(const r_iterator &other) {
            return _iterator >= other._iterator;
        }

        bool operator>(const r_iterator &other) {
            return _iterator <= other._iterator;
        }

        bool operator>=(const r_iterator &other) {
            return _iterator < other._iterator;
        }

        bool operator<=(const r_iterator &other) {
            return _iterator > other._iterator;
        }

        Iter base() {
            return _iterator;
        }

    private:
        Iter _iterator;
    };
}


#endif

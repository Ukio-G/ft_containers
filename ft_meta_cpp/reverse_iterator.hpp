#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

namespace ft {
    template<class Iter>
    class reverse_iterator {
        typedef reverse_iterator<Iter> r_iterator;
    public:


        typedef typename Iter::value_type value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename Iter::pointer pointer;
        typedef typename Iter::reference reference;
        typedef typename Iter::iterator_category iterator_category;
        typedef Iter iterator_type;

        reverse_iterator () : current() {}

        explicit reverse_iterator (iterator_type iterator) : current(iterator) { }

        template<class U>
        reverse_iterator (const reverse_iterator<U> & other) : current(other.current) {}

        pointer operator->() {
            return &(operator*());
        }
        /* C++ named requirements: LegacyIterator */


        //  * C++ named requirements: CopyAssignable
        template< class U >
        reverse_iterator& operator=( const reverse_iterator<U>& other ) {
            if (&other == this)
                return *this;
            current = other.current;
            return *this;
        }

        // is dereferenceable
        reference operator*() {
            Iter it = current;
            return *(--it);
        }

        //  is incrementable
        r_iterator& operator++() {
            current--;
            return *this;
        }

        r_iterator operator++(int) {
            r_iterator &result = *this;
            result.current--;
            return result;
        }

        /* C++ named requirements: LegacyInputIterator */

        r_iterator& operator--() {
            current++;
            return *this;
        }

        r_iterator operator--(int) {
            r_iterator &result = *this;
            current++;
            return result;
        }

        /* C++ named requirements: LegacyRandomAccessIterator */
        r_iterator& operator+=(difference_type n) {
            current -= n;
            return *this;
        }

        r_iterator& operator-=(difference_type n) {
            current += n;
            return *this;
        }

        r_iterator operator+(difference_type n) const {
            r_iterator result = *this;
            result.current -= n;
            return result;
        }

        r_iterator operator-(difference_type n) const {
            r_iterator result = *this;
            result.current += n;
            return result;
        }

        typename iterator_type::value_type& operator[](difference_type n) const {
            return current[-n - 1];
        }

        iterator_type base() const {
            return current;
        }

    private:
        Iter current;
    };
    template< class Iterator >
    typename reverse_iterator<Iterator>::difference_type
    operator-( const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs ) {
        return lhs.base() - rhs.base();
    }

    template<class Iter1, class Iter2>
    bool operator< (const reverse_iterator<Iter1> &lhs,
                    const reverse_iterator<Iter2> &rhs) {
        return lhs.base() < rhs.base();
    }

    template<class Iter1, class Iter2>
    bool operator> (const reverse_iterator<Iter1> &lhs,
                    const reverse_iterator<Iter2> &rhs) {
        return lhs.base() > rhs.base();
    }

    template<class Iter1, class Iter2>
    bool operator<= (const reverse_iterator<Iter1> &lhs,
                    const reverse_iterator<Iter2> &rhs) {
        return lhs.base() <= rhs.base();
    }

    template<class Iter1, class Iter2>
    bool operator>= (const reverse_iterator<Iter1> &lhs,
                    const reverse_iterator<Iter2> &rhs) {
        return lhs.base() >= rhs.base();
    }

    template<class Iter1, class Iter2>
    bool operator== (const reverse_iterator<Iter1> &lhs,
                    const reverse_iterator<Iter2> &rhs) {
        return lhs.base() == rhs.base();
    }

    template<class Iter1, class Iter2>
    bool operator!= (const reverse_iterator<Iter1> &lhs,
                    const reverse_iterator<Iter2> &rhs) {
        return lhs.base() != rhs.base();
    }

    template< class Iter >
    reverse_iterator<Iter>
    operator+( typename reverse_iterator<Iter>::difference_type n,
               const reverse_iterator<Iter>& it ) {
        reverse_iterator<Iter> result = it;
        result.current -= n;
        return result;
    }

    template< class Iter >
    reverse_iterator<Iter>
    operator-( typename reverse_iterator<Iter>::difference_type n,
               const reverse_iterator<Iter>& it ) {
        reverse_iterator<Iter> result = it;
        result.current += n;
        return result;
    }
}


#endif

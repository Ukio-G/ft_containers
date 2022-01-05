#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft {
    template <class T1, class T2>
    struct pair {
        /* 1 */
        pair() { }

        /* 2 */
        pair( const T1& x, const T2& y ) : first(x), second(y) { }

        /* 4 */
        template< class U1, class U2 >
        pair( const pair<U1, U2>& p ) : first(p.first), second(p.second) { }

        /* 7 */
        pair(const pair & other) : first(other.first), second(other.second) { }


        T1 first;
        T2 second;
    };

    template< class T1, class T2 >
    pair<T1, T2> make_pair( T1 t, T2 u ) {
        return pair<T1, T2>(t, u);
    }
}

#endif

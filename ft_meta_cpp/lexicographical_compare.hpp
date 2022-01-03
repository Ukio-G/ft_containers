#ifndef LEXICOGRAPHICAL_COMPARE_HPP
#define LEXICOGRAPHICAL_COMPARE_HPP

namespace ft {
/**
 * Get pair iterator to vec1 and vec2.
 * @tparam InputIt1
 * @tparam InputIt2
 * @param first1
 * @param last1
 * @param first2
 * @param last2
 * @return true if vec1 < vec2, or if vec1 less than vec2
 */
    template<class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2) {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }


/**
 * Get pair iterator to vec1 and vec2, and comparator
 * @tparam InputIt1
 * @tparam InputIt2
 * @param first1
 * @param last1
 * @param first2
 * @param last2
 * @return true if vec1 < vec2, or if vec1 less than vec2
 */
    template<class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2,
                                 Compare comp) {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) {
            if (comp(*first1, *first2)) return true;
            if (comp(*first2, *first1)) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }
}
#endif //LEXICOGRAPHICAL_COMPARE_HPP

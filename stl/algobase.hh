//
// Created by rda on 2023/5/26.
//

#ifndef MINISTL_ALGOBASE_HH
#define MINISTL_ALGOBASE_HH

namespace stl
{
    /* Modifying sequence operations */
    template <typename InputIt, typename OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
    {
    }

    template <typename InputIt, typename OutputIt, typename UnaryPredictate>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first, UnaryPredictate pred)
    {
    }

    template <typename BidirIt1, typename BidirIt2>
    BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
    {
    }

    template <typename InputIt, typename Size, typename OutputIt>
    OutputIt copy_n(InputIt first, Size count, OutputIt result)
    {
    }

    template <typename ForwardIt, typename T>
    void fill(ForwardIt first, ForwardIt last, const T &value)
    {
    }

    template <typename OutputIt, typename Size, typename T>
    void fill_n(OutputIt first, Size count, const T &value)
    {
    }

    template <class T>
    void swap(T &a, T &b) noexcept
    {
        T tmp = a;
        a = b;
        b = tmp;
    }

    template <typename T, std::size_t N>
    void swap(T (&a)[N], T (&b)[N])
    {
        for (std::size_t i = 0; i < N; ++i)
            swap(a[i], b[i]);
    }

    template <typename ForwardIt1, typename ForwardIt2>
    void iter_swap(ForwardIt1 a, ForwardIt2 b)
    {
        swap(*a, *b);
    }

    /* Comparison operations */
    template <typename InputIt1, typename InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
    {
    }

    template <typename InputIt1, typename InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
    {
    }
    /* Minimum/maximum operations */
    template <class T>
    const T &max(const T &a, const T &b)
    {
        return (b < a) ? a : b;
    }
    template <class T>
    const T &min(const T &a, const T &b)
    {
        return (b < a) ? b : a;
    }

    /* Non-modifying sequence operations */
    template <class InputIt1, class InputIt2>
    std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1,
             InputIt2 first2)
    {
    }

} // namespace stl

#endif
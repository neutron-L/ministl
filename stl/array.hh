// /home/rda/Documents/ministl/stl
// Create by rda on 2023/4/6
// 

#ifndef MINISTL_ARRAY_HH
#define MINISTL_ARRAY_HH


#include <cstddef>
#include <initializer_list>
#include <utility>

namespace stl
{
    template <typename T, size_t N>
    class array
    {
        friend void swap(array<T, N> & lhs, array<T, N> & rhs)
        {
            for (int i = 0; i < arr_size; ++i)
                swap(lhs[i], rhs[i]);
        }
    private:
        T arr[N];
        size_t arr_size;
    public:
        using value_type = T;
        using iterator = value_type *;
        /*
         * Class array<> constructors
         * */
        array();
        array(const array &);
        array(array &&);
        array(std::initializer_list<T>);

        /*
         * Non-modifying Operation
         * */
        bool empty() { return size() == 0;}
        size_t size() const { return arr_size; }
        size_t max_size() const { return N; }

        bool operator==(const array & rhs) const
        {
            for (int i = 0; i < N; ++i)
                if (arr[i] != rhs.arr[i])
                    return false;
            return true;
        }

        bool operator!=(const array & rhs) const
        {
            return !(this->operator==(rhs));
        }

        bool operator<=(const array & rhs)
        {
            for (int i = 0; i < N; ++i)
                if (arr[i] > rhs.arr[i])
                    return false;
            return true;
        }

        bool operator>=(const array & rhs)
        {
            for (int i = 0; i < N; ++i)
                if (arr[i] < rhs.arr[i])
                    return false;
            return true;
        }

        bool operator>(const array & rhs)
        {
            return !(this->operator<=(rhs));
        }

        bool operator<(const array & rhs)
        {
            return !(this->operator>=(rhs));
        }

        /*
         * Assignment Operation
         * */
        array & operator=(const array & rhs)
        {
            if (this != &rhs)
            {
                for (int i = 0; i < N; ++i)
                    arr[i] = rhs.arr[i];
            }

            return *this;
        }

        array & operator=(array && rhs)
        {
            if (this != &rhs)
            {
                for (int i = 0; i < N; ++i)
                    arr[i] = std::move(rhs.arr[i]);
            }

            return *this;
        }

        void swap(array & rhs)
        {
            swap(*this, rhs);
        }



    };
} // namespace stl


#endif
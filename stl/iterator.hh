//
// Created by rda on 2023/4/5.
//

#ifndef MINISTL_ITERATOR_HH
#define MINISTL_ITERATOR_HH

#include <cstddef>

#include <cstdint>

namespace stl
{
    /*
     * 5种类型的迭代器
     * */

    /*
     * @brief   Input Iterator: 所指向的对象不允许外界改变，read only
     * */
    struct input_iterator_tag
    {
    };

    /*
     * @brief   Output Iterator: write only
     * */
    struct output_iterator_tag
    {
    };

    /*
     * @brief   Forward Iterator: 允许“写入型“算法在此迭代器指定的区间上进行读写操作
     * */
    struct forward_iterator_tag : public input_iterator_tag
    {
    };

    /*
     * 可以双向移动的迭代器
     * */
    struct bidirectional_iterator_tag : public forward_iterator_tag
    {
    };

    /*
     * @brief   随机访问迭代器，支持所有指针的运算操作的迭代器
     * */
    struct random_access_iterator_tag : public bidirectional_iterator_tag
    {
    };

    /*
     * @brief   迭代器类型，该类的派生类符合stl的标准，一般只需要指定前两个类型，即迭代器类型和迭代器指向对象的类型
     * */
    template<typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T *, typename Reference = T &>
    struct iterator
    {
        using iterator_category = Category;
        using value_type = T;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };

    /*
     * @brief   迭代器的traits
     * */
    template<typename Iterator>
    struct iterator_traits
    {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    /*
     * @brief   针对原生指针而设计的偏特化版本traits
     * */
    template<typename T>
    struct iterator_traits<T *>
    {
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T &;
    };

    /*
     * @brief   针对原生的pointer-to-const而设计的篇特化版本traits
     * */
    template<typename T>
    struct iterator_traits<const T *>
    {
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T &;
    };

    /*
     * @brief   Iterator Adapters
    * */

    /*
     * @brief   bidirectional_iterator_tag and random_access_iterator都具备一个反向迭代器适配器
     * */
    template <typename Iterator>
    class reverse_iterator : public iterator<typename iterator_traits<Iterator>::iterator_category,
            typename iterator_traits<Iterator>::value_type,
            typename iterator_traits<Iterator>::difference_type,
            typename iterator_traits<Iterator>::pointer,
            typename iterator_traits<Iterator>::reference>{
        template<class Iter>
        friend class reverse_iterator;
    protected:
        Iterator current;
        using _traits_type = iterator_traits<Iterator>;

    public:
        using iterator_type = Iterator;
        using reference = typename _traits_type::reference;
        using difference_type = typename _traits_type::difference_type;
        using pointer = typename _traits_type::pointer ;

        /*
         * @brief   iterator constructors
         * */
        reverse_iterator() : current() {}
        reverse_iterator(Iterator it) : current(it) {}
        reverse_iterator(const reverse_iterator& other) : current(other.current) {}
        template <typename Iter>
        reverse_iterator(const reverse_iterator<Iter>& other) :  current(other.current) {}

        /*
         * @brief   iterator operators
         * */
        reverse_iterator& operator++()
        {
            --current;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator res = *this;
            --current;
            return res;
        }

        reverse_iterator& operator--()
        {
            ++current;
            return *this;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator res = *this;
            ++current;
            return res;
        }

        reverse_iterator operator+(difference_type n) const
        {
            return reverse_iterator(current - n);
        }

        reverse_iterator& operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }

        reverse_iterator operator-(difference_type n) const
        {
            return reverse_iterator(current + n);
        }

        reverse_iterator& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }

        difference_type operator-(const reverse_iterator& other) const
        {
            return other.current - current;
        }

        reference operator*() const
        {
            Iterator temp = current;
            return *--temp;
        }

        pointer operator->() const
        {
            Iterator temp = current;
            return to_pointer(--temp);
        }
        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }

        /*
         * @brief   compare functions
         * */
        bool operator==(const reverse_iterator& other) const
        {
            return base() == other.base();
        }

        bool operator!=(const reverse_iterator& other) const
        {
            return base() != other.base();
        }

        bool operator<(const reverse_iterator& other) const
        {
            return other.base() < base();
        }

        bool operator<=(const reverse_iterator& other) const
        {
            return other.base() <= base();
        }

        bool operator>(const reverse_iterator& other) const
        {
            return other.base() > base();
        }

        bool operator>=(const reverse_iterator& other) const
        {
            return other.base() >= base();
        }

        /*
         *
         * */
        template<class Tp>
        pointer to_pointer(Tp t)
        {
            return t->operator->();
        }

        template<class Tp>
        pointer to_pointer(Tp *  t)
        {
            return t;
        }

        Iterator base() const
        {
            return current;
        }
    };

    /*
     * @brief   插入迭代器
     * 1. back inserter，其内部调用push_back，在容器末端插入元素
     * 2. front inserter，其内部调用push_front，在容器头部插入元素
     * 3. general inserter，在初始化时接收的第二个参数所指向的位置前面插入元素，调用成员函数insert
     * TODO:
     * 4. normal iterator 
     * 5. move iterator
     * */

    /*
     * @brief   back inserter
     * */
    template<typename Container>
    class back_insert_iterator : public iterator<output_iterator_tag, void ,void ,void ,void>
    {
    private:
        Container * pc;
    public:
        explicit back_insert_iterator(Container & c) : pc (std::__addressof(c))
        {}

        back_insert_iterator & operator=(const typename Container::value_type & x)
        {
            pc->push_back(x);
            return *this;
        }

        back_insert_iterator & operator=(typename Container::value_type && x)
        {
            pc->push_back(std::move(x));
            return *this;
        }

        back_insert_iterator & operator*()
        {
            return *this;
        }

        back_insert_iterator & operator++()
        {
            return *this;
        }

        back_insert_iterator operator++(int)
        {
            return *this;
        }
    };

    /*
     * @brief   获取一个容器的back inserter
     * @param   非const修饰的容器类型变量
     * @return  关联该容器的一个back inserter
     * */
    template<typename Container>
    back_insert_iterator<Container> back_inserter(Container & c)
    {
        return back_insert_iterator<Container>(c);
    }


    /*
     * @brief   front inserter
     * */
    template<typename Container>
    class front_insert_iterator : public iterator<output_iterator_tag, void ,void ,void ,void>
    {
    private:
        Container * pc;
    public:
        explicit front_insert_iterator(Container & c) : pc (std::__addressof(c))
        {}

        front_insert_iterator & operator=(const typename Container::value_type & x)
        {
            pc->push_front(x);
            return *this;
        }

        front_insert_iterator & operator=(typename Container::value_type && x)
        {
            pc->push_front(std::move(x));
            return *this;
        }

        front_insert_iterator & operator*()
        {
            return *this;
        }

        front_insert_iterator & operator++()
        {
            return *this;
        }

        front_insert_iterator operator++(int)
        {
            return *this;
        }
    };

    /*
     * @brief   获取一个容器的back inserter
     * @param   非const修饰的容器类型变量
     * @return  关联该容器的一个back inserter
     * */
    template<typename Container>
    front_insert_iterator<Container> front_inserter(Container & c)
    {
        return front_insert_iterator<Container>(c);
    }


    /*
     * @brief   insert iterator
     * */
    template<typename Container>
    class insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
    {
        using Iter = typename Container::iterator;
    protected:
        Container * container;
        Iter iter;

    public:
        insert_iterator(Container & c, Iter i) : container(std::__addressof(c)), iter(i)
        {}

        insert_iterator & operator=(const typename Container::value_type & val)
        {
            iter = container->insert(iter, val);
            ++iter;
            return *this;
        }

        insert_iterator & operator=(typename Container::value_type && val)
        {
            iter = container->insert(iter, std::move(val));
            ++iter;
            return *this;
        }

        insert_iterator & operator*()
        {
            return *this;
        }

        insert_iterator & operator++()
        {
            return *this;
        }

        insert_iterator& operator++(int)
        {
            return *this;
        }
    };

    /*
     * @brief   获取一个容器指定迭代器位置的insert iterator
     * @param   c   容器
     * @param   i   容器中的某个迭代器
     * @return  一个插入迭代器
     * */
    template<typename Container>
    inline insert_iterator<Container> inserter(Container & c, typename Container::iterator i)
    {
        return insert_iterator<Container>(c, i);
    }

    // TODO: normal_iterator
    // TODO: move_iterator

    /*
     * @brief   获取一个迭代器的类别
     * @param   迭代器变量
     * @return  该迭代器类别类型的值
     * */
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator &)
    {
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type *
    distance_type(const Iterator &)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type *
    value_type(const Iterator &)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    }

    /*
     * @brief   获取两个输入迭代器之间的“距离”
     * @param   first   第一个迭代器
     * @param   last    第二个迭代器
     * @return  从第一个迭代器逐步自增到第二个迭代器需要自增的次数
     * */
    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance_aux(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    /*
     * @brief   获取两个随机访问迭代器之间的“距离”，直接相减即可
     * @param   first   第一个迭代器
     * @param   last    第二个迭代器
     * @return  第二个迭代器减去第一个迭代器的值
     * */
    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance_aux(InputIterator first, InputIterator last, random_access_iterator_tag)
    {
        return last - first;
    }


    /*
     * @brief   获取两个迭代器之间的“距离”，根据迭代器的具体类别选择辅助方法
     * @param   first   第一个迭代器
     * @param   last    第二个迭代器
     * @return  从第一个迭代器到第二个迭代器需要自增的次数
     * */
    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last)
    {
        using category = typename iterator_traits<InputIterator>::iterator_category;
        return distance_aux(first, last, iterator_category(first));
    }

    /*
     * @brief   输入迭代器向前自增指定次数，逐步自增
     * @param   first   待自增的迭代器
     * @param   n       自增次数
     * */
    template <typename InputIterator, typename Distance>
    inline void
    advance_aux(InputIterator & i, Distance n, input_iterator_tag)
    {
        while (n--)
            ++i;
    }

    /*
     * @brief   双向迭代器自增（自减）指定次数，根据参数的符号自增或自减即可
     * @param   first   待自增的迭代器
     * @param   n       自增（自减）次数
     * */
    template <typename InputIterator, typename Distance>
    inline void
    advance_aux(InputIterator & i, Distance n, bidirectional_iterator_tag)
    {
        if (n >= 0)
            while (n--)
                ++i;
        else
            while (n++)
                --i;
    }

    /*
     * @brief   随机访问迭代器自增指定次数，直接相加即可
     * @param   first   待自增的迭代器
     * @param   n       自增次数
     * */
    template <typename InputIterator, typename Distance>
    inline void
    advance_aux(InputIterator & i, Distance n, random_access_iterator_tag)
    {
        i += n;
    }

    /*
     * @brief   迭代器向前自增指定次数，根据迭代器类型选择合适的辅助方法
     * @param   first   待自增的迭代器
     * @param   n       自增次数
     * */
    template <typename InputIterator, typename Distance>
    inline void
    advance(InputIterator & i, Distance n)
    {
        advance_aux(i, n, iterator_category(i));
    }
} // namespace stl


#endif //UNTITLED_ITERATOR_H

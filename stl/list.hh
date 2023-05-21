#ifndef MINISTL_LIST_HH
#define MINISTL_LIST_HH

#include "alloc.hh"

namespace stl
{
    template<typename T, typename Alloc = alloc>
    class list
    {
    private:
        template<F>
        struct list_node
        {
            using void_pointer = void *;
            void_pointer prev;
            void_pointer next;
            F data;
        };

        template<typename F, typename Ref, typename Ptr>
        struct list_iterator
        {

        };
    public:

    protected:
    public:
    };

} // namespace stl


#endif
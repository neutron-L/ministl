#ifndef MINISTL_TYPE_HH
#define MINISTL_TYPE_HH

#include <iostream>

struct Item
{
    void * ptr;

    Item()=default;
    Item(const Item & rhs)
    {
        ptr = rhs.ptr;
        std::cout << "copy-constructor" << std::endl;
    }

    Item(Item && rhs)
    {
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
        std::cout << "move-constructor" << std::endl;
    }

    Item & operator=(const Item & rhs)
    {
        if (this != &rhs)
            ptr = rhs.ptr;
        std::cout << "copy-assignment" << std::endl;
        return *this;
    }

    Item & operator=(Item && rhs)
    {
        if (this != &rhs)
        {
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
            std::cout << "move-assginment" << std::endl;
        }
        
        return *this;
    }
};



#endif

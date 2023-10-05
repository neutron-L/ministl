//
// Created by rda on 2023/9/14.
//

#ifndef MINISTL_UNORDERED_SET_HH
#define MINISTL_UNORDERED_SET_HH

#include <iostream>
#include <cassert>


#include "alloc.hh"
#include "construct.hh"
#include "iterator.hh"


namespace stl
{
    template <typename Key,
              typename Hash = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    class unordered_set
    {
    private:
        using ht_type = Hashtable<>;
    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using hasher = Hash;
        using key_equal = KeyEqual;
        using allocator_type = Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = ht_type::iterator;
        using const_iterator = ht_type::const_iterator;
        using iterator = ht_type::local_iterator;
        using const_local_iterator = ht_type::const_local_iterator;
    
        
    };
} // namespace stl

#endif
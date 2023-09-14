//
// Created by rda on 2023/9/14.
//

#ifndef MINISTL_RBTREE_HH
#define MINISTL_RBTREE_HH

#include "alloc.hh"
#include "construct.hh"
#include "iterator.hh"

namespace stl
{
    // RB-Tree Node Color, include red and black
    enum class RBTreeColor
    {
        Red,
        Black
    };

    // Base struct of RB-Tree Node
    struct RBTreeNodeBase
    {
        using color_type = RBTreeColor;
        using base_ptr = RBTreeNodeBase *;

        color_type color{};
        base_ptr parent{};
        base_ptr left{};
        base_ptr right{};

        base_ptr minimum()
        {
            base_ptr cur = this;
            while (cur->left != nullptr)
                cur = cur->left;
            return cur;
        }

        base_ptr maximum()
        {
            base_ptr cur = this;
            while (cur->right != nullptr)
                cur = cur->right;
            return cur;
        }
    };

    // struct of RB-Tree Node
    template <typename Value>
    struct RBTreeNode : public RBTreeNodeBase
    {
        using link_type = RBTreeNode<Value> *;
        Value value_field;
    };

    // RB-Tree iterator base class
    struct RBTreeBaseIterator
    {
        using base_ptr = RBTreeNodeBase::base_ptr;
        using iterator_category = stl::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        base_ptr node;

        void increment()
        {
        }

        void decrement()
        {
        }
    };

    // RB-Tree iterator
    template <typename Value, typename Ref, typename Ptr>
    struct RBTreeIterator : public RBTreeBaseIterator
    {
        using value_type = Value;
        using reference = Ref;
        using pointer = Ptr;

        using iterator = RBTreeIterator<Value, Value &, Value *>;
        using const_iterator = RBTreeIterator<Value, const Value &, const Value *>;
        using self = RBTreeIterator<Value, Ref, Ptr>;
        using link_type = RBTreeNode<Value> *;

        RBTreeIterator() {}
        RBTreeIterator(link_type lt) { node = lt; }
        RBTreeIterator(const iterator &it) { node = it.node; }

        reference operator*() const { return link_type(node)->value_field; }

        pointer operator->() const { return &operator*(); }

        self &operator++()
        {
            increment();
            return *this;
        }
        self operator++(int)
        {
            auto obj = *this;
            increment();
            return obj;
        }

        self &operator++()
        {
            decrement();
            return *this;
        }
        self operator++(int)
        {
            auto obj = *this;
            decrement();
            return obj;
        }
    };

} // namespace stl

#endif

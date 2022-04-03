#ifndef _SET_H_
#define _SET_H_

#include <algorithm>
#include <iostream>
#include "avltree.h"

template <class T>
class set 
{
    AVLTree<T> body;
    AVLnode<T>* root = nullptr;
    size_t sz = 0;

public:
    class iterator 
    {
        AVLnode<T>* node = nullptr;
        const set* owner = nullptr;

    public:
        iterator() = default;
        iterator(AVLnode<T>* in, const set* own) : node(in), owner(own) { }
        iterator(const iterator& other) : node(other.node), owner(other.owner) { }

        T& operator* () const { return node->key; }
        T* operator-> () const { return &(node->key); }
        T* operator& () const { return (&node)->key;  }

        iterator& operator++ () 
        {
            if (node == nullptr) 
                return *this;
            node = node->nextNode();
            return *this;
        }
        iterator operator++ (int)
        {
            iterator cur = *this;
            ++* this;
            return cur;
        }

        iterator& operator-- () 
        {
            if (node == nullptr) 
            {
                *this = owner->rbegin();
                return *this;
            }
            
            const AVLnode<T>* tmp = node->prevNode();

            if (tmp != nullptr) 
                node = tmp;
            
            return *this;
        }
        iterator operator-- (int) 
        {
            iterator cur = *this;
            --* this;
            return cur;
        }

        bool operator== (const iterator& other) const { return (node == other.node && owner == other.owner); }
        bool operator!= (const iterator& other) const { return (node != other.node || owner != other.owner); }
    };

    set() = default;

    template<class InputIter>
    set(InputIter st, InputIter en) 
    {
        while (st != en) 
        {
            try 
            {
                insert(*st);
            }
            catch (...) 
            {
                if (root != nullptr) 
                    delete root;
            }
            ++st;
        }
    }

    set(const std::initializer_list<T>& inp) { *this = set(inp.begin(), inp.end()); }
    set(const set& other) { *this = set(other.begin(), other.end()); }
    ~set() { delete root; }

    void swap(set& rhs) { std::swap(this->root, rhs.root); }

    set& operator= (const set& other) 
    {
        set tmp(other); //possible problem

        std::swap(tmp.root, root);
        sz = tmp.sz;
        
        return *this;
    }

    iterator begin() const { return iterator(body.begin(root), this); }
    iterator end() const { return iterator(nullptr, this); }
    iterator rbegin() const { return iterator(body.rbegin(root), this); }

    iterator find(const T& key) const { return iterator(body.find(root, key), this); }

    iterator lower_bound(const T& key) const { return iterator(body.lower_bound(root, key), this); }

    void insert(const T& key) 
    {
        if (find(key) != end()) 
            return;

        root = body.insertNode(root, key);
        if (root != nullptr)
            root->parent = nullptr;
        ++sz;
    }

    void erase(const T& key) 
    {
        if (find(key) == end()) 
            return;
        root = body.removeNode(root, key);
        if (root != nullptr)
            root->parent = nullptr;
        --sz;
    }

    bool empty() const { return (sz == 0); }
    size_t size() const { return sz; }

    // Union.
    template <typename T>
    std::shared_ptr<set<T>> operator+(const set<T>& s)
    {
        auto d = std::make_shared<set<T>>();

        for (auto i = this->begin(); i != this->end(); ++i)
            d->insert(*i);

        if (this == &s)
            return std::make_shared<set<T>>(*this);

        for (auto i = s.begin(); i != s.end(); ++i)
            d->insert(*i);

        return d;
    }

    // Intersection.
    template <typename T>
    std::shared_ptr<set<T>> operator&(const set<T>& s)
    {
        auto d = std::make_shared<set<T>>();

        if (this == &s)
        {
            for (auto it = this->begin(); it != this->end(); ++it)
                d->insert(*it);

            return d;
        }

        auto s1 = s.begin(), s2 = this->begin();

        while (s1 != s.end() && s2 != this->end())
        {
            if (*s1 < *s2)
                ++s1;
            else if (*s2 < *s1)
                ++s2;
            else //if (*s1 == *s2)
            {
                d->insert(*s1);
                ++s1;
                ++s2;
            }
        }

        return d;
    }

    // Differnece.
    template <typename T>
    std::shared_ptr<set<T>> operator-(const set<T>& s)
    {
        auto d = std::make_shared<set<T>>();

        if (this != &s)
        {
            auto s1 = this->begin(), s2 = s.begin();

            while (s1 != this->end() && s2 != s.end())
            {
                if (*s1 < *s2)
                {
                    d->insert(*s1);
                    ++s1;
                }
                else if (*s2 < *s1)
                {
                    d->insert(*s2);
                    ++s2;
                }
                else
                {
                    ++s1;
                    ++s2;
                }
            }

            for (auto it = s2; it != s.end(); ++it)
                d->insert(*it);
        }
        
        return d;
    }
};

#endif

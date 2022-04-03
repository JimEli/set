#include <iostream>
#include <algorithm>
#include <cassert>
#include <iterator>
#include "set.h"

template 
<typename T>
void out(set<T>& s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
        std::cout << *it;
    std::cout << std::endl;
}

// Visual Leak Detector.
#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"

int main()
{
    set<int> s;

    assert(s.size() == 0);
    assert(s.empty());

    s.insert(10);
    auto iter = s.begin();
    assert(*iter == 10);

    s.insert(6);
    s.insert(6);

    assert(*(s.find(6)) == 6);
    assert(*(s.find(10)) == 10);
    assert(s.find(12) == s.end());

    iter = s.begin();
    assert(*iter == 6);
    ++iter;
    assert(*iter == 10);
    ++iter;
    assert(iter == s.end());

    s.insert(7);
    s.insert(9);
    s.insert(9);
    s.insert(8);
    s.insert(11);

    iter = s.begin();
    assert(*iter == 6);
    ++iter;
    assert(*iter == 7);
    ++iter;
    assert(*iter == 8);
    ++iter;
    assert(*iter == 9);
    ++iter;
    assert(*iter == 10);
    ++iter;
    assert(*iter == 11);

    // Second set.
    set<int> s2;
    s2.insert(3);
    s2.insert(7);
    s2.insert(-1);
    s2.insert(16);
    s2.insert(11);
    s2.insert(4);

    iter = s2.find(3);
    assert(*iter == 3);
    iter = s2.find(888);
    assert(iter == s2.end());

    s2.erase(7);
    iter = s2.begin();
    assert(*iter == -1);
    ++iter;
    assert(*iter == 3);
    ++iter;
    assert(*iter == 4);
    ++iter;
    assert(*iter == 11);
    ++iter;
    assert(*iter == 16);
    ++iter;
    assert(iter == s2.end());

    s2.erase(16);
    iter = s2.begin();
    assert(*iter == -1);
    ++iter;
    assert(*iter == 3);
    ++iter;
    assert(*iter == 4);
    ++iter;
    assert(*iter == 11);
    ++iter;
    assert(iter == s2.end());

    s2.erase(3);
    iter = s2.begin();
    assert(*iter == -1);
    ++iter;
    assert(*iter == 4);
    ++iter;
    assert(*iter == 11);
    ++iter;
    assert(iter == s2.end());

    s2.erase(11);
    iter = s2.begin();
    assert(*iter == -1);
    ++iter;
    assert(*iter == 4);
    ++iter;
    assert(iter == s2.end());

    s2.erase(-1);
    iter = s2.begin();
    assert(*iter == 4);
    ++iter;
    assert(iter == s2.end());

    s2.erase(4);
    iter = s2.begin();
    assert(iter == s2.end());

    std::cout << "All tests passed." << std::endl;

    // 3 sets.
    set<int> st1;
    set<int> st2;

    st1.insert(1);
    st1.insert(2);
    st1.insert(3);
    st1.insert(4);
    st2.insert(4);
    st2.insert(5);
    st2.insert(6);
        
    std::cout << "Union:\n";
    auto sr1 = st1 + st2;
    out(st1); out(st2); out(*sr1);

    std::cout << "Intersection:\n";
    sr1 = st1 & st2;
    out(st1); out(st2); out(*sr1);

    std::cout << "Difference:\n";
    sr1 = st1 - st2;
    out(st1); out(st2); out(*sr1);
        
    std::cout << "Swap:\n";
    st1.print();
    st2.print();
    st1.swap(st2);
    out(st1); out(st2);
}



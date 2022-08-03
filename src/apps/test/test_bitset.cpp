#include <bitset>
#include <iostream>
#include <base/phys_math.h>
#include "test_bitset.h"

template <class T>
inline T halfRight(T x) {
    int n = x.size()/2;
    return (x << n) >> n;
}

template <class T>
inline T halfLeft(T x) {
    int n = x.size()/2;
    return x << n;
}

void TestBitset::testCombineAddresses() {
    auto p = std::unique_ptr<double>(new double),
         q = std::unique_ptr<double>(new double);

    auto pi = (size_t) reinterpret_cast<uintptr_t>(p.get()),
         qi = (size_t) reinterpret_cast<uintptr_t>(q.get());

    std::bitset<8*sizeof(size_t)>
        pb(pi),
        qb(qi),
        pqb = halfRight(pb) | halfLeft(qb);

    size_t pqi = (size_t) pqb.to_ullong();
    /*
    std::cout
        << "p ULL  = " << pi << std::endl
        << "q ULL  = " << qi << std::endl << std::hex
        << "p hex  = " << pi << std::endl
        << "q hex  = " << qi << std::endl
        << "p bits = " << pb << std::endl
        << "q bits = " << qb << std::endl
        << "p ½    = " << halfRight(pb) << std::endl
        << "q ½    = " << halfLeft(qb) << std::endl
        << "pqb    = " << pqb << std::endl
        << "pq hex = " << pqi << std::endl << std::dec
        << "pqi    = " << pqi << std::endl;
    */
    QCOMPARE(phys_math::hashPointers(p.get(), q.get()), pqi);
}



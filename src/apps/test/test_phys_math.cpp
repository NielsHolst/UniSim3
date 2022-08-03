#include <base/phys_math.h>
#include <base/test_num.h>
#include "test_phys_math.h"

using namespace base;
using namespace phys_math;

void TestPhysMath::testCumNormal() {
    TestNum::eq(cumNormal(-1.96), 0.025);
    TestNum::eq(cumNormal( 1.96), 0.975);
}

void TestPhysMath::testInvCumNormal() {
    TestNum::eq(invCumNormal(0.025), -1.96);
    TestNum::eq(invCumNormal(0.975),  1.96);
}

void TestPhysMath::testInferNormal() {
    QPair<double, double> p = inferNormal(-1.96, 1.96, 0.95);
    TestNum::eqZero(p.first);
    TestNum::eq(p.second, 1.);
}

void TestPhysMath::testInvNormalRange() {
    // cf. RDev/SA/normal-range.R
    TestNum::eq(invNormalRange(0. , 20., 40., 0.95), 20.);
    TestNum::eq(invNormalRange(0.1, 20., 40., 0.95), 24.028);
    TestNum::eq(invNormalRange(0.5, 20., 40., 0.95), 30.);
    TestNum::eq(invNormalRange(0.8, 20., 40., 0.95), 34.040);
    TestNum::eq(invNormalRange(1. , 20., 40., 0.95), 40.);
}



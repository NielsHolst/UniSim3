#include <base/phys_math.h>
#include <base/test_num.h>
#include "test_phys_math.h"

using namespace base;
using namespace phys_math;
using namespace TestNum;

void TestPhysMath::testCumNormal() {
    eq(cumNormal(-1.96), 0.025);
    eq(cumNormal( 1.96), 0.975);
}

void TestPhysMath::testInvCumNormal() {
    eq(invCumNormal(0.025), -1.96);
    eq(invCumNormal(0.975),  1.96);
}

void TestPhysMath::testInferNormal() {
    QPair<double, double> p = inferNormal(-1.96, 1.96, 0.95);
    eqZero(p.first);
    eq(p.second, 1.);
}

void TestPhysMath::testInvNormalRange() {
    // Cf. RDev/SA/normal-range.R
    eq(invNormalRange(0. , 20., 40., 0.95), 20.);
    eq(invNormalRange(0.1, 20., 40., 0.95), 24.028);
    eq(invNormalRange(0.5, 20., 40., 0.95), 30.);
    eq(invNormalRange(0.8, 20., 40., 0.95), 34.040);
    eq(invNormalRange(1. , 20., 40., 0.95), 40.);
}

void TestPhysMath::testSvp() {
    // Cf. Table 2.3 in https://www.fao.org/3/x0490e/x0490e0j.htm#annex%202.%20meteorological%20tables
    eq(svp(25), 3168., 0.1);
}

void TestPhysMath::testSvpSlope() {
    // Cf. Table 2.4 in https://www.fao.org/3/x0490e/x0490e0j.htm#annex%202.%20meteorological%20tables
    eq(svpSlope(25), 189., 0.1);
}


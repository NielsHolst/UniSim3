#include <iostream>
#include <boost/math/tools/minima.hpp>
#include <base/minimise.h>
#include <base/test_num.h>
#include "test_minimise.h"

using boost::math::tools::brent_find_minima;
using namespace base;


struct f {
  double operator()(double x)
  {
      return (x-5)*(x-10);
  }
};

struct g
{
  QObject *_context;

  g(QObject *context) : _context(context) {}

  double operator()(double x)
  {
      return (x-5)*(x-10);
  }
};

void TestMinimise::testBrent() {
    double x,y;
    boost::uintmax_t maxIter = 100;
    try {
        std::pair<double, double> minimum =
                brent_find_minima<f, double>(f(), 0., 30., 16, maxIter);
        x = minimum.first;
        y = minimum.second;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }

    QVERIFY( TestNum::eq(x, 7.5) );
    QVERIFY( TestNum::eq(y, f()(x)) );
    std::cout << "maxIter = " << maxIter << "\n";
}

void TestMinimise::testWrapper() {
    double x,y;
    try {
        QPair<double, double> minimum = minimise(f(), 0, 30, 1e-6, 100, this);
        x = minimum.first;
        y = minimum.second;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }

    QVERIFY( TestNum::eq(x, 7.5) );
    QVERIFY( TestNum::eq(y, f()(x)) );
}

void TestMinimise::testBrentWithArg() {
    double x,y;
    boost::uintmax_t maxIter = 100;
    try {
        std::pair<double, double> minimum =
                brent_find_minima<g, double>(g(this), 0., 30., 16, maxIter);
        x = minimum.first;
        y = minimum.second;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }

    QVERIFY( TestNum::eq(x, 7.5) );
    QVERIFY( TestNum::eq(y, g(this)(x)) );
}

void TestMinimise::testWrapperWithArg() {
    double x,y;
    try {
        QPair<double, double> minimum = minimise(g(this), 0, 30, 1e-6, 100, this);
        x = minimum.first;
        y = minimum.second;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }

    QVERIFY( TestNum::eq(x, 7.5) );
    QVERIFY( TestNum::eq(y, g(this)(x)) );
}



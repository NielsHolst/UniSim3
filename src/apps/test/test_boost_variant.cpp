#include <iostream>
#include <string>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/variant/get.hpp>
#include "test_boost_variant.h"
#include <functional>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"

using std::cout;
namespace x3 = boost::spirit::x3;

struct X3Variant : x3::variant<int, double, std::string> {
    using base_type::base_type;
    using base_type::operator=;
};

struct DerivedBoostVariant : boost::variant<int, double, std::string> {
    using boost::variant<int, double, std::string>::operator=;
};

//int myType(boost::variant<double, int, std::string> var) {
//    try {
//        boost::get<double>(var);
//        return 0;
//    }
//    catch(...) {
//    }
//    try {
//        boost::get<int>(var);
//        return 1;
//    }
//    catch(...) {
//    }
//    try {
//        boost::get<std::string>(var);
//        return 1;
//    }
//    catch(...) {
//    }
//    return -1;
//}

void TestBoostVariant::test_boost_variant() {
    boost::variant<int, double, std::string> var;
    var = 117;
    QCOMPARE(var.which(), 0);
    QCOMPARE(boost::get<int>(var), 117);

    var = 1.17;
    QCOMPARE(var.which(), 1);
    QCOMPARE(boost::get<double>(var), 1.17);

    var = "abc";
    QCOMPARE(var.which(), 2);
    QCOMPARE(boost::get<std::string>(var), "abc");
}

void TestBoostVariant::test_derived_boost_variant() {
    DerivedBoostVariant var;
    var = 117;
    QCOMPARE(var.which(), 0);
    QCOMPARE(boost::get<int>(var), 117);

    var = 1.17;
    QCOMPARE(var.which(), 1);
    QCOMPARE(boost::get<double>(var), 1.17);

    var = "abc";
    QCOMPARE(var.which(), 2);
    QCOMPARE(boost::get<std::string>(var), "abc");
}

void TestBoostVariant::test_x3_variant() {
    X3Variant var;
    var = 117;
    QCOMPARE(var.get().which(), 0);
    QCOMPARE(boost::get<int>(var), 117);

    var = 1.17;
    QCOMPARE(var.get().which(), 1);
    QCOMPARE(boost::get<double>(var), 1.17);

    var = "abc";
    QCOMPARE(var.get().which(), 2);
    QCOMPARE(boost::get<std::string>(var), "abc");
}


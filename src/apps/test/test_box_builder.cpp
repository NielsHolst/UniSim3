#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "test_box_builder.h"
#include "test_box_cases.h"

using namespace base;

void TestBoxBuilder::testBuild1() {
    bool excepted(false);
    std::unique_ptr<Box> root;
    try {
        root = std::unique_ptr<Box>( TestBoxCases::case1() );
        root->initializeFamily();
        root->resetFamily();
        root->cleanupFamily();
        root->debriefFamily();
    }
    catch(const base::Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    catch (const std::exception& ex) {
        QString s = "Unexpected exception: " + QString(ex.what());
        QFAIL(qPrintable(s));
    }
    catch (...) {
        QFAIL("Unexpected exception");
    }
    QVERIFY(!excepted);
//    std::cout << qPrintable(root->toText()) << std::endl;
}

void TestBoxBuilder::testBuild2() {
    bool excepted(false);
    std::unique_ptr<Box> root;
    try {
        root = std::unique_ptr<Box>( TestBoxCases::case2() );
        root->initializeFamily();
        root->resetFamily();
        root->cleanupFamily();
        root->debriefFamily();
    }
    catch(const base::Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    catch (const std::exception& ex) {
        QString s = "Unexpected exception: " + QString(ex.what());
        QFAIL(qPrintable(s));
    }
    catch (...) {
        QFAIL("Unexpected exception");
    }
    QVERIFY(!excepted);
//    std::cout << qPrintable(root->toText()) << std::endl;
}

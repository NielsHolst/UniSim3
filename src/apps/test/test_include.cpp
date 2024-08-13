/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box.h>
#include <base/command.h>
#include "exception_expectation.h"
#include "test_include.h"

using namespace base;

void TestInclude::manyIncludes() {
    bool excepted(false);
    try {
        Command::submit(QStringList() << "run" << "include/test_many_includes.box");
    }
    UNEXPECTED_EXCEPTION;

    QVERIFY(Box::hasRoot());
    Box *root = Box::root();

    try {
        QVERIFY(root->findOne<Box*>("sim/mainA"));
        QVERIFY(root->findOne<Box*>("sim/mainB"));
        QVERIFY(root->findOne<Box*>("sim/mainC"));
        QVERIFY(root->findOne<Box*>("sim/parcelA"));
        QVERIFY(root->findOne<Box*>("sim/parcelB"));
        QVERIFY(root->findOne<Box*>("sim/parcelD"));
        QVERIFY(root->findOne<Box*>("sim/mainB/parcelC"));
    }
    UNEXPECTED_EXCEPTION;
}

void TestInclude::commentedIncludes() {
    bool excepted(false);
    try {
        Command::submit(QStringList() << "run" << "include/test_commented_includes.box");
    }
    UNEXPECTED_EXCEPTION;

    QVERIFY(Box::hasRoot());
    Box *root = Box::root();

    try {
        QVERIFY(root->findOne<Box*>("sim/mainA"));
        QVERIFY(root->findOne<Box*>("sim/mainB"));
        QVERIFY(root->findOne<Box*>("sim/mainC"));
        QVERIFY(root->findOne<Box*>("sim/parcelA"));
        QVERIFY(root->findOne<Box*>("sim/parcelB"));
        QVERIFY(root->findOne<Box*>("sim/parcelD"));
        QVERIFY(root->findOne<Box*>("sim/mainB/parcelC"));
    }
    UNEXPECTED_EXCEPTION;
}

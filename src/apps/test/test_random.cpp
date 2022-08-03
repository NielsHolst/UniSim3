#include <iostream>
#include <QSet>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/track.h>
#include "exception_expectation.h"
#include "test_random.h"

using namespace base;
using namespace std;

void TestRandom::testDrawAt() {
    bool excepted(false);
    int errors = dialog().errorCount();
    cout << "A\n";
    try {
        Command::submit(QStringList() << "load" << "random/draw_at.box", nullptr);
    }
    UNEXPECTED
    cout << "B\n";
    QCOMPARE(errors, dialog().errorCount());

    Box *sim;
    Port *a, *b, *c;
    double a1, b1, c1,
           a2, b2, c2,
           a3, b3, c3;
    try {
        sim = environment().root();
        Box *client = sim->findOne<Box>("client");
        a = client->port("a");
        b = client->port("b");
        c = client->port("c");
    }
    UNEXPECTED
    cout << "C\n";

    // They must all be different, except (d,e) and (f,g) which draw from common boxes
    try {
        cout << "C1\n";
        Track::effectuateOrders();
        cout << "C2\n";
        environment().computationStep(ComputationStep::Initialize);
        cout << "C3\n";
        sim->initializeFamily();
        cout << "C4\n";
        a1 = a->value<double>();
        cout << "C5\n";
        b1 = b->value<double>();
        cout << "C6\n";
        c1 = c->value<double>();
        cout << "C7\n";
    }
    UNEXPECTED
    QSet<double> all;
    all << a1 << b1 << c1;
    QCOMPARE(all.size(), 3);
    cout << "D\n";

    // The atReset and atUserReset must have changed
    try {
        environment().computationStep(ComputationStep::Reset);
        sim->resetFamily();
        Track::resetAll();
        a2 = a->value<double>();
        b2 = b->value<double>();
        c2 = c->value<double>();
    }
    UNEXPECTED
    QCOMPARE(a1, a2);
    QCOMPARE(c1, c2);
    cout << "E\n";

    try {
        environment().computationStep(ComputationStep::Update);
        sim->updateFamily();
        a3 = a->value<double>();
        b3 = b->value<double>();
        c3 = c->value<double>();
    }
    UNEXPECTED
    QCOMPARE(a2, a3);
    QCOMPARE(b2, b3);
    QVERIFY(c2!=c3);
    cout << "F\n";

    // Must run OK
    try {
        sim->run();
    }
    UNEXPECTED
    cout << "G\n";
}

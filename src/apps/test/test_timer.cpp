#include <chrono>
#include <iostream>
#include <thread>
#include <base/box.h>
#include <base/timer.h>
#include "test_timer.h"

using namespace base;
using namespace std::chrono;
using namespace std::this_thread;

void TestTimer::testDuration() {
    auto box = std::unique_ptr<Box>( new Box("box", nullptr));
    Timer timer(box.get());
    timer.addProfile("profile");

    timer.start("profile");
    sleep_for(milliseconds(1200));
    timer.stop("profile");
    double duration = timer.duration("profile");
    QVERIFY(duration>1190.);
    QVERIFY(duration<1210.);

    timer.start("profile");
    sleep_for(milliseconds(1300));
    timer.stop("profile");
    double total = timer.duration("profile");
    QVERIFY(total>2490.);
    QVERIFY(total<2510.);
}



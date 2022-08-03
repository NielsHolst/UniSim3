#include <QStringList>
#include <base/random_order.h>
#include "test_random_order.h"

using namespace base;

void TestRandomOrder::test3() {
    RandomOrder order(3, this);
    QStringList list;
    for (int n=0; n<30; ++n) {
        list.clear();
        for (int i=0; i<3; ++i)
            list << QString::number(order.next());
        QString s = list.join("");
        QVERIFY(s=="012" || s=="021" || s=="102" || s=="120" || s=="201" || s=="210");
        order.shuffle();
    }
}


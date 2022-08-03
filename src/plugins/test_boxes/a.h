#ifndef A_H
#define A_H
#include <base/box.h>
#include <base/path.h>

namespace test_boxes {

class A : public base::Box
{
public:
    A(QString name, base::Box *parent);
private:
    bool pbool;
    int pint;
    double pdouble;
    QString pstring;
    base::Path ppath;
    QVector<int> ints;
    QVector<QString> strings;
};

}
#endif


#ifndef BIG_BANG_H
#define BIG_BANG_H
#include <base/box.h>

namespace demo {

class BigBang : public base::Box
{
public:
    BigBang(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Input
    QString interactionFile;
    bool hasNames;
    // Data
};

}
#endif

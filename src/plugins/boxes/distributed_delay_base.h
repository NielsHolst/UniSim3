/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DISTRIBUTED_DELAY_BASE_H
#define DISTRIBUTED_DELAY_BASE_H
#include <QVector>

namespace base {
    class Box;
};

namespace boxes {

class DistributedDelayBase
{
public:
    DistributedDelayBase(base::Box *parent);
    virtual ~DistributedDelayBase() {}
    void scale(double factor);
    const QVector<double>& take(double proportion);
    void put(const QVector<double> &addition);
    QVector<double> &data();
    double content() const;

protected:
    QVector<double> _x;
    double _xSum;
    base::Box *_parent;

private:
    QVector<double> taken;
};

}
#endif

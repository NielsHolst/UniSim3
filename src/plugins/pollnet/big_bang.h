/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BIG_BANG_H
#define BIG_BANG_H
#include <QVector>
#include <base/box.h>

namespace pollnet {

class BigBang : public base::Box
{
public:
    BigBang(QString name, QObject *parent);
    void amend();
private:
    // Input
    QString connectivityFile;
    // Data
    QVector<Box*> plants, pollinators;
};

}
#endif

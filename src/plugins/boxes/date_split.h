/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DATE_SPLIT_H
#define DATE_SPLIT_H
#include <QDate>
#include <base/box.h>

namespace boxes {

class DateSplit : public base::Box
{
public:
    DateSplit(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    QDate date;
    // Output
    int day, month, year;
};

}

#endif

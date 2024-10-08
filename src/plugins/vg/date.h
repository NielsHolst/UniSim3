/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DATE_H
#define DATE_H
#include <QDate>
#include <base/box.h>

namespace boxes {

class Date : public base::Box
{
public:
    Date(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    QString dateString;
    int day, month, year;
    // Output
    QDate date;
    // Methods
    void fromString();
    void fromDetails();
};

}

#endif

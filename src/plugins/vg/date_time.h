/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DATE_TIME_H
#define DATE_TIME_H
#include <QDate>
#include <base/box.h>

namespace boxes {

class DateTime : public base::Box
{
public:
    DateTime(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    QString dateTimeString;
    int day, month, year, hour, minute, second;
    // Output
    QDateTime dateTime;
    // Methods
    void fromString();
    void fromDetails();
};

}

#endif

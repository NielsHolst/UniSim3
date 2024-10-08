/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TIME_TO_HOURS_H
#define TIME_TO_HOURS_H
#include <QTime>
#include <base/box.h>

namespace boxes {

class TimeToHours : public base::Box
{
public:
    TimeToHours(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    QTime time;
    // Output
    double value;
};

}

#endif

/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ALL_TYPES_H
#define ALL_TYPES_H

#include <base/box.h>
#include <base/bare_date.h>

namespace boxes {

class AllTypes : public base::Box
{
public:
    AllTypes(QString name, Box *parent);
private:
    // Inputs
    bool _bool;
    int _int;
    double _double;
    QString _string;
    QDate date;
    QTime time;
    QDateTime datetime;
    base::BareDate baredate;
    base::Path path;

    QVector<bool> vec_bool;
    QVector<int> vec_int;
    QVector<double> vec_double;
    QVector<QString> vec_string;
    QVector<QDate> vec_date;
    QVector<QTime> vec_time;
    QVector<QDateTime> vec_datetime;
    QVector<base::BareDate> vec_baredate;
};
} //namespace


#endif

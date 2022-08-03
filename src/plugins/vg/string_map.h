/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef UNISIM_STRING_MAP_H
#define UNISIM_STRING_MAP_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <base/exception.h>

#define StringMapEntry(map, tag) map [ #tag ] = tag

namespace vg {

class NamedObject;

template <class T>
class StringMap : public QMap<QString,T>
{
public:
    StringMap();
    QStringList validKeys() const;
    T seek(QString key, const Box *context=0);
};

template <class T>
StringMap<T>::StringMap()
    : QMap<QString,T>()
{
}

template <class T>
QStringList StringMap<T>::validKeys() const {
    return QStringList(QMap<QString, T>::keys());
}

template <class T>
T StringMap<T>::seek(QString key, const Box *context) {
    if (!QMap<QString, T>::contains(key)) {
        QString msg = "Unknown key in list. Only these keys are valid: '%1'";
        ThrowException(msg.arg(validKeys().join(","))).value(key).context(context);
    }
    return QMap<QString, T>::value(key);
}

} //namespace

#endif


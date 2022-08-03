/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_HISTORY_H
#define BASE_HISTORY_H
#include <QStringList>
#include <QVector>

namespace base {

class History
{
public:
    History(int bufferSize = 100);
    void add(QString entry);
    QString previous() const;
    QString next();
private:
    int _bufferSize, _last;
    mutable int _index;
    bool _isFull;
    QVector<QString> _buffer;
    int inc(int &i) const;
    int decr(int &i) const;
};

}

#endif

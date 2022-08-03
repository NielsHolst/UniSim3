/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_UNIQUE_NAMES_H
#define BASE_UNIQUE_NAMES_H
#include <QObject>
#include <QStringList>
#include <QList>
#include "node.h"

namespace base {

class Port;

class UniqueNames
{
public:
    UniqueNames(QVector<QString> fullNames);
    UniqueNames(QVector<Port*> ports);
    UniqueNames(QVector<const Port*> ports);
    bool extending();
    void extendByNumber();
    QStringList resolved();
private:
    // Data
    class Entry {
    public:
        Entry(QString fullName, int index);
        bool extendUniqueName();
        void extendByNumber(int i);
        int index() const { return _index; }
        QString uniqueName() const { return _uniqueName; }
        bool hasSameUniqueName(const Entry &a) { return _uniqueName == a._uniqueName; };
    private:
        QStringList _parts;
        QString _uniqueName;
        int _index;
    };

    QVector<Entry> _entries;
    int _nextEntry;
    // Methods
    void sortByName();
    void sortByIndex();
};

}

#endif

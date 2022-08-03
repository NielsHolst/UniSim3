/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <iostream>
#include <port.h>
#include "unique_names.h"

namespace base {

UniqueNames::UniqueNames(QVector<QString> fullNames) {
    int i = 0;
    for (auto fullName : fullNames)
        _entries << Entry{fullName, i++};
}

UniqueNames::UniqueNames(QVector<Port*> ports) {
    int i = 0;
    for (auto port : ports)
        _entries << Entry{port->fullName(), i++};
}

UniqueNames::UniqueNames(QVector<const Port*> ports) {
    int i = 0;
    for (auto port : ports)
        _entries << Entry{port->fullName(), i++};
}

UniqueNames::Entry::Entry(QString fullName, int index) {
    // Keep index
    _index = index;

    // Replace port brackets with a slash
    if (fullName.right(1) == "]") {
        fullName.chop(1);
        fullName.replace("[", "/");
    }

    // Remove unnamed box specs
    fullName.replace("::*", "");

    // Split full names by slashes
    _parts = fullName.split("/");

    // Ignore 'value' port name
    Q_ASSERT(!_parts.isEmpty());
    if (_parts.last() == "value")
        _parts.removeLast();

    // Place first unique name
    extendUniqueName();
}

bool UniqueNames::Entry::extendUniqueName() {
    if (_parts.isEmpty())
        return false;
    if (_uniqueName.isEmpty())
        _uniqueName = _parts.last();
    else
        _uniqueName.prepend(_parts.last() + ".");
    _parts.removeLast();
    return true;
}

void UniqueNames::Entry::extendByNumber(int i) {
    _uniqueName.append("." + QString::number(i));
}

bool UniqueNames::extending() {
    bool extended = false;
    auto head = _entries.begin(),
         tail = _entries.begin(),
         end  = _entries.end();
    ++tail;
    while (tail != end) {
        QString headName = head->uniqueName();
        if (headName == tail->uniqueName())
            extended = head->extendUniqueName() || extended ;

        while (tail != end && headName == tail->uniqueName()) {
            extended = tail->extendUniqueName() || extended ;
            ++head;
            ++tail;
        }
        ++head;
        if (tail != end)
            ++tail;
    }
    return extended;
}

void UniqueNames::extendByNumber() {
    auto head = _entries.begin(),
         tail = _entries.begin(),
         end  = _entries.end();
    ++tail;
    int i;
    while (tail != end) {
        i = 0;
        QString headName = head->uniqueName();
        if (headName == tail->uniqueName())
            head->extendByNumber(i++);

        while (tail != end && headName == tail->uniqueName()) {
            tail->extendByNumber(i++);
            ++head;
            ++tail;
        }
        ++head;
        if (tail != end)
            ++tail;
    }

}

QStringList UniqueNames::resolved() {
    do {
        sortByName();
    } while (extending()) ;

    extendByNumber();

    sortByIndex();
    QStringList result;
    for (auto entry : _entries)
        result << entry.uniqueName();
    return result;
}

void UniqueNames::sortByName() {
    std::sort(_entries.begin(), _entries.end(),
              [](Entry a, Entry b) { return a.uniqueName() < b.uniqueName(); });
}

void UniqueNames::sortByIndex() {
    std::sort(_entries.begin(), _entries.end(),
              [](Entry a, Entry b) { return a.index() < b.index(); });
}


}

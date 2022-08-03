/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "history.h"

namespace base {

History::History(int bufferSize)
    : _bufferSize(bufferSize), _last(-1), _index(-1), _isFull(false)
{
    _buffer.fill(QString(), _bufferSize);
}

void History::add(QString entry) {
    _buffer[inc(_last)] = entry;
    _isFull = _isFull || (_last == _bufferSize-1);
    _index = -1;
}

QString History::previous() const {
    // Empty history
    if (_last == -1)
        return QString();
    // Last entry just added: take that
    if (_index == -1)
        return _buffer.at(_index = _last);
    // Browsing history (previous() or next() called since the last add())
    return _buffer.at(decr(_index));
}

QString History::next() {
    // Empty history
    if (_last == -1)
        return QString();
    // Take the next entry
    // Last entry just added: take the entry after that
    if (_index == -1) {
        _index = _last;
        return _buffer.at(inc(_index));
    }
    // Browsing history (previous() or next() called since the last add())
    return _buffer.at(inc(_index));
}

int History::inc(int &i) const {
    ++i;
    if (_isFull) {
        if (i == _bufferSize) i = 0;
    }
    else {
        if (i == _last+1) i = 0;
    }
    return i;
}

int History::decr(int &i) const {
    --i;
    if (_isFull) {
        if (i == -1) i = _bufferSize - 1;
    }
    else {
        if (i == -1) i = _last;
    }
    return i;
}

}

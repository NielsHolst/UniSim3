/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <QDateTime>
#include <QFileInfo>
#include "box.h"
#include "computation.h"
#include "environment.h"
#include "exception.h"
#include "node.h"

namespace base {

int Exception::_count = 0;

Exception::Exception(QString message)
    : _message(message), _caller(Caller())
{
    ++_count;
    Computation::changeStep(Computation::Step::Ready);
}

Exception& Exception::file(const char *s) {
    _file = s;
    return *this;
}

Exception& Exception::line(int i) {
    _line = i;
    return *this;
}

Exception& Exception::context(const Node *object) {
    _contextDescription = object ? Node::fullName(object) : "";
    return *this;
}

Exception& Exception::context(Node *object) {
    _contextDescription = object ? Node::fullName(object) : "";
    return *this;
}

Exception& Exception::caller(Caller c) {
    _caller = c;
    return *this;
}

Exception& Exception::hint(QString s) {
    _hint = s;
    return *this;
}

Exception& Exception::id(QString s) {
    _id = s;
    return *this;
}

QString Exception::id() const {
    return _id;
}

QString Exception::what() const {
    QString text;

    if (environment().runMode() == Environment::RunMode::WithoutDialog)
        text = "In file: " + environment().currentLoadArg() + "\n";

    text += QString{"Error: %1"}.arg(_message);

    QString description;
    if (_contextDescription.isEmpty()) {
        if (Box::current())
            description += Box::current()->fullName();
        else
            description += "Unknown context";
    }
    else
        description += _contextDescription;

    if (!_value.isNull())
        text += QString("\nValue: '%1'").arg(_value);
    if (!_value1.isNull())
        text += QString("\nValue1: '%1'").arg(_value1);
    if (!_value2.isNull())
        text += QString("\nValue2: '%1'").arg(_value2);
    text += QString("\nObject: %1").arg(description);
    if (!_hint.isEmpty())
        text += "\nHint: " + _hint;
    if (!_file.isEmpty())
        text += QString("\nSource code: %1, line %2").arg(_file).arg(_line);
    if (_caller.caller())
        text += QString("\nCalled by %1\n in %2, line %3").
                arg(Node::fullName(_caller.caller())).arg(_caller.file()).arg(_caller.line());
    if (!dateTime().isNull())
        text += "\nCalendar time: " + convert<QString>(dateTime());
    return text;
}

int Exception::count() {
    return _count;
}

template <> QString Exception::asString(bool v) noexcept {
    return v ? "true" : "false";
}

template <> QString Exception::asString(char v) noexcept {
    return QString(v);
}

template <> QString Exception::asString(const char *v) noexcept {
    return QString(v);
}

template <> QString Exception::asString(QString v) noexcept {
    return v;
}

template <> QString Exception::asString(QDate v) noexcept {
    QString s;
    try {
        s = v.toString(Qt::ISODate);
    }
    catch (...) {
        s = "Invalid date";
    }
    return s;
}

template <> QString Exception::asString(QTime v) noexcept {
    QString s;
    try {
        s = v.toString("hh:mm:ss");
    }
    catch (...) {
        s = "Invalid time";
    }
    return s;
}

template <> QString Exception::asString(QDateTime v) noexcept {
    QString s;
    try {
        s = v.toString("yyyy/MM/dd hh:mm:ss");
    }
    catch (...) {
        s = "Invalid date time";
    }
    return s;
}

QDateTime Exception::dateTime() const noexcept {
    QDateTime result;
    Box *calendar;
    try {
        try {
            calendar = Path("calendar").findMaybeOne<Box*>();
        }
        catch (...) {
            calendar = nullptr;
        }
        if (calendar) {
            Port *port = calendar->peakPort("dateTime");
            if (port) {
                result = port->value<QDateTime>();
            }
        }
    }
    catch (...) {
        result = QDateTime();
    }
    return result;
}

}


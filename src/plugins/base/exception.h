/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <boost/lexical_cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "caller.h"

#define ThrowException(X) \
    throw base::Exception((X)).file(__FILE__).line(__LINE__)

namespace base {

class Node;

class Exception {
public:
    Exception(QString message);
    Exception& file(const char *s);
    Exception& line(int i);
    Exception& context(const Node *object);
    Exception& context(Node *object);
    Exception& caller(Caller c);
    Exception& hint(QString s);
    Exception& id(QString s);
    template <class T> Exception& value(T v);
    template <class T> Exception& value1(T v);
    template <class T> Exception& value2(T v);

    QString id() const;
    QString what() const;
    static int count();
private:
    // data
    QString _message, _contextDescription,
            _value, _value1, _value2, _hint, _fullName, _file, _id;
    Caller _caller;
    int _line;
    static int _count;
    // methods
    template <class T> QString asString(T v) noexcept;
    QDateTime dateTime() const noexcept;
};

template <class T> Exception& Exception::value(T v) {
    _value = asString(v);
    return *this;
}

template <class T> Exception& Exception::value1(T v) {
    _value1 = asString(v);
    return *this;
}
template <class T> Exception& Exception::value2(T v) {
    _value2 = asString(v);
    return *this;
}

template <class T> QString Exception::asString(T v) noexcept {
    QString s;
    try {
        s = QString::fromStdString(boost::lexical_cast<std::string>(v));
    }
    catch (...) {
        s = "Invalid value";
    }
    return s;
}

template <> QString Exception::asString(bool v) noexcept;
template <> QString Exception::asString(char v) noexcept;
template <> QString Exception::asString(const char *v) noexcept;
template <> QString Exception::asString(QString v) noexcept;
template <> QString Exception::asString(QDate v) noexcept;
template <> QString Exception::asString(QTime v) noexcept;
template <> QString Exception::asString(QDateTime v) noexcept;

}
#endif

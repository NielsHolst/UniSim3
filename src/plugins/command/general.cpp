/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/node.h>
#include <base/port.h>
#include "general.h"

using namespace base;

namespace command {

namespace {
    QString typeInfo(Node *node) {
        QString s = node->className();
        Port *port = dynamic_cast<Port*>(node);
        if (port)
            s += " " + port->value().typeName();
        return s;
    }

    int maxWidth(QVector<Node*> nodes) {
        int width(0);
        for (auto node : nodes) {
            int w = typeInfo(node).size();
            if (w > width) width = w;
        }
        return width;
    }
}

QString info(QVector<Node*> nodes) {
    QStringList list;
    int W = maxWidth(nodes);
    for (auto node : nodes) {
        int width = typeInfo(node).size();
        QString filler = QString().fill(' ', W - width + 1);
        QString s = typeInfo(node) + filler + node->fullName();
        list << s;
    }
    return list.join("\n");
}

QString info(Node *node) {
    return info(QVector<Node*>() << node);
}

QString readHeader(QFile &file) {
    QString header = QString(file.readLine()).trimmed();

    auto skipFormats = Box::root()->findMaybeOne<Port*>("OutputR::*[skipFormats]");
    bool readFormatLine = skipFormats && !skipFormats->value<bool>();
    if (readFormatLine)
        file.readLine();

    return header;
}

QStringList readHeadLines(QFile &file, int n) {
    QStringList lines;
    int i = 0;
    while (!file.atEnd() && i<n) {
        lines << QString(file.readLine()).trimmed();
        ++i;
    }
    return lines;
}

QStringList readTailLines(QFile &file, int n) {
    boost::circular_buffer<QString> buffer(n);
    buffer.resize(n);
    QStringList head = readHeadLines(file, n);
    for (auto line : head)
        buffer.push_back(line);
    while (!file.atEnd())
        buffer.push_back( QString(file.readLine()).trimmed() );

    QStringList lines;
    for (auto it = buffer.begin(); it != buffer.end(); ++it)
        lines << *it;
    return lines;
}

void skipLines(QFile &file, int n) {
    QStringList lines;
    int i = 0;
    while (!file.atEnd() && i<n) {
        file.readLine();
        ++i;
    }
}

QStringList readLines(QFile &file, int n) {
    QStringList lines;
    int i = 0;
    while (!file.atEnd() && i<n) {
        lines << QString(file.readLine()).trimmed();
        ++i;
    }
    return lines;
}


QVector<int> getWidths(QStringList lines) {
    QVector<int> widths;
    bool fillWidths = true;
    for (auto columns : lines) {
        int i = 0;
        for (auto column : columns.split("\t")) {
            if (fillWidths)
                widths << column.size();
            else if (column.size() > widths.at(i))
                widths[i] = column.size();
            ++i;
        }
        fillWidths = false;
    }
    return widths;
}

QStringList rightJustify(QStringList lines, QVector<int> widths) {
    QStringList result;
    for (auto columns : lines) {
        QStringList output;
        int i = 0;
        for (auto column : columns.split("\t")) {
            output << column.rightJustified(widths.at(i));
            ++i;
        }
        result << output.join(" ");
    }
    return result;
}


}

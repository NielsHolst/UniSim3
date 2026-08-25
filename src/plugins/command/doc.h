/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DOC_H
#define DOC_H

#include <QFile>
#include <QTextStream>

#include <base/command.h>
#include "base/port_type.h"
#include <base/base_dllexport.h>

namespace base {
    class Box;
    class FactoryPlugIn;
    class Node;
    class Port;
}

namespace command {

class UNI_API doc : public base::Command
{
    class OutputFileStream {
    public:
        OutputFileStream();
        void open(QString fileName);
        void close();

        template <typename T>
        OutputFileStream& operator<<(const T& value)
        {
            _stream << value;
            return *this;
        }

    private:
        QFile _file;
        QTextStream _stream;
    };

public:
    doc(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
    void openFileStream();
    void closeFileStream();
    void writeModules();
    void writeClasses(base::FactoryPlugIn *factory);
    void writeClass(base::FactoryPlugIn *factory, QString className);
    void writePorts(base::Box *box, base::PortType type);
    void writePort(const base::Port *port);
    void writeTypeScript();
    static QString label(QString s, int indent);
    static QString nameValue(QString name, QString value, int indent);
    static QString indent(int width);
    // Data
    OutputFileStream _jsonStream, _typStream;
    QString _currentModule, _currentClass;
    //QStringList _currentFootnotes;
};

}

#endif

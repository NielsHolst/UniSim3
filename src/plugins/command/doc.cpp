/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <limits>
#include <base/box.h>
#include <base/command_help.h>
#include <base/computation.h>
#include <base/dialog_base.h>
#include <base/environment.h>
#include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include <base/publish.h>
#include "doc.h"

using namespace base;

namespace command {

PUBLISH(doc)
HELP(doc, "doc", "generates class documentation files")

inline QString escChars(QString s) {
    return s.replace(QString("\\"), QString("\\\\")).replace(QString("\""), QString("\\\""));
}

doc::OutputFileStream::OutputFileStream() {
}

void doc::OutputFileStream::open(QString fileName) {
    QString filePath = environment().outputFileNamePath(fileName);
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Could not open file").value(filePath);
    _stream.setDevice(&_file);
}

void doc::OutputFileStream::close() {
    dialog().information("Documentation written to " + _file.fileName());
    _file.close();
}

doc::doc(QString name, Box *parent)
    : Command(name, parent)
{
}

void doc::doExecute() {
    if (_args.size() > 1)
        dialog().error("Write 'json' with no arguments");
    _typStream.open("generated_api.typ");
    writeTypeScript();
    _typStream.close();

    _jsonStream.open("generated_api.json");
    writeModules();
    _jsonStream.close();

    Computation::changeStep(Computation::Step::Ready);
}

void doc::writeTypeScript() {
    _typStream << "#import \"../lib/helpers.typ\": class-api\n";
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        QString moduleName = factory->id();
        _typStream << "= Module: " << moduleName << "\n";
        for (const QString &className : factory->inventory()) {
            QString moduleClassName = factory->id() + "::" + className;
            Box *box = MegaFactory::create<Box>(moduleClassName, "dummy");
            if (box) {
                _typStream << "#class-api("
                           << "\"" << moduleName << "\", "
                           << "\"" << className << "\")\n";
                QFileInfo source = QFileInfo(box->sourceCodeFilePathName());
                source = QFileInfo(source.dir().filePath(source.completeBaseName() + ".typ"));
                if (source.exists()) {
                    int n = QString("C:/Work/QDev/UniSim3").size();
                    _typStream << "#include "
                               << "\"" << source.canonicalFilePath().mid(n) << "\"\n";
                }
            }
        }
    }
}

void doc::writeModules() {
    const int tab = 4;
    const char
        head[] = "{\n  \"modules\": {",
        tail[] = "  }\n}";
    _jsonStream << head;
    bool prependComma = false;
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        _currentModule = factory->id();
        _jsonStream << (prependComma ? ",\n" : "\n")
                << label(factory->id(), tab) << "{\n";
        writeClasses(factory);
        _jsonStream << indent(tab) << "}";
        prependComma = true;
    }
    _jsonStream << "\n";
    _jsonStream << tail;
}

void doc::writeClasses(FactoryPlugIn *factory) {
    const int tab = 6;
    _jsonStream << label("classes", tab) << "{";

    bool prependComma = false;
    for (const QString &className : factory->inventory()) {
        _currentClass = className;
        _jsonStream << (prependComma ? ",\n" : "\n")
                << label(className, tab + 2) <<  "{\n";
        writeClass(factory, className);
        _jsonStream << indent(tab + 2) << "}";
        prependComma = true;
    }
    _jsonStream << "\n" << indent(tab + 2) << "}"
            << indent(tab) << "\n";
}

void doc::writeClass(FactoryPlugIn *factory, QString className) {
    const int tab = 10;
    // _currentFootnotes.clear();
    Box *box = MegaFactory::create<Box>(factory->id() + "::" + className, "dummy");
    if (box) {
        _jsonStream << nameValue("id", _currentModule + "::" + className, tab) << ",\n"
                << nameValue("description", escChars(box->help()), tab) << ",\n";

        _jsonStream << label("inputs", tab) << "[";
        writePorts(box, PortType::Input);
        _jsonStream << indent(tab) + "],\n";
        // if (!_currentFootnotes.isEmpty()) {
        //     _jsonStream << indent(tab) << "#text(size: 10pt)[\n";
        //     int i = 0;
        //     for (const QString &footnote: std::as_const(_currentFootnotes)) {
        //         _jsonStream << indent(tab) << QChar('a' + i++) << ") " + footnote + "\n";
        //     }
        //     _jsonStream << indent(tab) << "]\n";
        // }

        _jsonStream << label("outputs", tab) << "[";
        writePorts(box, PortType::Output);
        _jsonStream << indent(tab) + "]\n";
    }
    delete box;
}

void doc::writePorts(Box *box, PortType type) {
    bool prependComma = false;
    for (const Port *port : box->portsInOrder()) {
        if (port->type() == type) {
            _jsonStream << (prependComma ? ",\n" : "\n");
            writePort(port);
            prependComma = true;
        }
    }
    _jsonStream << "\n";
}

void doc::writePort(const Port *port) {
    const int tab = 14;
    QString value = port->unparsedExpression();

    if (value.isEmpty()) {
        bool isVector = port->value().isVector(),
             isPath = port->value().type() == Value::Type::Path,
             isBool = port->value().type() == Value::Type::Bool;
        if (isBool)
            value = port->value<bool>() ? "TRUE" : "FALSE";
        else
        value = (isVector || isPath) ? "null" : port->value<QString>();
    }

    if (port->value().type() == Value::Type::Double) {
        double x = port->value<double>();
        if (x == std::numeric_limits<double>::lowest())
            value = "$-infinity$";
        else if (x == std::numeric_limits<double>::max())
            value = "$infinity$";
    }

    QString defaultValue = escChars(value);
    // const bool createFootnote = (defaultValue.size() > 12);
    // if (createFootnote) {
    //     QString letter = QChar('a' + (int) _currentFootnotes.size());
    //     _currentFootnotes.push_back(defaultValue);
    //     defaultValue = letter + ")";
    // }

    _jsonStream << indent(tab - 2) << "{\n"
            << nameValue("id", _currentModule + "::" + _currentClass + "::" + port->name(), tab) << ",\n"
            << nameValue("name", port->name(), tab) << ",\n"
            << nameValue("type", port->value().typeName(), tab) << ",\n"
            << nameValue("default", defaultValue, tab) << ",\n"
            << nameValue("description", escChars(port->help()), tab) << "\n"
            << indent(tab - 2) << "}";
}

QString doc::label(QString s, int indent) {
    return QString().fill(' ', indent) + "\"" + s + "\" : ";
}

QString doc::nameValue(QString name, QString value, int indent) {
    return label(name, indent) + "\"" + value + "\"";
}

QString doc::indent(int width) {
    return QString().fill(' ', width);
}


}

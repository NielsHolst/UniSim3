/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include <base/publish.h>
#include "help.h"

using namespace base;

namespace command {

PUBLISH(help)
HELP(help, "help", "shows command, plugin or class documentation")

help::help(QString name, Box *parent)
    : Command(name, parent), _parent(nullptr), _useMarkdown(false)
{
    helpText("help");
}

help::~help() {
    delete _parent;
}

void help::doExecute() {
    switch (_args.size() - 1) {
    case 0:
        processArgument("");
        break;
    case 1:
        processArgument(QString(_args.at(1)));
        break;
    case 2:
        processOptions(QString(_args.at(2)));
        processArgument(QString(_args.at(1)));
        break;
    default:
        dialog().error("Write: 'help ?' to see the possible command formats");
    }
}

void help::processOptions(QString options) {
    if (options=="md")
        _useMarkdown = true;
    else
        ThrowException("The only valid option is 'md' for output in Markdown format");
}

void help::processArgument(QString argument) {
    if (argument=="c" || argument=="C" || argument=="command")
        showCommands();
    else if (argument=="p" || argument=="P")
        showPlugins();
    else if (argument=="" || argument=="?")
        showHelp();
    else {
        if (getPlugIn(argument))
            showPlugin();
        else if (createBox(argument))
            showClass();
        else {
            QStringList pluginNames = MegaFactory::find(argument);
            int n = pluginNames.size();
            if (n==0) {
                dialog().error("Unknown name\n"
                           "Write: 'help ?' to see the possible command formats");
            }
            else if (n==1) {
                dialog().error("Unexpected error constructing " + argument + " in factory " + pluginNames.first());
            }
            else {
                for (int i=0; i<n; ++i)
                    pluginNames[i] += "::" + argument;
                dialog().error("Pick a specific class:\n" + pluginNames.join("\n"));
            }
        }

    }
}

void help::showCommands() {
    QString text;
    QTextStream sink(&text);
    for (CommandHelp help : CommandHelp::all()) {
        int width = help.command().size();
        QString filler = QString().fill(' ', CommandHelp::maxWidth() - width + 1);
        sink << help.command() << filler << "-" << help.description() << "\n";
    }
    text.remove(text.size()-1, 1);
    dialog().information(text);
}

void help::showPlugins() {
    // Collect plugin info
    auto plugins = MegaFactory::factories();
    LineInfo info;
    for (int i=0; i < plugins.size(); ++i) {
        QString name = plugins.at(i)->id(),
                desc = createBox(name + "Documentation") ?
                       _box->port("title")->value<QString>() : "undescribed";
        info.addLine(name, desc);
    }
    dialog().information(info.combined().join("\n"));
}

void help::showHelp() {
    QString s = "Use one of these formats:\n"
                "help    -- to show this help overview\n"
                "help ?  -- to show this help overview\n"
                "help c  -- to show all commands\n"
                "help p  -- to show all plugins\n"
                "help <class name>    -- to show specific class documentation\n"
                "help <class name> md -- to show specific class documentation in Markdown format\n"
                "help <plugin name>   -- to show specific plugin documentation\n";
    dialog().information(s);
}

bool help::getPlugIn(QString name) {
    _plugIn = nullptr;
    for (auto factory : MegaFactory::factories()) {
        if (name == factory->id()) {
            _plugIn = factory;
        }
    }
    return _plugIn;
}

bool help::createBox(QString className) {
    try {
        _box = MegaFactory::create<Box>(className, "helpObject", _parent);
    }
    catch (Exception &) {
        _box = nullptr;
    }
    return _box;
}

void help::showPlugin() {
    // Collect info on plugin
    QStringList headerLines;
    QString name = _plugIn->id();
    if (createBox(name+"Documentation")) {
        QString title = _box->port("title")->value().asString();
        int length = title.length();
        QVector<QString>
                authorNames = _box->port("authorNames")->value<QVector<QString>>(),
                authorInstitutes = _box->port("authorInstitutes")->value<QVector<QString>>(),
                authorEmails = _box->port("authorEmails")->value<QVector<QString>>();
        QStringList authors;
        for (int i=0; i < authorNames.size(); ++i) {
            QString line = authorNames.at(i) + ", " +
                   authorInstitutes.at(i) + ", " +
                   authorEmails.at(i);
            if (line.size() > length)
                length = line.size();
            authors << line;
        }
        QString divider;
        divider.fill('-', length);
        headerLines
            << divider
            << title
            << divider
            << "Developed by"
            << authors
            << divider;
    }
    // Collect info on classes
    LineInfo info;
    for (QString name : _plugIn->inventory()) {
        createBox(_plugIn->id() + "::" + name);
        if (!_box)
            ThrowException("Cannot create object of class '" + name + "'");
        info.addLine(name, _box->help());
    }
    QStringList all = headerLines + info.combined();
    dialog().information(all.join("\n"));
}

void help::showClass() {
    setColWidths();
    QString msg;
    if (_useMarkdown) {
        msg += "\n| Inputs | Type | Default | Description |\n"
                 "| ------ | ---- | ------- | ----------- |\n";
    }
    else {
        msg +=  "\n" + _box->className() + " " + _box->help() +
                "\n\nInputs:\n";
    }
    msg += portsHelp(PortType::Input).join("\n");
    if (_useMarkdown) {
        msg += "\n| **Outputs** | | | |\n";
    }
    else {
        msg += "\n\nOutputs:\n";
    }
    msg += portsHelp(PortType::Output).join("\n");

    if (!_box->additionalOutputs().isEmpty()) {
        msg += _useMarkdown ? "\n|*additional*" : QString("\nAdditional").leftJustified(_colWidthName);
        msg += QString("| ").leftJustified(_colWidthType) +
               QString("| ").leftJustified(_colWidthValue + _colWidthUnit) +
               "|" + _box->additionalOutputs();
        if (_useMarkdown)
            msg += "|";
    }
    dialog().information(msg);
    if (_useMarkdown)
        environment().copyToClipboard(msg, "\nMarkdown table copied to clipboard!");
}

namespace {
    bool showValue(const Port *port) {
        return port->type() == PortType::Input;
    }
}

inline QString inBrackets(QString s) {
    return (s.isEmpty() || s[0]!='[') ? "["+s+"]" : s;
}

QString help::escaped(QString s) const {
    if (_useMarkdown) {
        s = s.replace("<", "&#60;");
        s = s.replace(">", "&#62;");
        s = s.replace(":", "&#58;");
        s = s.replace("|", "&#124;");
        s = s.replace("*", "\\*");
        s = s.replace("-", "&minus;");
        s = s.replace("oC", "&#8451;");
    }
    return s;
}

QString help::italics(QString s) const{
    return _useMarkdown ? "*" + s + "*" : s;
}

void help::setColWidths() {
    _colWidthName = _box->additionalOutputs().isEmpty() ? QString("Outputs").size() : QString("additional").size();
    _colWidthType = QString("Type").size();
    _colWidthValue = QString("Default").size();
    _colWidthUnit = 2;
    for (const Port *port : _box->findMany<Port*>(".[*]")) {
        _colWidthName = std::max(_colWidthName, port->objectName().size());
        _colWidthType = std::max(_colWidthType, port->value().typeName().size());
        if (showValue(port)) {
            auto sz = port->unparsedExpression().isEmpty() ? port->value().asString().size() : 8;
            _colWidthValue = std::max(_colWidthValue, sz);
        }
        _colWidthUnit = std::max(_colWidthUnit, inBrackets(port->unit()).size());
    }
}

QStringList help::portsHelp(PortType type) {
    QStringList list;
    for (const Port *port : _box->portsInOrder()) {
        if (port->type() == type) {
            bool isComputed = !port->unparsedExpression().isEmpty();
            QString item;
            item = _useMarkdown ? "|" : ".";
            item +=       port->objectName().leftJustified(_colWidthName) +
                    "|" + port->value().typeName().leftJustified(_colWidthType) + "|";

            if (isComputed && _useMarkdown) {
                item += italics("computed");
            }
            else {
                QString value =  showValue(port) ? port->value().asString() : "";
                item += value.rightJustified(_colWidthValue);
            }

            if (_colWidthUnit > 0) {
                QString unit = " " + inBrackets(port->unit()).leftJustified(_colWidthUnit);
                item += escaped(unit) + "|";
            }

            QString portHelp = escaped(port->help());
            if (isComputed) {
                QString eq = _useMarkdown ? "= " : " = ";
                item += italics(eq + escaped(port->unparsedExpression()));
                if (!portHelp.isEmpty())
                    item += " (" + portHelp + ")";
            }
            else {
                item += portHelp;
            }
            if (_useMarkdown)
                item += "|";
            list << item;
        }
    }
    if (list.isEmpty())
        list << "none";
    return list;
}

QString help::sideEffects() {
    QStringList lines = _box->sideEffects().split("\n");
    QString s;
    for (int i = 0; i < lines.size(); ++i)
        s += QString("(%1) %2\n").arg(i+1).arg(lines.at(i));
    return s;
}

LineInfo::LineInfo()
    : maxWidth(0)
{
}

void LineInfo::addLine(QString name, QString desc) {
    items << Item{name, desc};
    if (name.size() > maxWidth)
        maxWidth = name.size();
}

QStringList LineInfo::combined() {
    QStringList msg;
    for (Item item : items) {
        QString line = item.name.leftJustified(maxWidth+1, ' ') + "-" + item.desc;
        msg << line;
    }
    return msg;
}

}

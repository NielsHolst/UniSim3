/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <base/box.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include "help_class_html.h"

using namespace base;

namespace command {

HelpClassHtml::HelpClassHtml(Box *box)
    : HelpClassBase(box)
{
}

QString HelpClassHtml::header() const {
    return
        "<table id=\"table-interface\">"
          "<thead>"
            "<tr><th>Inputs</th><th>Type</th><th>Default</th><th>Purpose / Expression</th></tr>"
          "</thead>"
          "<tbody>\n";

}

namespace {
    QString wrapCode(QString s) {
        return "<span class=\"table-interface-code\">" + s + "</span>";
    }

    QString replaceBackticks(QString s) {
        QRegularExpression re("`[^`]+`");
        for (;true;) {
            auto match = re.match(s);
            if (match.hasMatch()) {
                QString captured = match.capturedTexts().at(0),
                        replacedText = wrapCode( captured.mid(1, captured.size() - 2) );
                int leftEnd = match.capturedStart(0),
                    rightStart = match.capturedEnd(0);
                s = s.left(leftEnd) +
                    replacedText +
                    s.mid(rightStart);
            }
            else {
                break;
            }

        }
        return s;
    }

    QString replaceDollars(QString s) {
        QRegularExpression re("\\$[^\\$]+\\$");
        for (;true;) {
            auto match = re.match(s);
            if (match.hasMatch()) {
                QString captured = match.capturedTexts().at(0),
                        replacedText = "\\(" + captured.mid(1, captured.size() - 2) + "\\)";
                int leftEnd = match.capturedStart(0),
                    rightStart = match.capturedEnd(0);
                s = s.left(leftEnd) +
                    replacedText +
                    s.mid(rightStart);
            }
            else {
                break;
            }

        }
        return s;
    }
}

QStringList HelpClassHtml::portLines(PortType type) const {
    QStringList list;
    for (const Port *port : _box->portsInOrder()) {
        if (port->type() == type) {
            QString value = isConstant(port) ? port->value().asString() : "-";
            if (!hasIntervalAsUnit(port))
                value += " " + port->unit();

            QString desc,
                    portHelp = replaceDollars( replaceBackticks(port->help()) );


            if (isConstant(port)) {
                desc = portHelp;
                if (hasIntervalAsUnit(port))
                    desc += " " + port->unit();
            } else {
                desc = "&larr; " + wrapCode( port->unparsedExpression() );
                if (!portHelp.isEmpty())
                    desc += " (" + portHelp + ")";
            }

            QStringList cells;
            cells << "<td>" << port->objectName() << "</td>"
                  << "<td>" << port->value().typeName() << "</td>"
                  << "<td>" << value << "</td>"
                  << "<td>" << desc << "</td>";
            list << "<tr>" + cells.join("") + "</tr>";
        }
    }
    return list;
}


QString HelpClassHtml::inputPorts() const {
    QStringList inputs = portLines(PortType::Input);
    return inputs.isEmpty() ?
        "<tr><td colspan=4><span class=\"table-interface-normal\"><em>None</em></span></td></tr>\n" :
        (inputs.join("\n") + "\n");
}

QString HelpClassHtml::outputPorts() const {
    QString s = "<tr><th>Outputs</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n";
    QStringList outputs = portLines(PortType::Output);
    s += outputs.isEmpty() ?
        "<tr><td colspan=4><span class=\"table-interface-normal\"><em>None</em></span></td></tr>\n" :
        (outputs.join("\n") + "\n");
    return s;
}

QString HelpClassHtml::additionalOutputPorts() const {
    QString additional = _box->additionalOutputs();
    return additional.isEmpty() ? "" :
        ("<tr><th>Additionally</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n"
        "<tr><td colspan=4><span class=\"table-interface-normal\">" + replaceBackticks(additional) + "</span></td></tr>\n");
}

QString HelpClassHtml::footer() const {
    return "</tbody></table>";
}

QString HelpClassHtml::sideEffects() const {
    QString side = _box->sideEffects();
    return (side.isEmpty()) ? "" : ("Note the side effects: " + side + "\n");
}

void HelpClassHtml::done(QString info) {
//    dialog().information(info);
    QString filePath = outputFilePath();
    QFile file(filePath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath);
    file.write(qPrintable("<div class=\"begin macro\"></div>\n" + info + "\n<div class=\"end macro\"></div>\n"));
    file.close();
    dialog().information(filePath + " written");
}

namespace {
    void cdSubDir(QDir &dir, QString subDir) {
        if (!dir.cd(subDir)) {
            if (!dir.mkdir(subDir) )
                ThrowException("Cannot create sub-folder").value(subDir).value2(dir.absolutePath());
            if (!dir.cd(subDir))
                ThrowException("Cannot access sub-folder").value(subDir).value2(dir.absolutePath());
        }
    }
}

QString HelpClassHtml::outputFilePath() const {
    // Go to output folder
    QDir dir = environment().resolveDir(Environment::Output);
    // Go to its parent
    if (!dir.cdUp())
        ThrowException("Cannot access output folder");
    cdSubDir(dir, "site");
    cdSubDir(dir, "snippets");
    cdSubDir(dir, "plugins");
    QString plugin    = _box->pedigree().last().namespaceName,
            className = _box->pedigree().last().className;
    cdSubDir(dir, plugin);
    return dir.absoluteFilePath(className.toLower()) + ".html";
}

}

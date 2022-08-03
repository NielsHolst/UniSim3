 /* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMapIterator>
#include <QSet>
#include <QString>
#include "boxscript_preprocessor.h"
#include "dialog.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"

using namespace std;

namespace base {

BoxScriptPreprocessor::BoxScriptPreprocessor()
{
}

QString BoxScriptPreprocessor::preprocess(QString filePath) {
    // Read box script and expand macros
    QString code = readFile(filePath, FileIncludes());
    // Set using directive and comment it out
    MegaFactory::usingPlugin("");
    setUsing(extractUsing(code));
    code = code.replace("#using", "// #using");
    // Replace any macros
    return replaceMacros(code);
}

QString BoxScriptPreprocessor::readFile(QString filePath, FileIncludes earlierIncludes) {
    if (earlierIncludes.isEmpty())
        _origFilePath = filePath;
    _curFilePath = filePath;
    QFile file;
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString msg("Could not open file for reading");
        ThrowException(msg).value(filePath);
    }
    // Make certain code ends with a line break
    QString code = QString(file.readAll());
    if (!code.endsWith("\n"))
        code += "\n";
    // Expand any includes
    return expandIncludes(filePath, code, earlierIncludes);
}

QString BoxScriptPreprocessor::expandIncludes(QString sourceFilePath, QString code, FileIncludes earlierIncludes) {
    // New code is collated while the original code is unchanged
    QString newCode;
    // Find all includes in this file
    Positions includes = findDirective(code, "include");
    bool hasIncludes = !includes.isEmpty();
    // If file has includes then the new code runs until the first include
    if (hasIncludes)
        newCode = code.mid(0, includes.at(0).begin-1);
    // Replace every include with included file
    for (Position include : includes) {
        // Extract the include file path
        QString line = code.mid(include.begin, include.end - include.begin),
                includeFilePath = extractIncludeFilePath(line);
        // Append the include to check list
        FileInclude newFileInclude = FileInclude{sourceFilePath, includeFilePath};
        checkCyclicIncludes(newFileInclude, earlierIncludes);
        earlierIncludes << newFileInclude;
        // Append code from included file, embraced in comments
        QString paths = "(" + sourceFilePath + " -> " + includeFilePath + ")\n";
        newCode +=
                "\n// begin-include " + paths +
                readFile(includeFilePath, earlierIncludes) +
                "\n// end-include "  + paths;
    }
    // If file had include we are new at the end of the last include, or else we are still at the beginning
    int at = hasIncludes ? includes.last().end : 0;
    newCode += code.mid(at);
    // Return the new code
    return newCode;
}

QString BoxScriptPreprocessor::extractUsing(QString code) {
    // Collect the using directives in one set
    QSet<QString> usingPlugin;
    Positions includes = findDirective(code, "using");
    for (Position include : includes) {
        QString line = code.mid(include.begin, include.end - include.begin);
        #if QT_VERSION >= 0x050E00
            QStringList items = line.split(" ", Qt::SkipEmptyParts);
        #else
            QStringList items = line.split(" ", QString::SkipEmptyParts);
        #endif
        if (items.size() != 2)
            ThrowException("Write '#using <plugin>").value(line);
        usingPlugin << items.at(1);
    }
    // Only zero or one member is legal
    QString result;
    switch (usingPlugin.size()) {
    case 0:
        result = "";
        break;
    case 1:
        result = usingPlugin.values().at(0);
        break;
    default:
        ThrowException("Only one #using directive is allowed").
                value(QStringList(usingPlugin.values()).join(", "));
    }
    return result;
}

void BoxScriptPreprocessor::setUsing(QString pluginName) {
    auto plugins = MegaFactory::factories();
    QStringList pluginNames;
    for (int i=0; i < plugins.size(); ++i) {
        pluginNames << plugins.at(i)->id();
    }
    if (pluginName.isEmpty())
        MegaFactory::usingPlugin("");
    else if (pluginNames.contains(pluginName))
        MegaFactory::usingPlugin(pluginName);
    else
        ThrowException("Plugin name in #using directive does not exist").value(pluginName).
                hint("Existing plugins:\n" + pluginNames.join("\n"));
}

BoxScriptPreprocessor::Positions BoxScriptPreprocessor::findDirective(QString code, QString directive) const {
    Positions positions;
    QString find = "#" + directive;
    int at = 0;
    while (true) {
        at = code.indexOf(find, at);
        if (at==-1 || isComment(code.mid(0,at)))
            break;
        int begin = at;
        at = code.indexOf("\n", at);
        positions << Position{begin, at};
    }
    return positions;
}

bool BoxScriptPreprocessor::isComment(QString s) const {
    // Search backwards for comment slashes in this line
    bool comment = false;
    for (int i = s.size()-2; i>=0; --i) {
        if (s.at(i) == QChar::CarriageReturn)
            break;
        else if (s.mid(i,i+1) == "//") {
            comment = true;
            break;
        }
    }
    return comment;
}

QString BoxScriptPreprocessor::extractIncludeFilePath(QString includeLine) const {
    // Retrieve file name
    int at = includeLine.indexOf("#include");
    QString relFilePath = includeLine.mid(at+8).trimmed();
    // Deal with apostrophes
    if (!(relFilePath.startsWith("\"") && relFilePath.endsWith("\""))) {
        ThrowException("File name after #include must be in apostrophes").
                value(includeLine).value2("In " + _curFilePath);
    }
    relFilePath = relFilePath.mid(1,relFilePath.size()-2);
    // Construct absolute file path
    QDir dir = QDir(QFileInfo(_origFilePath).absolutePath());
    return QFileInfo(dir, relFilePath).absoluteFilePath();
}

void BoxScriptPreprocessor::checkCyclicIncludes(FileInclude newFileInclude, FileIncludes earlierIncludes) const {
    for (FileInclude fileInclude : earlierIncludes) {
        if (fileInclude.includeFilePath == newFileInclude.includeFilePath &&
            fileInclude.sourceFilePath  == newFileInclude.sourceFilePath) {
            ThrowException("Cyclical include").value(fileInclude.sourceFilePath).
                    hint(backtrack(earlierIncludes));
        }
    }
}

QString BoxScriptPreprocessor::backtrack(FileIncludes earlierIncludes) const {
    QString track;
    for (FileInclude fileInclude : earlierIncludes) {
        track += "\n" + fileInclude.sourceFilePath + " -> " + fileInclude.includeFilePath;
    }
    return track;
}

QString BoxScriptPreprocessor::replaceMacros(QString code) {
    QMap<QString, QString> definitions = extractMacros(code);
    QMapIterator<QString, QString> def(definitions);
    while (def.hasNext()) {
        def.next();
        code = code.replace("$"+def.key(), def.value());
        code = code.replace("#define", "// Effectuated macro: ");
    }
    return code;
}


QMap<QString,QString> BoxScriptPreprocessor::extractMacros(QString code) {
    Positions defines = findDirective(code, "define");
    QMap<QString, QString> definitions;
    for (Position define : defines) {
        QString assignment = code.mid(define.begin + 7, define.end - define.begin - 7);
        int splitAt = assignment.indexOf("=");
        if (splitAt == -1)
            ThrowException("#define line must contain an equal sign").value(assignment).value2("In " + _curFilePath);
        QString name  = assignment.mid(0,splitAt).trimmed(),
                value = assignment.mid(splitAt+2).trimmed();
        definitions[name] = value;
    }
    return definitions;
}

} // namespace

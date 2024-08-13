/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
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

namespace {

QString getSlice(QString source, int from, int to) {
    const int n = to - from +1;
    return (n > 0) ? source.mid(from, n) : "";
}

}

QString BoxScriptPreprocessor::expandIncludes(QString sourceFilePath, QString code, FileIncludes earlierIncludes) {
    // New code is collated while the original code is unchanged
    QString newCode;
    // Find all includes in this file
    Positions includes = findDirective(code, "include");
    // Current position in source file
    int pos = 0;
    // Replace every include with included file
    for (Position include : includes) {
        // Copy original source from current position until beginning of next include
        newCode += getSlice(code, pos, include.begin - 1);

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
        pos = include.end + 1;
    }
    // Copy rest of original
    newCode += code.mid(pos);
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
    for (int pos = code.indexOf(find); pos > -1; pos = code.indexOf(find, pos)) {
        // Find line beginning
        int beginLine = pos;
        QChar ch;
        for (int i = pos; i>-1; --i)
            ch = code.at(i);
        while (code.at(beginLine) != '\n' && beginLine > 0)
            --beginLine;
        // Is the directive commented?
        const bool isCommented = (code.mid(beginLine, pos - beginLine).indexOf("//") > -1);
        // Extract directive
        const int begin = pos,
                  end   = code.indexOf("\n", pos);
        pos = end;
        // Only add uncommented directive
        if (!isCommented)
            positions << Position{begin, end};
    }
    return positions;
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

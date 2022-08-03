/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include "altova_xml.h"
#include "environment.h"
#include "exception.h"

namespace base {

inline QString embrace(QString s) {
    return "\"" + s + "\"";
}

AltovaXml::AltovaXml(QObject *parent)
    : QObject(parent),
      _xsltFileName("excel-to-vg.xsl")
{
    QString folder = QApplication::applicationDirPath();
    _searchPath
            <<  folder
            << (folder + "/..")
            << (folder + "/../..");
}

void AltovaXml::run(QString inputFilePath,
                    QString outputFilePath)
{
    QProcess *process = new QProcess(this);
    QString message("XML translation failed");
    QStringList args = QStringList()
        << "/xslt2" << xsltPath()
        << "/in" << inputFilePath
        << "/out" << outputFilePath;

    process->start(embrace(altovaPath()), args);

    if (process->waitForStarted()) {
        if (!process->waitForFinished()) {
            ThrowException(message);
        }
    }
}

QString AltovaXml::xsltPath() {
    QDir dir = environment().resolveDir(Environment::Input);
    QString filePath = dir.absoluteFilePath("scripts/"+_xsltFileName);
    if (!QFile::exists(filePath)) {
        QString message("Cannot find xslt script: %1");
        ThrowException(message.arg(filePath));
    }
    return filePath;
}

QString AltovaXml::altovaPath() {
    QStringList fullPaths;
    for (const QString &path : _searchPath) {
        QString fullPath = QFileInfo(path + "/AltovaXML.exe").absoluteFilePath();
        fullPaths << fullPath;
        if (QFile::exists(fullPath))
            return fullPath;
    }
    ThrowException("Cannot find AltovaXML.exe on any search path").value("\n" + fullPaths.join("\n"));
}


}

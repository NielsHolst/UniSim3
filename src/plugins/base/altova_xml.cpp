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
    : QObject(parent)
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
        << "/xslt2" << xsltPath(xsltFileName(inputFilePath))
        << "/in" << inputFilePath
        << "/out" << outputFilePath;

    process->start(embrace(altovaPath()), args);

    QMessageBox::information(nullptr, "test", embrace(altovaPath()) + "\n" + args.join("\n"));

    if (process->waitForStarted()) {
        if (!process->waitForFinished()) {
            ThrowException(message);
        }
    }
}

QString AltovaXml::xsltFileName(QString inputFilePath) {
    QFile file(inputFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        ThrowException("Cannot open XML input file").value(inputFilePath);

    enum {Version2, Version3, NotFound} version = NotFound;
    int i = 0;
    while (++i<10 && version==NotFound) {
        QString line =  file.readLine();
        if (line.contains("<DVV_SETUP>"))
            version = Version2;
        else if (line.contains("<VG_SETUP>"))
            version = Version3;
    }
    if (version == NotFound)
        ThrowException("XML root must be names <DVV_SETUP> or <VG_SETUP>").value(inputFilePath);
    return (version == Version2) ? "excel-to-vg.xsl" : "excel-to-vg-3.xsl";
}

QString AltovaXml::xsltPath(QString xsltFileName) {
    QDir dir = environment().resolveDir(Environment::Input);
    QString filePath = dir.absoluteFilePath("scripts/" + xsltFileName);
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

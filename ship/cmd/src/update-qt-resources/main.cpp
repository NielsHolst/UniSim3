#include <exception>
#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include "../common/common.h"

using namespace std;

QStringList _dataFiles;

QString pathResources() {
    return pathProject() + "/src/resources";
}


bool argsOk(int argc) {
    if (argc == 5)
        return true;
    cout << "Four arguments are needed";
    return false;
}

bool updateVersion(char *argv[]){
    QString
        major = argv[1],
        minor = argv[2],
        sub = argv[3],
        config = argv[4];
    QFile file(pathResources() + "/version.txt");
    bool ok = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    QString versionInfo = major + "\t" + minor + "\t" + sub + "\t" + config;
    file.write(qPrintable(versionInfo));
    return true;
}

bool collectDataFiles(QDir dir) {
    QDirIterator it(dir);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        QString fileName = info.fileName(),
                filePath = info.absoluteFilePath();
        if (fileName == "." || fileName == "..")
            continue;
        if (info.isDir())
            collectDataFiles(QDir(filePath));
        else {
            int pos = filePath.indexOf("/src/resources/data/");
            if (pos == -1) {
                cout << "Cannot find '/src/resources/data/' in '"
                     << qPrintable(filePath) << "'";
                return false;
            }
            QString relativeFilePath = filePath.mid(pos+15);
            cout << qPrintable(relativeFilePath + "\n");
            _dataFiles << relativeFilePath;
        }
    }
    return true;
}

QString body() {
    QString s;
    for (int i = 0; i < _dataFiles.size(); ++i) {
        s += "    <file>" + _dataFiles.at(i) + "</file>\n";
    }
    return s;
}

bool writeQrc() {
    QFile file(pathResources() + "/application.qrc");
    bool ok = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    QString head =
            "<!DOCTYPE RCC>\n<RCC version=\"1.0\">\n"
            "  <qresource>\n"
            "    <file>boxes_head.cson</file>\n"
            "    <file>notepad_grammar_template.xml</file>\n"
            "    <file>version.txt</file>\n",
            tail =
            "  </qresource>\n"
            "</RCC>";
    _dataFiles.clear();
    if (!collectDataFiles(QDir(pathResources() + "/data/input")) ||
        !collectDataFiles(QDir(pathResources() + "/data/vg")))
            return false;
    file.write(qPrintable(head+body()+tail));
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    cout << "Update Qt resources...\n";
    if (!argsOk(argc) || !updateVersion(argv) || !writeQrc())
        return 1;
    return 0;
}


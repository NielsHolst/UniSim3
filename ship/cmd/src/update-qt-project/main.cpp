#include <exception>
#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "../common/common.h"

using namespace std;

bool argsOk(int argc) {
    if (argc == 5)
        return true;
    cout << "Four arguments are needed";
    return false;
}

bool updateConfig(QString major,
                  QString minor,
                  QString sub,
                  QString config) {
    QString version = major + "." + minor + "." + sub;
    if (config != "debug" && config != "release") {
        cout << "Invalid config value";
        return false;
    }

    QFile file(pathProject() + "/src/boxes_common.pri");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    QString s = file.readAll();
    if (s.isEmpty()) {
        cout << "Nothing read from " << qPrintable(file.fileName());
        return false;
    }

    int begin = s.indexOf("# AUTO-CONFIG-BEGIN"),
        end = s.indexOf("# AUTO-CONFIG-END");
    if (begin==-1 || end==-1) {
        cout << "Missing AUTO-CONFIG comment";
        return false;
    }

    QString result =
            s.left(begin) +
            "# AUTO-CONFIG-BEGIN\n" +
            "CONFIG += " + config + "\n" +
            "VERSION = " + version + "\n" +
            "CONFIG += skip_target_version_ext\n" +
            s.mid(end);

    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    file.write(qPrintable(result));
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (!argsOk(argc) || !updateConfig(argv[1], argv[2], argv[3], argv[4]))
        return 1;
    return 0;
}


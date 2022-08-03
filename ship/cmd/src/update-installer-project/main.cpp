#include <exception>
#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "../common/common.h"

using namespace std;

static QFile file;
static QString content, version, version1, version2;

bool argsOk(int argc) {
    if (argc == 4)
        return true;
    cout << "Three arguments are needed";
    return false;
}

void setVersion(char *argv[]) {
    version1 = QString(argv[1]);
    version2 = QString(argv[1]) + "." + argv[2];
    version  = QString(argv[1]) + "." + argv[2] + "." + argv[3];
}

bool readContent() {
    file.setFileName(pathProject() + "/ship/UniversalSimulator.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    content = file.readAll();
    if (content.isEmpty()) {
        cout << "Nothing read from " << qPrintable(file.fileName());
        return false;
    }
    return true;
}

bool writeFile(QString s) {
    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    file.write(qPrintable(s));
    file.close();
    return true;
}

bool updateXmlVersion() {
    if (!readContent())
        return false;

    int begin = content.indexOf("<version>"),
        end = content.indexOf("</version>");

    if (begin==-1 || end==-1) {
        cout << "Missing <version> element";
        return false;
    }

    QString result =
            content.left(begin) +
            "<version>" + version + content.mid(end);

    return writeFile(result);
}

bool updateLibVersion() {
    QString pathBaseFileName = "/users/nielsholst/lib/libuniversal_simulator_base";
    if (!readContent())
        return false;
    int begin= content.indexOf("<origin>"+pathBaseFileName),
        pos = content.size() - begin,
        beginList = content.lastIndexOf("<distributionFileList>", -pos),
        endList = content.indexOf("</distributionFileList>", beginList);
    if (begin==-1) {
        cout << qPrintable("Missing: <origin>" + pathBaseFileName);
        return false;
    }
    if (beginList==-1 || endList==-1) {
        cout << "Missing <distributionFileList> element";
        return false;
    }
    QString result =
            content.left(beginList) +
            "<distributionFileList>\n"
            "  <distributionFile>\n"
            "    <origin>" + pathBaseFileName +"." + version + ".dylib</origin>\n" +
            "  </distributionFile>\n" +
            "  <distributionFile>\n"
            "    <origin>" + pathBaseFileName +"." + version2 + ".dylib</origin>\n" +
            "  </distributionFile>\n" +
            "  <distributionFile>\n"
            "    <origin>" + pathBaseFileName +"." + version1 + ".dylib</origin>\n" +
            "  </distributionFile>\n" +
            "  <distributionFile>\n"
            "    <origin>" + pathBaseFileName + ".dylib</origin>\n" +
            "  </distributionFile>\n" +
            content.mid(endList);

    return writeFile(result);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    bool ok = false;
    if (argsOk(argc)) {
        setVersion(argv);
        if (updateXmlVersion() && updateLibVersion())
            ok = true;
    }
    if (!ok)
        cout << "\nError in update-installer-project.\n";

    return ok ? 0 : 1;
}


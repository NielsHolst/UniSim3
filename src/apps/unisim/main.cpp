#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include "QProcess"
#include <QProgressDialog>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <base/altova_xml.h>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog_minimal.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/organisation.h>
#include "main_window.h"

using namespace std;
using namespace base;

void myMsgHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    QMessageBox::warning(nullptr, "Unexpected error", msg);
}

int runWithDialog() {
    MainWindow window;
    window.show();
    window.init();
    return qApp->exec();
}

bool canWriteTo(QString fileName) {
    QFile f(fileName);
    bool ok = f.open(QIODevice::WriteOnly);
    f.close();
    return ok;
}

QString translateInputXml(QString inputFilePath) {
    AltovaXml altova;

    QFileInfo fi(inputFilePath);
    QString path = fi.absolutePath(),
            fileName = fi.fileName();

    if (fi.suffix().toLower() != "xml")
        ThrowException("Input must be an XML file").value(inputFilePath);

    QDir dir(path);
    if (!dir.exists())
        ThrowException("Input folder does not exist:\n" + path);
    dir.cdUp();
    dir.mkdir("temp");
    dir.cd("temp");
    if (!dir.exists())
        ThrowException("Temporary folder does not exist:\n" + dir.absolutePath());

    QString outputFilePath = dir.absoluteFilePath(fileName);
    altova.run(inputFilePath, outputFilePath);
    return outputFilePath;
}

QStringList extractArguments(int argc, char *argv[]) {
    QStringList list;
    for(int i = 1; i < argc; ++i) {
        list << argv[i];
    }
    if (argc != 4)
        ThrowException("Wrong number or arguments:\n"+list.join("\n"));
    return list;
}

QString destinationFilePath(QStringList args) {
    return QFileInfo(args.at(2)).absoluteFilePath();
}

void clearDestination(QStringList args) {
    QString filePath = destinationFilePath(args);
    if (QFile::exists(filePath)) {
        if (!QFile::remove(filePath))
            QMessageBox::warning(nullptr, "Error", "Cannot access output file:\n"+filePath);
    }
}

void copyFile(QString fromFilePath, QString toFilePath) {
    if (!QFile::copy(fromFilePath, toFilePath)) {
        QString message("Cannot copy output file\nFrom: %1\nTo: %2");
        ThrowException(message.arg(fromFilePath).arg(toFilePath));
    }
}
void writeErrorToOutputFile(QString errorMessage, QStringList args) {
    QString filePath = destinationFilePath(args);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        ThrowException("Cannot open output file").value(filePath);
    QTextStream stream(&file);
    stream << "!\n" << errorMessage;
}

int runWithoutDialog(int argc, char *argv[]) {
    DialogMinimal *dialog = new DialogMinimal(qApp);
    QStringList args = extractArguments(argc, argv);
    try {
        environment().checkInstallation();

        QString command = args.at(0).toLower();
        if (command!="run" && command!="doc")
            ThrowException("Use 'run' or 'doc' as first argument").value(command).value2(args.join("\n"));

        QString inputFilePath = args.at(1);
        clearDestination(args);

        QString translatedFilePath = translateInputXml(inputFilePath);

        Command::submit(QStringList() << "set" << "folder" << "work" << "HOME");
        Command::submit(QStringList() << "set" << "folder" << "input" << "input");
        Command::submit(QStringList() << "set" << "folder" << "output" << "output");

        dialog->resetErrorCount();
        Command::submit(QStringList() << command << translatedFilePath);
        if (dialog->errorCount() > 0)
            ThrowException(dialog->getError());

        Box *sim = Box::root();
        if (!sim)
            ThrowException("No XML script loaded");
        Box *outputText = sim->findOne<Box*>("output/text");
        QString outputFilePath = outputText->port("filePath")->value<QString>();
        copyFile(outputFilePath, destinationFilePath(args));
    }
    catch (Exception &ex){
        writeErrorToOutputFile(ex.what(), args);
        QMessageBox::warning(nullptr, "Error", ex.what());
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMsgHandler);
    QApplication app(argc, argv);
    app.setObjectName("application");

    bool hasArguments = (argc > 1);
    int result;
    try {
        result = hasArguments ? runWithoutDialog(argc, argv) : runWithDialog();
    }
    catch (Exception &ex){
        result = 1;
        QMessageBox::warning(nullptr, "Error", ex.what());
    }
    catch (...) {
        QMessageBox::warning(nullptr, "Error", "Unknown exception");
    }

    return result;
}

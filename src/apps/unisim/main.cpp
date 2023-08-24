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
    environment().runMode(Environment::RunMode::WithDialog);
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

QStringList extractArguments(int argc, char *argv[]) {
    QStringList list;
    for(int i = 1; i < argc; ++i) {
        list << argv[i];
    }
    if (argc != 4)
        ThrowException("Wrong number or arguments:\n"+list.join("\n"));
    return list;
}

void clearOutputFile(QString filePath) {
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
void writeErrorToOutputFile(QString errorMessage, QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        ThrowException("Cannot open output file").value(filePath);
    QTextStream stream(&file);
    stream << "!\n" << errorMessage;
}

int runWithoutDialog(int argc, char *argv[]) {
    environment().runMode(Environment::RunMode::WithoutDialog);

    DialogMinimal *dialog = new DialogMinimal(qApp);
    QStringList args = extractArguments(argc, argv);
//    QStringList args = {"run",
//                        "C:/MyDocuments/QDev/UniSim3/input/demo/butterfly.box",
//                        "C:/MyDocuments/QDev/UniSim3/output/butterfly.txt"};
    QString
        inputFilePath  = QFileInfo(args.at(1)).absoluteFilePath(),
        outputFilePath = QFileInfo(args.at(2)).absoluteFilePath();
    try {
        environment().checkInstallation();

        QString command = args.at(0).toLower();
        if (command!="run" && command!="doc")
            ThrowException("Use 'run' or 'doc' as first argument").value(command).value2(args.join("\n"));

        clearOutputFile(outputFilePath);

        Command::submit(QStringList() << "set" << "folder" << "work" << "HOME");
        Command::submit(QStringList() << "set" << "folder" << "input" << "input");
        Command::submit(QStringList() << "set" << "folder" << "output" << "output");

        dialog->resetErrorCount();
        Command::submit(QStringList() << command << inputFilePath);
        if (dialog->errorCount() > 0)
            ThrowException(dialog->getError());

        Box *sim = Box::root();
        if (!sim)
            ThrowException("No XML script loaded");
        Box *writer = sim->findOne<Box*>("outputWriter");
        QString writerFilePath = writer->port("filePath")->value<QString>();
        copyFile(writerFilePath, outputFilePath);
    }
    catch (Exception &ex){
        writeErrorToOutputFile(ex.what(), outputFilePath);
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

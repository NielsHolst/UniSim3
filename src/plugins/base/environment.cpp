/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSet>
#include <QSettings>
#include <QStandardPaths>
#include "box.h"
#include "command.h"
#include "dialog.h"
#include "environment.h"
#include "port.h"
#include "save_grammar_atom.h"
#include "save_grammar_notepad.h"
#include "version.h"


namespace base {

std::unique_ptr<Environment> Environment::_environment = nullptr;

const QString PATH_NOT_SET = "/FolderNotSet";

Environment& environment() {
    if (!Environment::_environment)
        Environment::_environment = std::unique_ptr<Environment>( new Environment );
    return *Environment::_environment;
}

Environment::Environment()
    : _runMode(RunMode::Unknown),_current(nullptr), _isSilent(false), _isUnattended(false)
{
    QSettings settings;
    _isFirstInstallation = !QDir(homePath()).exists();
    if (_isFirstInstallation)
        initDir();
    else
        getDirSettings();
    _latestLoadArg = settings.value("environment/latest-load-arg", QString()).toString();
    _latestInputFilePath = QFileInfo(_latestLoadArg).path();
    _latestOutputFilePath["txt"] = settings.value("environment/latest-output-file-path-txt", QString()).toString();

    for (int i=0; i<10; ++i) {
        QString  key = QString("environment/latest-load-arg-%1").arg(i),
                 value = settings.value(key).toString();
        if (value.isEmpty())
            break;
        _latestLoadArgs.push(value);
    }
    if (_latestLoadArgs.isEmpty() && !_latestLoadArg.isEmpty())
        _latestLoadArgs.push(_latestLoadArg);
}

Environment::~Environment() {
    QSettings settings;
    Folder fo = Folder(0);
    while (true) {
        QString key = "environment/dir" + convert<QString>(fo);
        settings.setValue(key, _dir.value(fo).path());
        if (fo == LastFolder) break;
        fo = Folder(fo+1);
    }
    settings.setValue("environment/latest-load-arg", _latestLoadArg);
    settings.setValue("environment/latest-output-file-path-txt", latestOutputFilePath("txt"));

    int i=0;
    QSet<QString> unique;
    while (!_latestLoadArgs.isEmpty() && i<10) {
      QString key = QString("environment/latest-load-arg-%1").arg(i++),
              value = _latestLoadArgs.pop();
      if (!unique.contains(value))
        settings.setValue(key, value);
      unique << value;
    }
}

void Environment::runMode(Environment::RunMode mode) {
    _runMode = mode;
}

Environment::RunMode Environment::runMode() const {
    return _runMode;
}

QString Environment::homePath() const {
    QStringList paths =
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    if (paths.isEmpty())
        ThrowException("Cannot find home folder");
    return paths.at(0) + "/UniversalSimulatorHome";

}

QString Environment::openOutputFile(QFile &file, QString extension) {
    // If multiple instances are running there is an off chance that the file path already exists
    // The forced increment of the counter will most likely but not certainly solve the problem
    QString filePath = outputFilePath(extension);
    while (QFileInfo::exists(filePath)) {
        incrementFileCounter();
        filePath = outputFilePath(extension);
    }
    file.setFileName(filePath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::NewOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath);
    return filePath;
}

QString Environment::outputFilePath(QString extension, int offset) {
    QString fileName = _latestLoadArg;

    char numberFilled[16];
    sprintf(numberFilled, "%04d", fileCountervalue() + offset);

    QString ext = (extension.at(0) == '.') ? extension : ("." + extension);
    return outputFileNamePath(QFileInfo(fileName).completeBaseName() + "_" + numberFilled + ext);
}

QString Environment::outputFileNamePath(QString fileName) {
    // Resolve dir paths in output and in fileName
    QString fileNameDirPath = QFileInfo(fileName).path();
    bool fileNameHasDir = !fileNameDirPath.isEmpty();
    QDir outputDir = resolveDir(Output),
         fileNameDir = QDir(fileNameDirPath);
    bool fileNameHasAbsoluteDir = fileNameHasDir && fileNameDir.isAbsolute();

    // Construct path to output dir
    QString outputDirPath;
    if (fileNameHasDir)
        outputDirPath = fileNameHasAbsoluteDir ?
                        fileNameDir.absolutePath() :
                        outputDir.absolutePath() + "/" + fileNameDirPath;
    else
        outputDirPath = outputDir.absolutePath();
    outputDirPath.replace("\\", "/");

    // Create dir as needed and return complete absolute file name path
    QDir dir = makeDirAsNeeded(outputDirPath);
    QString thePath = dir.absoluteFilePath( QFileInfo(fileName).fileName() );
    _latestOutputFilePath[QFileInfo(fileName).suffix()] = thePath;
    return thePath;
}

QString Environment::filePath(Folder folder, QString fileName) const {
    if (QFileInfo(fileName).isAbsolute())
        return fileName;
    QString fileNamePath = resolveDir(folder).absoluteFilePath(fileName);
    if (!QFileInfo::exists(fileNamePath))
        ThrowException("Could not find file").value(fileNamePath);
    return fileNamePath;
}

QString Environment::folderInfo(Folder folder) {
    QString info;
    QDir folderDir = dir(folder),
         resolvedDir = resolveDir(folder);

    if (folderDir.isRelative()) {
        info = "relative path '%1' resolves to '%2'";
        info = info.arg(folderDir.path()).arg(resolvedDir.absolutePath());
    }
    else {
        info = "absolute path '%1'";
        info = info.arg(folderDir.absolutePath());
    }
    if (!resolvedDir.exists()) {
        if (folder == Output) {
//            info += "\n  '" + resolvedDir.absolutePath() +  "' will be created when needed";
        }
        else {
            bool shouldExist = (folder == Input) ||
                               (folder != Input && folderDir.dirName() != PATH_NOT_SET);
            if (shouldExist)
                info += "\n  Warning: '" + resolvedDir.absolutePath() +  "' does not exist";
            else
                info += "\n  Warning: You have not set this folder";
        }
    }
    return info;
}

QString Environment::latestOutputFilePath(QString fileExtension) const {
    return _latestOutputFilePath.contains(fileExtension) ?
           _latestOutputFilePath.value(fileExtension) :
           QString();
}

void Environment::currentLoadArg(QString arg) {
    _currentLoadArg = arg;
}

QString Environment::currentLoadArg() const {
    return _currentLoadArg;
}

void Environment::current(Box *box) {
    _current = box;
}

Box *Environment::current() const {
    return _current;
}

void Environment::isSilent(bool silent) {
    _isSilent = silent;
}

bool Environment::isSilent() const {
    return _isSilent;
}

void Environment::isUnattended(bool unattended) {
    _isUnattended = unattended;
}

bool Environment::isUnattended() const {
    return _isUnattended;
}

void Environment::latestLoadArg(QString arg) {
    _latestLoadArg = arg;
    if (_latestLoadArgs.isEmpty() || arg != _latestLoadArgs.top())
        _latestLoadArgs.push(arg);
    _latestInputFilePath = QFileInfo(_latestLoadArg).path();
}

QString Environment::latestLoadArg() const {
    return _latestLoadArg;
}

QDir Environment::currentBoxScriptFolder() const {
    QString loadFileNamePath = filePath(Input, _currentLoadArg);
    return QFileInfo(loadFileNamePath).absoluteDir();
}

QString Environment::latestInputFilePath() const {
    return _latestInputFilePath;
}

QStringList Environment::latestLoadArgs() const {
    QStringList latest;
    for (int i=0; i<_latestLoadArgs.size(); ++i)
        latest << _latestLoadArgs.at(i);
    return latest;
}

QString Environment::inputFileNamePath(QString fileName) const {
    // First search locally from current folder and upwards in hierarchy
    QDir local = currentBoxScriptFolder();
    QString fileNamePath;
    bool found;
    do {
        fileNamePath = local.absoluteFilePath(fileName);
        found = QFileInfo(fileNamePath).exists();
        if (found) break;
    } while (local.cdUp());

    // Then look in home folder
    if (!found) {
        QDir home(homePath());
        if (home.cd("input")) {
            fileNamePath = home.absoluteFilePath(fileName);
            found = QFileInfo(fileNamePath).exists();
        }
    }

    // Done
    return found ?
           QDir::cleanPath(fileNamePath) :
           currentBoxScriptFolder().absoluteFilePath(fileName);
}

QString Environment::inputFileContent(QString fileName) const {
    QString fileNamePath = inputFileNamePath(fileName);
    QFile file(fileNamePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        ThrowException("Could not open file").value(fileNamePath);
    return QString( file.readAll() );
}


QDir Environment::dir(Folder folder) const {
    QDir value = _dir.value(folder);
    if (folder == Notepad) {
        if (!value.exists())
            value = findNotepadDir();
    }
    else if (folder == Atom) {
        if (!value.exists())
            value = findAtomDir();
    }
    return value;
}

void Environment::dir(Folder folder, QString path) {
    dir(folder, QDir(path));
}

void Environment::dir(Folder folder, QDir specificDir) {
    _dir[folder] = specificDir;
    if (folder == Input)
        _latestInputFilePath = specificDir.path();
}

QDir Environment::resolveDir(Folder folder, Folder work) const {
    if (folder == Work)
        return dir(work);
    QDir workDir = dir(work);
    QDir specificDir = dir(folder);
    QString path = (specificDir.isAbsolute()) ?
                specificDir.absolutePath() :
                (workDir.absolutePath() + "/" + specificDir.path());
    return QDir(path);
}

QDir Environment::makeDirAsNeeded(QDir dirNeeded) {
    if (!dirNeeded.mkpath("."))
        ThrowException("Could not create folder").value(dirNeeded.path());
    return dirNeeded;
}

void Environment::incrementFileCounter() {
//    QString s = "Environment::incrementFileCounter(), fileCountervalue():%1";
//    dialog().information(s.arg(fileCountervalue()));
    QSettings settings;
    int number = fileCountervalue();
    settings.setValue(fileCounterKey(), ++number);
    settings.sync();
}

int Environment::fileCountervalue() {
    QSettings settings;
    bool ok;
    int number = settings.value(fileCounterKey(), 0).toInt(&ok);
    return ok ? number : 0;
}

QString Environment::fileCounterKey() {
    return "environment/file-counter/" + QFileInfo(_latestLoadArg).completeBaseName();
}

void Environment::copyToClipboard(QString text, QString message) {
    // Write text to file
    QString fileNamePath = resolveDir(Output).absoluteFilePath("clip.txt");
    QFile file(fileNamePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream str(&file);
        str << text;
        file.close();
    }
    // Write text to clipboard
    if (message.isEmpty()) {
        if (!_isUnattended) {
            QApplication::clipboard()->setText(text);
            dialog().information("Executable R snippet copied to clipboard!");
        }
    }
    else {
        QApplication::clipboard()->setText(text);
        dialog().information(message);
    }
}

void Environment::recreateClipboard() {
    QString fileNamePath = resolveDir(Output).absoluteFilePath("clip.txt");
    QFile file(fileNamePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString text = QTextStream(&file).readAll();
        QApplication::clipboard()->setText(text);
        dialog().information("Executable R script copied to clipboard!");
    }
}

void Environment::checkInstallation() const {
    if (isNewInstallation()) {
        // Reconfigure home folder
        dialog().information("New installation detected; reconfiguring HOME folder...");
        int numErrors = Exception::count();
        Command::submit(QStringList() << "reconfigure");
        bool successful = (numErrors == Exception::count());
        if (successful) {
            // Reset work folder to home
//            dialog().information("Work folder set to:");
//            Command::submit(QStringList() << "set" << "folder" << "work" << "HOME", this2);
            // Update the version number
            updateInstallation();
        }
    }
}

bool Environment::isFirstInstallation() const {
    return _isFirstInstallation;
}

bool Environment::isNewInstallation() const {
    QFile file(homePath() + "/.version");
    bool isNew(true);
    if (file.open(QIODevice::Text|QIODevice::ReadOnly)) {
        QString currentVersion = file.readAll();
        isNew = (currentVersion != versionRaw());
    }
    file.close();
    return isNew;
}

void Environment::updateInstallation() const {
    QString fileNamePath = homePath() + "/.version";
    QFile file(fileNamePath);
    if (file.open(QIODevice::Text|QIODevice::WriteOnly))
        file.write(qPrintable(versionRaw()));
    else
        ThrowException("Cannot update version file").value(fileNamePath);
}

bool Environment::isLinux() const {
#ifdef Q_OS_LINUX
    return true;
#endif
    return false;
}

bool Environment::isMac() const {
#ifdef Q_OS_MACOS
    return true;
#endif
    return false;
}

bool Environment::isWindows() const {
    bool isWin = false;
    #ifdef Q_OS_WIN
        isWin = true;
    #endif
    return isWin;
}

void Environment::initDir() {
    _dir[Work] = QDir(homePath());
    _dir[Input] = QDir("input");
    _dir[Output] = QDir("output");
    _dir[Atom] = findAtomDir();
    _dir[Notepad] = findNotepadDir();
    _dir[Graphviz] = findGraphvizDir();
}

//inline QDir homeDir() { return QDir(QStandardPaths::locate(QStandardPaths::HomeLocation, "", QStandardPaths::LocateDirectory)); }
inline QDir homeDir() { return QDir::home(); }

QDir Environment::findAtomDir() const {
    QString path = ".atom/packages/language-boxes/grammars";
    QDir dir = homeDir().filePath(path);
    if (!dir.exists()) {
        if (homeDir().mkpath(path)) {
            try {
                SaveGrammarAtom().write();
            }
            catch (Exception &) {
            }
        }
    }
    dir.setPath(homeDir().filePath(path));
    if (!dir.exists())
        dir.setPath("/Users/user-name/"+path);
    return dir;
}

QDir Environment::findNotepadDir() const {
    QString path = "AppData/Roaming/Notepad++";
    QDir dir = homeDir().filePath(path);
    if (!dir.exists()) {
        if (homeDir().mkpath(path)) {
            try {
                SaveGrammarNotepad().write();
            }
            catch (Exception &) {
            }
        }
    }
    dir.setPath(homeDir().filePath(path));
    if (!dir.exists())
        dir.setPath("/Users/user-name/"+path);
    return dir;
}

QDir Environment::findGraphvizDir() const {
    QString path = PATH_NOT_SET;
    return QDir(path);
}

void Environment::getDirSettings() {
    QSettings settings;
    Folder fo = Folder(0);
    while (true) {
        QString key = "environment/dir" + convert<QString>(fo);
        _dir[fo] = QDir(settings.value(key, QString(".")).toString());
        if (fo == LastFolder) break;
        fo = Folder(fo+1);
    }   
    if (_dir[Atom]==QDir(".") || !_dir[Atom].exists())
        _dir[Atom] = findAtomDir();
    if (_dir[Notepad]==QDir(".") || !_dir[Notepad].exists())
        _dir[Notepad] = findNotepadDir();
}

#define FOLDER_CASE(X) \
    case Environment::X: \
        s = #X; \
        break

#define FOLDER_ENTRY(X) \
    _map[#X] = Environment::X

namespace {
    const QMap<QString, Environment::Folder>& folderMap() {
        static QMap<QString, Environment::Folder> _map;
        if (_map.isEmpty()) {
            FOLDER_ENTRY(Work);
            FOLDER_ENTRY(Input);
            FOLDER_ENTRY(Output);
            FOLDER_ENTRY(Notepad);
            FOLDER_ENTRY(Atom);
            FOLDER_ENTRY(Graphviz);
        }
        return _map;
    }
}


template<> QString convert(Environment::Folder folder) {
    QString s;
    switch (folder) {
        FOLDER_CASE(Work);
        FOLDER_CASE(Input);
        FOLDER_CASE(Output);
        FOLDER_CASE(Notepad);
        FOLDER_CASE(Atom);
        FOLDER_CASE(Graphviz);
    }
    return s;
}

template<> Environment::Folder convert(QString s) {
    s = s.toLower();
    s[0] = s.at(0).toUpper();
    if (folderMap().contains(s))
        return folderMap().value(s);
    ThrowException("Unknown folder").value(s);
}

void Environment::option(QString name, QVariant value) {
    _options[name] = value;
}

QVariant Environment::option(QString name) {
    return _options.contains(name) ? _options.value(name) : QVariant();
}

}


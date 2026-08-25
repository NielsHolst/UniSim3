/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_ENVIRONMENT_H
#define BASE_ENVIRONMENT_H

#include <QDir>
#include <QMap>
#include <QStack>
#include <QVariant>

#include "convert.h"
#include "exception.h"
#include "base_dllexport.h"

namespace base {

class Box;
class Command;

class UNI_API Environment {
public:
    enum Folder {Work, Input, Output, Notepad, Atom, Graphviz};
    const Folder LastFolder = Graphviz;
    enum class RunMode {Unknown, WithDialog, WithoutDialog};

    Environment();
    ~Environment();

    void runMode(RunMode mode);
    RunMode runMode() const;

    QString homePath() const;

    QString openOutputFile(QFile &file, QString extension);
    QString outputFilePath(QString extension, int offset=0);
    QString outputFileNamePath(QString fileName);

    QString filePath(Folder folder, QString fileName) const;
    QString folderInfo(Folder folder);

    void currentLoadArg(QString arg);
    QString currentLoadArg() const;

    void current(Box *box);
    Box *current() const;

    void isSilent(bool silent);
    bool isSilent() const;

    void isUnattended(bool unattended);
    bool isUnattended() const;

    QString latestOutputFilePath(QString fileExtension) const;
    void latestLoadArg(QString arg);
    QString latestLoadArg() const;
    QStringList latestLoadArgs() const;

    QDir currentBoxScriptFolder() const;
    QString latestInputFilePath() const;
    QString inputFileNamePath(QString fileName) const;
    QString inputFileContent(QString fileName) const;

    QDir dir(Folder folder) const;
    void dir(Folder folder, QString path);
    void dir(Folder folder, QDir specificDir);
    QDir resolveDir(Folder folder, Folder work = Work) const;

    void incrementFileCounter();
    void copyToClipboard(QString text, QString message="");
    void recreateClipboard();

    void checkInstallation() const;
    bool isFirstInstallation() const;
    bool isNewInstallation() const;
    void updateInstallation() const;

    bool isLinux() const;
    bool isMac() const;
    bool isWindows() const;

    void option(QString name, QVariant value);
    QVariant option(QString name);

    static QDir makeDirAsNeeded(QDir dirNeeded);
private:
    // Data
    RunMode _runMode;
    Box *_current;
    QMap<Folder, QDir> _dir;
    QMap<QString,QString> _latestOutputFilePath;
    QString _latestLoadArg, _currentLoadArg, _latestInputFilePath;
    bool _isFirstInstallation, _isSilent, _isUnattended;
    QMap<QString, QVariant> _options;
    QStack<QString> _latestLoadArgs;
    // Methods
    QString fileCounterKey();
    int fileCountervalue();
    void initDir();
    QDir findAtomDir() const;
    QDir findNotepadDir() const;
    QDir findGraphvizDir() const;
    void getDirSettings();
};

UNI_API Environment& environment();

template<class T> T convert(Environment::Folder )  { ThrowException("Cannot only convert Folder to QString"); }

template<> UNI_API QString convert(Environment::Folder transform);
template<> UNI_API Environment::Folder convert(QString s);

}

#endif

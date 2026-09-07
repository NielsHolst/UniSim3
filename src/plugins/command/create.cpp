/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDir>
#include <QFileInfo>
#include <base/command_help.h>
#include <base/dialog_base.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "create.h"

using namespace base;

namespace command {

PUBLISH(create)
HELP(create, "create", "creates source code templates")

create::create(QString name, Box *parent)
    : Command(name, parent)
{
}

void create::doExecute() {
    extractPluginName();

    setWorkFolder();
    createFolder();
    writeProjectFile();
    writeDocumentationFileHeader();
    writeDocumentationFileSource();
    writeExampleHeader();
    writeExampleSource();
    writeBoxscript();
}

void create::extractPluginName() {
    if (_args.size() != 3)
        ThrowException("Write: create plugin <plugin name>");
    _pluginName = _args[2];
    _pluginFolderPath = environment().resolveDir(Environment::Folder::Work).absolutePath() +
                        "/src/plugins/" + _pluginName;
}

void create::setWorkFolder() {
    dialog().information("Set work folder to software development folder\n");
    try {
        Command::submit(QStringList() << "set" << "folder" << "work" << "DEV");
    }
    catch (const base::Exception &ex) {
        dialog().error("Cannot create source code files");
        return;
    }

}

void create::createFolder() {
    dialog().information("\nCreating folder " + _pluginFolderPath);
    if (QFileInfo::exists(_pluginFolderPath))
        ThrowException("Cannot create folder. It already exists").value(_pluginFolderPath);
    QDir dir;
    if (!dir.mkdir(_pluginFolderPath))
        ThrowException("Cannot create folder").value(_pluginFolderPath);
}

void create::writeProjectFile() {
    static const char *text =
R"~(#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = @

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
  @_documentation.h \
  logistic_growth.h \

SOURCES +=  \
  @_documentation.cpp \
  logistic_growth.cpp \
)~";
    writeFile(text, _pluginName + ".pro");
}


void create::writeDocumentationFileHeader() {
    static const char *text =

R"~(#pragma once
#include <base/documentation.h>

namespace @ {

class @Documentation : public base::Documentation
{
public:
    @Documentation(QString name, Box *parent);
};

} //namespace
)~";
    writeFile(text, _pluginName + "_documentation.h");
}

void create::writeDocumentationFileSource() {
    static const char *text =
R"~(#include <base/publish.h>
#include "@_documentation.h"

using namespace base;

namespace @ {

PUBLISH(@Documentation)

@Documentation::@Documentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "@ model";
    authorNames
            << "Author 1"
            << "Author 2"
            << "etc.";
    authorInstitutes
            << "Institute 1"
            << "Institute 2"
            << "etc.";
    authorEmails
            << "email 1"
            << "email 2"
            << "etc.";
}

} //namespace
)~";
    writeFile(text, _pluginName + "_documentation.cpp");
}

void create::writeExampleHeader() {
    static const char *text =
R"~(#pragma once
#include <base/box.h>

namespace @ {

class LogisticGrowth : public base::Box
{
public:
    LogisticGrowth(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double r, N0, dt, K;
    // Outputs
    double dN, N;
    // Methods
    double increment() const;
};

} //namespace
)~";
    writeFile(text, "logistic_growth.h");
}

void create::writeExampleSource() {
    static const char *text =
R"~(#include <base/exception.h>
#include <base/publish.h>
#include "logistic_growth.h"

using namespace base;

namespace @ {

PUBLISH(LogisticGrowth)

LogisticGrowth::LogisticGrowth(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("models logistic growth");
    Input(r).equals(0.02).unit("per day").help("Exponential growth rate");
    Input(K).equals(50.).unit("per km2").help("Carrying capacity");
    Input(N0).equals(3.5).unit("per km2").help("Initial density");
    Input(dt).equals(1.).unit("days").help("Simulation time step");
    Output(N).unit("per km2").help("Density");
    Output(dN).unit("per km2").help("Density increment");
}

void LogisticGrowth::reset() {
    // Don't apply increment at start
    N = N0;
    dN = increment();
}

void LogisticGrowth::update() {
    dN = increment();
    N += dN;
}

double LogisticGrowth::increment() const {
    if (K <= 0)
        ThrowException("Carrying capacity must be > 0").value(K);
    return r*dt*N*(1 - N/K);
}

} //namespace
)~";
    writeFile(text, "logistic_growth.cpp");
}

void create::writeBoxscript() {
    static const char *text =
R"~(#using @
Simulation sim {
  .steps = 120
  LogisticGrowth mouse {
    .r = 0.08
    .K = 4000
    .N0 = 50
  }
  OutputR {
    PageR {
      .xAxis = sim[step]
      PlotR {
        .ports = mouse[N] | mouse[dN]
      }
    }
  }
}
)~";
    writeFile(text, _pluginName + "_logistic_growth.box");
}


void create::writeFile(const char *text, QString fileName) {
    bool isBoxscript = fileName.endsWith(".box");
    QString
        folderPath = isBoxscript ? environment().resolveDir(Environment::Folder::Input).absolutePath() : _pluginFolderPath,
        filePath = folderPath + "/" + fileName;

    dialog().information("Writing file " + filePath);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        ThrowException("Cannot open file for writing").value(filePath);

    QString s = QString(text).replace("@", _pluginName);
    file.write(s.toUtf8());
}

}  //namespace

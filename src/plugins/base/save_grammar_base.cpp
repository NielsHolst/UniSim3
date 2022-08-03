/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "dialog.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "port.h"
#include "save_grammar_base.h"

namespace base {

SaveGrammarBase::SaveGrammarBase() {
}

void SaveGrammarBase::write() {
    openFileStream();
    writeBeginning();
    writePlugins();
    writeClasses();
    writePorts();
    writeAttributes();
    writeConstants();
    writeEnd();
    closeFileStream();
}


void SaveGrammarBase::openFileStream() {
    _file.setFileName(grammarFilePath());
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(grammarFilePath());
    _stream.setDevice(&_file);
}

void SaveGrammarBase::closeFileStream() {
    dialog().information("grammar saved to " + grammarFilePath() + "...");
    _file.close();
}

QStringList SaveGrammarBase::plugInNames() {
    QStringList names;
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command")
            names << factory->id();
    }
    return names;
}

QStringList SaveGrammarBase::classNames() {
    QSet<QString> names;
    names << "Box";
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command") {
            auto inv = factory->inventory();
            #if QT_VERSION >= 0x050E00
                names.unite(QSet<QString>(inv.begin(), inv.end()));
            #else
                names.unite(QSet<QString>(inv.toSet()));
            #endif
        }
    }
    QStringList sorted = names.values();
    sorted.sort();
    return sorted;
}

QStringList SaveGrammarBase::portNames() {
    // Set of port names of all classes
    QSet<QString> names;

    // Create an object of each class
    std::unique_ptr<Node> node;
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command") {
            for (auto className : factory->inventory()) {
                node = std::unique_ptr<Node>( factory->create(className, className.toLower(), nullptr) );
                names |= collectPortNames(dynamic_cast<Box *>(node.get()));
            }
        }
    }
    // Pool port names

    // Sort port names
    #if QT_VERSION >= 0x050E00
        QStringList sorted = QStringList( QList<QString>(names.begin(), names.end()) );
    #else
        QStringList sorted = QStringList( QList<QString>(names.toList()) );
    #endif
    sorted.sort();
    return sorted;
}

QSet<QString> SaveGrammarBase::collectPortNames(Box *box) {
    QSet<QString> names;
    if (box) {
        for (auto port : box->portsInOrder())
            names << port->objectName();
    }
    return names;
}

QStringList SaveGrammarBase::attributeNames() {
    return QStringList(); // unused
}

QStringList SaveGrammarBase::transformNames() {
    return QStringList(); // unused
}

QStringList SaveGrammarBase::constantNames() {
    return QStringList() << "FALSE" << "TRUE";
}

}

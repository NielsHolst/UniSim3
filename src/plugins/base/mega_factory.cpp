/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <typeinfo>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include <QPluginLoader>
#include <QSettings>
#include <QStringList>
#include "box.h"
#include "dialog.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "node.h"

namespace base {

std::unique_ptr<MegaFactory> MegaFactory::_me = nullptr;
QString MegaFactory::_usingPluginName;

MegaFactory::MegaFactory() {
    setObjectName("MegaFactory");
    QDir dir = pluginsDir();
    bool notFound = (dir == QDir("/"));
    if (notFound)
        dialog().error("Could not find plugins folder,\nexpected near here: " +
                       QDir(QApplication::applicationDirPath()).absolutePath());

    dialog().pushMessage();
    foreach (QString filename, dir.entryList(QDir::Files)) {
        QString filePath = dir.absoluteFilePath(filename);
        QPluginLoader loader(filePath);
        FactoryPlugIn *factory = qobject_cast<FactoryPlugIn*>(loader.instance());
        if (factory) {
            dialog().message("loading " + filePath +"...");
            _factories << factory;
            for (QString id : factory->inventory()) {
                productIndex.insert(id, factory);
                QString idWithNamespace = factory->id() + "::" + id;
                productIndex.insert(idWithNamespace, factory);
            }
        }
    }
    dialog().popMessage();
}

QDir MegaFactory::pluginsDir() {
    QDir dir(QApplication::applicationDirPath());
    bool found;
    do {
        found = dir.cd("plugins");
        if (found) break;
        dir.cdUp();
    } while (dir != QDir("/"));
    return dir;
}

void MegaFactory::loadPlugins() {
    me();
}

Node *MegaFactory::createObject(QString className, QString objectName, Box *parent)
{
    FactoryPlugIn *factory;
    Node *creation;
    QStringList classNameParts = className.split("::");
    QString classNameProper = (classNameParts.size() == 1) ? className : classNameParts.at(1),
            namespaceName;

    // Box objects are not created by a factory, because the Box class is defined in the base plug-in
    if (className == "Box" || className == "base::Box") {
        creation = new Box(objectName, parent);
        namespaceName = "base";
    }
    else {
        switch (me().productIndex.count(className)) {
        case 0:
            // Error: Unknown class
            ThrowException("Unknown class").value(className)
                    .value2("\nUncreated object: "+objectName+"\nof parent: "+Node::fullName(parent));
        case 1:
            // Success: Create object
            factory = me().productIndex.value(className);
            creation = factory->create(classNameProper, objectName, parent);
            namespaceName = factory->id();
            break;
        default:
            // Too many candidate factories: Try again with 'using' plugin name as a qualifier
            namespaceName = _usingPluginName;
            if (!className.contains("::"))
                creation = createObject(namespaceName+"::"+className, objectName, parent);
            else {
                QString msg = "Qualify class name with plug-in name as in:\n" +
                        qualifiedClassNames(className).join("\n");
                ThrowException(msg);
            }
        }
    }

    Node *node = dynamic_cast<Node*>(creation);
    if (node)
        node->setClassName(namespaceName, className);

    return creation;
}

QStringList MegaFactory::qualifiedClassNames(QString className) {
    QStringList result;
    if (className.contains("::"))
        result << className;
    else {
        QList<FactoryPlugIn*> factories = me().productIndex.values(className);
        for (auto factory : factories)
            result << (factory->id() + "::" + className);
    }
    return result;
}

void MegaFactory::usingPlugin(QString pluginName) {
    _usingPluginName = pluginName;
}

const QList<FactoryPlugIn*>& MegaFactory::factories() {
    return me()._factories;
}

MegaFactory& MegaFactory::me() {
    if (!_me)
        _me = std::unique_ptr<MegaFactory>( new MegaFactory );
    return *_me;
}

 QStringList MegaFactory::find(QString className) {
     QStringList names;
     auto factories = me().productIndex.values(className);
     for (auto factory : factories)
         names << factory->id();
     return names;
 }

} //namespace

/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_MEGA_FACTORY_H
#define BASE_MEGA_FACTORY_H

#include <QDir>
#include <QMultiMap>
#include <QObject>
#include "box.h"
#include "exception.h"

namespace base {
	
class Box;
class FactoryPlugIn;

class MegaFactory : public QObject
{
public:
    static void loadPlugins();

    template <class T=Box>
    static T* create(QString className, QString objectName, Box *parent=nullptr);

    static const QList<base::FactoryPlugIn*> &factories();
    static QDir pluginsDir();
    static QStringList qualifiedClassNames(QString className);
    static void usingPlugin(QString pluginName);
    static QStringList find(QString className);
private:
    // methods
    MegaFactory();
    static std::unique_ptr<MegaFactory> _me;
    static MegaFactory &me();

    static Node* createObject(QString className, QString objectName, Box *parent=nullptr);
    // data
    typedef QMultiMap<QString, FactoryPlugIn*> ProductIndex;
    ProductIndex productIndex;
    QList<base::FactoryPlugIn*> _factories;
    static QString _usingPluginName;
};

template <class T>
T* MegaFactory::create(QString className, QString objectName, Box *parent)
{
    Node *object = MegaFactory::createObject(className, objectName, parent);
    QString msg = QString("MegaFactory cannot create object '%1' of class '%2'").arg(objectName).arg(className);
    if (!object)
        ThrowException(msg);
    T *specificObject = dynamic_cast<T*>(object);
    if (!object)
        ThrowException(msg);
    return specificObject;
}


}//namespace

#endif

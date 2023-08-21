/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HELP_CLASS_BASE_H
#define HELP_CLASS_BASE_H
#include <QString>
#include <base/port_type.h>

namespace base {
    class Box;
    class Port;
}

namespace command {

class HelpClassBase
{
public:
    HelpClassBase(base::Box *box);
    virtual ~HelpClassBase() {}
    void showClassInfo();
protected:
    #if QT_VERSION < 0x060000
        int       _colWidthName, _colWidthType, _colWidthValue, _colWidthUnit;
    #else
        qsizetype _colWidthName, _colWidthType, _colWidthValue, _colWidthUnit;
    #endif

    base::Box *_box;
    bool isConstant(const base::Port *port) const;
    bool hasIntervalAsUnit(const base::Port *port) const;
    QString pluginName() const;
    QString className() const;
private:
    // methods
    virtual QString header() const = 0;
    virtual QString inputPorts() const = 0;
    virtual QString outputPorts() const = 0;
    virtual QString additionalOutputPorts() const = 0;
    virtual QString footer() const = 0;
    virtual QString sideEffects() const = 0;
    virtual QStringList portLines(base::PortType type) const = 0;
    virtual void done(QString info) = 0;
    void setColWidths();
};

}

#endif

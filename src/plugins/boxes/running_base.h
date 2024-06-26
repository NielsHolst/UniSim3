/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RUNNING_BASE_H
#define RUNNING_BASE_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class RunningBase : public base::Box
{
public: 
    RunningBase(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double input, initial;
protected:
    // Output
    double value;
private:
    // Data
    QVector<double> buffer;
    int pos;
protected:
    // Methods
    virtual int bufferSize() const = 0;
};

} //namespace
#endif

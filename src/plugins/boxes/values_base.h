/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VALUES_BASE_H
#define VALUES_BASE_H
#include <base/box.h>

namespace boxes {

class ValuesBase : public base::Box
{
public:
    ValuesBase(QString name, Box *parent);
    void amend() final;
    void initialize() final;
    virtual void setValues() = 0;
protected:
    // Inputs
    int counter;
    // Outputs
    QVector<double> values;
    int numValues;
    double value;
    // Methods
    void setCounterByIteration();
    void setCounterByStep();
    void resetByIteration();
    void resetByStep();
    void updateByStep();
private:
    virtual void setCounter() = 0;
    virtual void setIterations() {}
    virtual void setSteps() {}
    // Data
    int _offset;
};

}

#endif

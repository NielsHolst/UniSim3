/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RANDOM_BASE_H
#define RANDOM_BASE_H
#include <base/box.h>

namespace base {
    class RandomOrder;
}

namespace boxes {

class RandomiserBase;

class RandomBase : public base::Box
{
public: 
    RandomBase(QString name, Box *parent);
    virtual ~RandomBase() {}
    void amend() final;
    void initialize() final;
    void reset() final;
    void update() final;
    RandomiserBase* randomiser();
    void setDrawAtReset(bool value) { drawAtReset = value; }
    virtual void drawValue() = 0;
    virtual void setValue(double newValue) = 0;
    virtual double getValue() const = 0;
protected:
    // Inputs
    double P;
    bool
        drawAtInitialize,
        drawAtReset,
        drawAtUpdate;
    // Data
    base::RandomOrder *_number;
    bool _valueSet;
private:
    RandomiserBase *_randomiser;
    // Methods
    virtual void checkInputs(QString text) = 0;
    void setNumStrata();
};

} //namespace
#endif

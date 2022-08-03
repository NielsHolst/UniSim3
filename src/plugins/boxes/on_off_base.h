/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ON_OFF_BASE_H
#define ON_OFF_BASE_H
#include <base/box.h>

namespace boxes {

template <class X, class Limits=X> class OnOffBase : public base::Box
{
public:
    OnOffBase(QString name, Box *parent) : Box(name, parent) {
        help("swithes on inside [xOn,xOff] interval");
        Input(x).help("x-value at which to calculate value");
        Input(xOn).help("Lower threshold");
        Input(xOff).help("Upper threshold");
        Input(valueOn).equals(1).help("Value inside [xOn,xOff] interval");
        Input(valueOff).equals(0).help("Value outside [xOn,xOff] interval");;
        Input(isSticky).equals(false).help("Should remain on when switched on?");
        Output(value).help("Current on or off value");
        Output(isOn).help("Is switch on?");
    }
    void reset() {
        setSwitch();
        setValue();
    }
    void update() {
        if (!(isOn && isSticky)) {
            setSwitch();
            setValue();
        }
    }
protected:
    // Input
    X x;
    Limits xOn, xOff;
    double valueOn, valueOff;
    bool isSticky;
    // Output
    double value;
    bool isOn;
    // Methods
private:
    virtual void setSwitch() = 0;
    virtual void setValue() = 0;
};

}

#endif

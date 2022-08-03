/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_CALC_PARASITE_HOST_H
#define FOOD_WEB_CALC_PARASITE_HOST_H
#include "food_web_calc_base.h"

namespace boxes {

class FoodWebCalcParasiteHost : public FoodWebCalcBase
{
public: 
    FoodWebCalcParasiteHost(QString name, Box *parent);
    void amend();
    void reset();
protected:
    // Outputs (indirect)
    base::Matrix2D<double> _Snet;
    QVector<double> _SnetTotal, _sdRatioNet;
    base::Matrix2D<double> _avgnumAttacks, _numAttacked;
    QVector<double> _numAttackedTotal;
    base::Matrix2D<double> _netNumAttacked;
    QVector<double> _netNumAttackedTotal;
    base::Matrix2D<double> _propAttacked;
    QVector<double> _propAttackedTotal;
    base::Matrix2D<double> _netPropAttacked;
    QVector<double> _netPropAttackedTotal;
    // Methods
    virtual void computeSearchRates();
    virtual void computeLosses();
    virtual QString attackerClass() const {return "Parasite";}
    virtual QString resourceClass() const {return "Host";}
};

} //namespace
#endif

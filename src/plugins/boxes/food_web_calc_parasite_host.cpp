/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/proportions.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "food_web_calc_parasite_host.h"

using namespace base;
using namespace TestNum;

namespace boxes {

PUBLISH(FoodWebCalcParasiteHost)

FoodWebCalcParasiteHost::FoodWebCalcParasiteHost(QString name, Box *parent)
    : FoodWebCalcBase(name, parent) {
    help("computes food web acquisition among Parasite/Host objects");
}

void FoodWebCalcParasiteHost::amend() {
    FoodWebCalcBase::amend();

    // Allocate output buffers
    _Snet.resize(_nResources, _nAttackers);
    _SnetTotal.resize(_nAttackers);
    _sdRatioNet.resize(_nAttackers);
    _avgnumAttacks.resize(_nResources, _nAttackers);
    _numAttacked.resize(_nResources, _nAttackers);
    _numAttackedTotal.resize(_nResources);
    _netNumAttacked.resize(_nResources, _nAttackers);
    _netNumAttackedTotal.resize(_nResources);
    _propAttacked.resize(_nResources, _nAttackers);
    _propAttackedTotal.resize(_nResources);
    _netPropAttacked.resize(_nResources, _nAttackers);
    _netPropAttackedTotal.resize(_nResources);
    // Create parasite-host output ports
    for (int i=0; i<_nResources; ++i) {
        for (int j=0; j<_nAttackers; ++j) {
            QString suffix = "_" + _attackerNames.at(j) + "_" + _resourceNames.at(i);
            NamedOutput("net_supply" + suffix, _Snet(i,j));
            NamedOutput("avg_num_attacks" + suffix, _avgnumAttacks(i,j));
            NamedOutput("num_attacked" + suffix, _numAttacked(i,j));
            NamedOutput("net_num_attacked" + suffix, _netNumAttacked(i,j));
            NamedOutput("prop_attacked" + suffix, _propAttacked(i,j));
            NamedOutput("net_prop_attacked" + suffix, _netPropAttacked(i,j));
        }
    }
    // Create parasite output ports
    for (int j=0; j<_nAttackers; ++j) {
        QString suffix = "_" + _attackerNames.at(j);
        NamedOutput("net_supply" + suffix, _SnetTotal[j]);
        NamedOutput("net_sd_ratio" + suffix, _sdRatioNet[j]);
    }
    // Create host output ports
    for (int i=0; i<_nResources; ++i) {
        QString suffix = "_" + _resourceNames.at(i);
        NamedOutput("num_attacked" + suffix, _numAttackedTotal[i]);
        NamedOutput("net_num_attacked" + suffix, _netNumAttackedTotal[i]);
        NamedOutput("prop_attacked" + suffix, _propAttackedTotal[i]);
        NamedOutput("net_prop_attacked" + suffix, _netPropAttackedTotal[i]);
    }
}

void FoodWebCalcParasiteHost::reset() {
    FoodWebCalcBase::reset();
    _Snet.fill(0.);
    _SnetTotal.fill(0.);
    _sdRatioNet.fill(0.);
    _avgnumAttacks.fill(0.);
    _numAttacked.fill(0.);
    _numAttackedTotal.fill(0.);
    _netNumAttacked.fill(0.);
    _netNumAttackedTotal.fill(0.);
    _propAttacked.fill(0.);
    _propAttackedTotal.fill(0.);
    _netPropAttacked.fill(0.);
    _netPropAttackedTotal.fill(0.);
}

void FoodWebCalcParasiteHost::computeSearchRates() {
    // For every resource i, compute the search rates s_ij for all it attackers j by by eq. 8.
    for (int i=0; i<_nResources; ++i) {
        // Compute total search rate: si_
        double sum_aY(0);
        for (int j=0; j<_nAttackers; ++j)
            sum_aY += a(i,j)*Y(j);

        double si_ = 1. - exp(-sum_aY*timeStep);
        // Split search rate among predators: _s(i,j)
        for (int j=0; j<_nAttackers; ++j) {
            _s(i,j) = (sum_aY > 0.) ? a(i,j)*Y(j)/sum_aY*si_ : 0.;
        }
    }
}
void FoodWebCalcParasiteHost::computeLosses() {
    // Parasite-host outputs
    for (int i=0; i<_nResources; ++i) {
        Proportions prop(this);
        for (int j=0; j<_nAttackers; ++j) {
            double avgAttacks = (X(i) > 0.) ? S(i,j)/X(i) : 0.,
                   propAttacked = 1. - exp(avgAttacks);
            _avgnumAttacks(i,j) = avgAttacks;
            _propAttacked(i,j) = propAttacked;
            prop.add(propAttacked);
        }
        QVector<double> netPropAttacked = prop.reconciled();
        for (int j=0; j<_nAttackers; ++j) {
            _netPropAttacked(i,j) = netPropAttacked.at(j);
            _netNumAttacked(i,j)  = netPropAttacked.at(j)*X(i);
            _Snet(i,j) = netPropAttacked.at(j)*g(i,j)*X(i);
        }
    }
    // Parasite outputs
    for (int j=0; j<_nAttackers; ++j) {
        double SnetTotal = 0.;
        for (int i=0; i<_nResources; ++i) {
            SnetTotal += _Snet(i,j);
        }
        _SnetTotal[j] = SnetTotal;
        _sdRatioNet[j] = (D(j)>0.) ? SnetTotal/D(j) : 0.;
    }
    // Parasite outputs
    for (int i=0; i<_nResources; ++i) {
        double numAttackedTotal = 0.,
               netNumAttackedTotal = 0.;
        for (int j=0; j<_nAttackers; ++j) {
            numAttackedTotal += _numAttacked(i,j);
            netNumAttackedTotal += _netNumAttacked(i,j);
        }
        _numAttackedTotal[i] = numAttackedTotal;
        _netNumAttackedTotal[i] = netNumAttackedTotal;
        _propAttackedTotal[i] = (X(i)>0.) ? numAttackedTotal/X(i) : 0.;
    }


}

} //namespace


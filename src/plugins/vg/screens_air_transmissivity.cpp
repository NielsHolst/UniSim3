/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <math.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "screens_air_transmissivity.h"

#include <base/dialog.h>

using namespace base;
using TestNum::eq;

namespace vg {

PUBLISH(ScreensAirTransmissivity)

ScreensAirTransmissivity::ScreensAirTransmissivity(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes total air transmissivity through screens");
    Input(screenAirExponent).imports("shelter[screenAirExponent]");
    Input(states).imports("../*[state]").unit("[0;1]");
    Input(transmissivities).imports("../*[transmissivityAir]").unit("[0;1]");

    Output(value).help("total air transmissivity").unit("[0;1]");
}

void ScreensAirTransmissivity::reset() {
    _n = states.size();
    int n = transmissivities.size();
    if (_n != n)
        ThrowException("Vector must have equal size").value(_n).value(n).context(this);
    update();
}

/* The solution below led to too high RH due to inefficient ventilation even with a screen crack
 *
 * R code from screen_energy_combined.R
    # tr: transmissions [0;1]
    # st: states [0;1]  (1=fully drawn)
    combine = function(tr, st) {
      n = length(st)
      if (n==1) {
        overlap =  0
      } else if (sum(st)<1) {
        overlap = 0
      } else {
        overlap = (sum(st)-1)/(n-1)
      }
      transmission = 1 - (1-tr)*st
      multiplicative = prod(transmission)
      unscreened = max(1 - sum(st),0)
      additive = unscreened + sum((1-unscreened)*transmission*st/sum(st))

      data.frame(
        Overlap = overlap,
        Comb = overlap*(multiplicative - additive) + additive  #combined transmission
      )
    }

void ScreensAirTransmissivity::update() {
    double sum_st = sum(states);
    if (eqZero(sum_st)) {
        value = 1.;
    }
    else {
        double overlap = (_n == 1 || sum_st < 1.) ? 0. : (sum_st-1)/(_n-1);
        QVector<double> diff, prod, transmission;
        difference(diff, 1., transmissivities);
        product(prod, diff, states);
        difference(transmission, 1., prod);
        double multiplicative = product(transmission),
               unscreened = std::max(1-sum_st, 0.);
        product(prod, transmission, states);
        double additive = unscreened + sum(prod)*(1-unscreened)/sum_st;
        value = overlap*(multiplicative - additive) + additive;
    }
}
*/

void ScreensAirTransmissivity::update() {
    value = 1.;
    for (int i=0; i<_n; ++i) {
        const double &state(states.at(i)),
                     &trans(transmissivities.at(i));
        value *= trans + (1.-trans)*pow((1.-state), 1./screenAirExponent);
    }
}



} //namespace


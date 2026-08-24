/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PHYS_MATH_H
#define PHYS_MATH_H

#include <limits>
#include <math.h>
#include <stdlib.h>
#include <boost/math/constants/constants.hpp>
#include <QPair>

#include "base_dllexport.h"

/*! \file phys_math.h
// General physical functions and constants
 */

namespace phys_math {

constexpr double
    PI = boost::math::constants::pi<double>(), //!< Mathematical PI
    T0 = 273.15,                //!< Absolute zero offset (oC)
    g = 9.81,                   //!< Gravity (m/s2)
    Sigma = 5.6704e-8,          //!< Stefan-Boltzmann constant (W/m2/K4)
    Mwater = 18.016e-3,         //!< Molar mass of water (kg/mol)
    MCo2 = 44.01e-3,            //!< Molar mass of CO2 (kg/mol or mg/umol)
    Mwa = 0.622,                //!< Molar mass of water relative to molar mass of air (-)
    R = 8.314,                  //!< Gas constant (J/mol/K)
    P0 = 101325,                //!< Standard atmospheric pressure = 1 atm (Pa)
    CpAir = 1020,               //!< Specific heat capacity of air (J/kg/K)
    CpWater = 4184,             //!< Specific heat capacity of water (J/kg/K)
    RhoAir = 1.19,              //!< Density of air at P0 (kg/m3)
    CpAirVol = CpAir*RhoAir,    //!< Volume-specific heat capacity of air (J/m3/K)
    CpWaterVol = CpWater*1000,  //!< Volume-specific heat capacity of water (J/m3/K)
    RhoCo2 = 1.98,              //!< Density of CO2 at T0 (kg/m3)
    LHe = 2454e3,               //!< Latent heat of vaporisation of water (J/kg) at 20 oC
    Psychr = CpAir*P0/LHe/Mwa;  //!< Psychrometric constant (Pa/K)

const double xGauss3[3] = {0.1127, 0.5, 0.8873},
             wGauss3[3] = {0.2778, 0.4444, 0.2778};

// Amount of water vapour
UNI_API double ahFromSh(double sh);
UNI_API double ahFromVp(double temperature, double vp);
UNI_API double shFromAh(double ah);
UNI_API double shFromVp(double temperature, double vp);
UNI_API double vpFromAh(double temperature, double ah);
UNI_API double vpFromSh(double temperature, double sh);

// Saturated amount of water vapour
UNI_API double sah(double temperature);
UNI_API double ssh(double temperature);
UNI_API double svp(double temperature);
UNI_API double svpSlope(double temperature);

// Relative measures of water vapour
UNI_API double rhFromAh(double temperature, double ah);
UNI_API double rhFromSh(double temperature, double sh);
UNI_API double rhFromVp(double temperature, double vp);

UNI_API double ahFromRh(double temperature, double rh);
UNI_API double shFromRh(double temperature, double rh);
UNI_API double vpFromRh(double temperature, double rh);

UNI_API double adxFromRh(double temperature, double rh);
UNI_API double sdxFromRh(double temperature, double rh);
UNI_API double vpdFromRh(double temperature, double rh);
UNI_API double rhFromAdx(double temperature, double adx);
UNI_API double rhFromSdx(double temperature, double sdx);
UNI_API double rhFromVpd(double temperature, double vpd);

// CO2
UNI_API double ppmFromAbsCo2(double temperature, double abs, double P = P0);
UNI_API double absFromPpmCo2(double temperature, double ppm, double P = P0);

// Other climatic parameters
UNI_API double Tdew(double temperature, double rh);
UNI_API double Twet(double temperature, double rh);
UNI_API double rhFromTwet(double temperature, double Twet);
UNI_API double virtualTemperatureFromAh(double temperature, double ah);
UNI_API double rhoAir(double temperature, double P = P0);
UNI_API double enthalpy(double temperature, double sh);
UNI_API double evaporationHeat(double temperature);

// Radiation
UNI_API double jointEmissivity(double em1, double em2);

// Mathematics
UNI_API inline double infinity() { return std::numeric_limits<double>::infinity(); }
UNI_API inline double safeDiv(double a, double b) {return (a==0. && b==0.) ? 0. : a/b;}
UNI_API inline bool isNan(double x) { return std::fpclassify(x) == FP_NAN; }
UNI_API inline int sqr(int x) {return x*x;}
UNI_API inline double sqr(double x) {return x*x;}
UNI_API inline double p2(double x) { return sqr(x); }
UNI_API inline double p3(double x) { return pow(x, 3.); }
UNI_API inline double p4(double x) { return pow(x, 4.); }
UNI_API inline double p4K(double T) { return pow(T+T0, 4.); }

UNI_API double cumNormal(double x, double m=0, double sd=1);
UNI_API double invCumNormal(double p, double m=0, double sd=1);
UNI_API QPair<double, double> inferNormal(double xmin, double xmax, double p=0.99);
UNI_API double invNormalRange(double u, double xmin, double xmax, double p=0.99);

UNI_API double logistic(double current, double target, double rate, double dt);
UNI_API double propIntegral(double current, double target, double rate, double dt);
UNI_API double propExpIntegral(double current, double target, double rate, double dt, double exponent);
UNI_API double invPropExpIntegral(double integral, double target, double rate, double dt, double exponent);

// Pointer math
UNI_API size_t hashPointers(const void *p, const void *q);

template <class T> T UNI_API minmax(T min, T value, T max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

struct UNI_API Parabola {
    double a,b,c;
};
UNI_API Parabola fitParabola(double y0, double y1, double y2);
UNI_API Parabola fitParabola(const double *x, const double *y);

UNI_API double fitPointPPS(double y0, double y1, double y2_slope);

UNI_API double fitSlopePPP(double y0, double y1, double y2);

UNI_API double sineWaveUp(double proportion, double y0, double y1);

UNI_API double sineWaveDown(double proportion, double y0, double y1);

} //namespace


#endif

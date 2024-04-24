/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <bitset>
#include <math.h>
#include <boost/math/distributions/normal.hpp>
#include <QtGlobal>
#include "exception.h"
#include "phys_math.h"

using namespace std;
using boost::math::normal;

/*! \file phys_math.cpp
 * General physical-mathematical functions and constants
 */

namespace phys_math {

// Amount of water vapour

//! Compute absolute humidity from temperature and specific humidity
/*!	\param temperature (oC)
    \param specific humidity (kg/kg)
    \return absolute humidity (kg/m3)
 */
double ahFromSh(double sh) {
    return sh*RhoAir;
}

//! Compute absolute humidity from temperature and vapour pressure
/*!	\param temperature (oC)
    \param vapour pressure (Pa)
    \return absolute humidity (kg/m3)
 */
double ahFromVp(double temperature, double vp) {
    return vp*Mwater/(R*(temperature + T0));
}

//! Compute specific humidity from temperature and absolute humidity
/*!	\param temperature (oC)
    \param absolute humidity (kg/m3)
    \return specific humidity (kg/kg)
 */
double shFromAh(double ah) {
    return ah/RhoAir;
}

//! Compute specific humidity from temperature and vapour pressure
/*!	\param temperature (oC)
    \param vapour pressure (Pa)
    \return specific humidity (kg/kg)
 */
double shFromVp(double temperature, double vp) {
    double ah = ahFromVp(temperature, vp);
    return shFromAh(ah);
}

//! Compute vapour pressure from temperature and absolute humidity
/*!	\param temperature (oC)
    \param absolute humidity (kg/m3)
    \return vapour pressure (Pa)
 */
double vpFromAh(double temperature, double ah) {
    return ah/Mwater*(R*(temperature + T0));
}

//! Compute vapour pressure from temperature and specific humidity
/*!	\param temperature (oC)
    \param specific humidity (kg/kg)
    \return vapour pressure (Pa)
 */
double vpFromSh(double temperature, double sh) {
    double ah = ahFromSh(sh);
    return vpFromAh(temperature,ah);
}

// Saturated amount of water vapour

//! Compute saturated absolute humidity from temperature
/*!	\param temperature (oC)
    \return absolute humidity (kg/m3)
 */
double sah(double temperature) {
    return ahFromVp(temperature, svp(temperature));
}

//! Compute saturated specific humidity from temperature
/*!	\param temperature (oC)
    \return specific humidity (kg/kg)
 */
double ssh(double temperature) {
    return shFromVp(temperature, svp(temperature));
}

//! Compute saturated water vapour pressure from temperature
/*!	\param temperature (oC)
    \return saturated water vapour pressure (Pa)
 */
double svp(double temperature) {
    return 610.8*exp(17.27*temperature/(temperature+237.3));
}

//! Compute slope of saturated water vapour pressure curve on temperature
/*!	\param temperature (oC)
    \return saturated water vapour pressure slope, dsvp/dT (Pa/K)
 */
double svpSlope(double temperature) {
    return 4098*svp(temperature)/p2(temperature+237.3);
}

// Relative measures of water vapour

//! Compute relative humidity from temperature and absolute humidity
/*!	\param temperature (oC)
    \param absolute humidity (kg/m3)
    \return relative humidity [0;100]
 */
double rhFromAh(double temperature, double ah) {
    return min(100*ah/sah(temperature), 100.);
}

//! Compute relative humidity from temperature and specific humidity
/*!	\param temperature (oC)
    \param specific humidity (kg/kg)
    \return relative humidity [0;100]
 */
double rhFromSh(double temperature, double sh) {
    return min(100*sh/ssh(temperature), 100.);
}

//! Compute relative humidity from temperature and vapour pressure
/*!	\param temperature (oC)
    \param vapour pressure (Pa)
    \return relative humidity [0;100]
 */
double rhFromVp(double temperature, double vp) {
    return min(100*vp/svp(temperature), 100.);
}

//! Compute absolute humidity from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return absolute humidity (kg/m3)
 */
double ahFromRh(double temperature, double rh) {
    return rh/100*sah(temperature);
}

//! Compute specific humidity from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return specific humidity (kg/kg)
 */
double shFromRh(double temperature, double rh) {
    return rh/100*ssh(temperature);
}

//! Compute vapour pressure from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return vapour pressure (Pa)
 */
double vpFromRh(double temperature, double rh) {
    return rh/100*svp(temperature);
}

//! Compute absolute humidity Delta X from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return absolute humidity Delta X (kg/m3)
 */
double adxFromRh(double temperature, double rh) {
    double ah = ahFromRh(temperature, rh);
    return sah(temperature) - ah;
}

//! Compute specific humidity Delta X from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return specific humidity Delta X (kg/kg)
 */
double sdxFromRh(double temperature, double rh) {
    double sh = shFromRh(temperature, rh);
    return ssh(temperature) - sh;
}

//! Compute vapour pressure deficit from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return vapour pressure deficit (Pa)
 */
double vpdFromRh(double temperature, double rh) {
    double vp = vpFromRh(temperature, rh);
    return svp(temperature) - vp;
}

double rhFromAdx(double temperature, double adx) {
    double ah = sah(temperature) - adx;
    return rhFromAh(temperature, ah);
}

double rhFromSdx(double temperature, double sdx) {
    double sh = ssh(temperature) - sdx;
    return rhFromSh(temperature, sh);
}

double rhFromVpd(double temperature, double vpd) {
    double vp = svp(temperature) - vpd;
    return rhFromVp(temperature, vp);
}

// CO2

//! Compute ppm CO2 from absolute CO2
/*!	\param temperature (oC)
    \param absolute CO2 (kg/m3)
    \param air pressure (Pa)
    \return ppm CO2 (micromole/mole)
 */
double ppmFromAbsCo2(double temperature, double abs, double P) {
    return abs*R*(temperature+T0)*1e6/P/MCo2;
}

//! Compute absolute CO2 from ppm CO2
/*!	\param temperature (oC)
    \param ppm CO2 (micromole/mole)
    \param air pressure (Pa)
    \return absolute CO2 (kg/m3)
 */
double absFromPpmCo2(double temperature, double ppm, double P) {
    return P*ppm*1e-6/R/(temperature+T0)*MCo2;
}

// Other

//! Compute dew point temperature from temperature and r.h.
/*! Calculated as an inverse function of p_satu.
    \param temperature (oC)
    \param rh relative humidity (%)
    \return dew point temperature (oC)
 */
double Tdew(double temperature, double rh) {
    double vp = vpFromRh(temperature, rh);
    const double k1 = 133.32,
        k2 = 1.0887,
        k3 = 276.4,
        k4 = 0.0583,
        k5 = 2.1938;
    return (k5*log(vp/k1) - k3) / (log(vp/k1)*k4 - k2) - T0;
}

//! Compute wet bulb temperature from temperature and r.h.
/*! See DOI: 10.1175/JAMC-D-11-0143.1
    \param temperature (oC)
    \param rh relative humidity (%)
    \return wet bulb temperature (oC)
 */
double Twet(double temperature, double rh) {
    return std::min(
                temperature*atan(0.151977*sqrt(rh + 8.313659)) +
                atan(temperature+rh) -
                atan(rh-1.676331) +
                0.00391838*pow(rh,1.5)*atan(0.023101*rh) -
                4.686035,
                temperature);
}

//! Compute r.h. from temperatur and wet bulb temperature
/*! See weather.gov/media/epz/wxcalc/rhTdFromWetBulb.pdf
    \param temperature (oC)
    \param wet bulb temperature (oC)
    \return rh relative humidity (%)
 */
double rhFromTwet(double temperature, double Twet) {
    double
        &T(temperature),
        es = 6.112*exp(17.67*T   /(T   +243.5)),
        ew = 6.112*exp(17.67*Twet/(Twet+243.5)),
        ea = ew - 1013.25*(T - Twet)*0.00066*(1 + 0.00115*Twet);
    return ea/es*100.;
}

double virtualTemperatureFromAh(double temperature, double ah) {
    return (temperature+T0)*(1+0.608*shFromAh(ah));
}

//! Compute air density from temperature and pressure
/*! See chemeurope.com/en/encyclopedia/Density_of_air.html
    \param temperature (oC)
    \param air pressure (Pa)
    \return air density (kg/m3)
 */
double rhoAir(double T, double P) {
    return P/287.058/(T+T0);
}

//! Compute air enthalpy from temperature and specific humidity
/*! See sciencing.com/calculate-enthalpy-air-6118694.html
    \param temperature (oC)
    \param specific humidity (kg/kg)
    \return air enthalphy (J/kg)
 */
double enthalpy(double temperature, double sh) {
  return 1.007*temperature - 0.026 + sh*(2501. + 1.84*temperature);
}

//! Compute heat of evaporation from temperature
/*! See mychemengmusings.wordpress.com/2019/01/08/handy-equations-to-calculate-heat-of-evaporation-and-condensation-of-water-steam/
    \param temperature (oC)
    \return heat of evaporation (J/kg)
 */
double evaporationHeat(double temperature) {
    double y = 193.1 - 10950.*log((374.-temperature)/647.) * pow(374.-temperature,0.785) /(273.+temperature);
    return 1000.*y;
}

//! Joint of two emissivities
/*! * \brief Joint emmisivity of two opposing surfaces
 * \param em1 is the emmisivity of the one surface
 * \param em2 is the emmisivity of the other surface
 * \return joint emmisivity of the two
 */

double jointEmissivity(double em1, double em2) {
    return (em1==0. || em2==0.) ? 0. :
           1/(1/em1 + 1/em2 - 1);

}

//! Cumulative normal distribution
/*!
 * \param x a value of the normally distributed variate
 * \param m the mean
 * \param s the standard deviation
 * \return the probability of a value <= x
 */
double cumNormal(double x, double m, double sd) {
    normal s(m, sd);
    return cdf(s, x);
//    return 0.5*(1 + erf((x-m)/sqrt(2)/sd));
}

//! The inverse of the cumulative normal distribution
/*!
 * \param p the probability
 * \param m the mean
 * \param s the standard deviation
 * \return the maximum value attained at probability p
 */
double invCumNormal(double p, double m, double sd) {
    normal s(m, sd);
    return quantile(s, p);
    /* From Shore, H (1982). Simple Approximations for the Inverse Cumulative Function,
     * the Density Function and the Loss Integral of the Normal Distribution.
     * Journal of the Royal Statistical Society. Series C (Applied Statistics). 31: 108Ã¢â‚¬â€œ114.
     * doi:10.2307/2347972
     */
//    bool flip = p<0.5;
//    if (flip) p = 1. - p;
//    double y = 5.5556*(1. - pow((1. - p)/p, 0.1186);
//    if (flip) y = -y;
//    return m + y*sd;
}

//! Infer the normal distribution from x-range and x-range probability
/*!
 * \param xmin the minimum allowed x value
 * \param xmax the maximum allowed x value
 * \param p the probability that x falls in the [xmin,xmax] range
 * \return mean and standard deviation of the corresponding normal distribution
 */
QPair<double, double> inferNormal(double xmin, double xmax, double p) {
    if (p<=0 || p>=1)
        ThrowException("P must be in the open-ended range (0;1)").value(p);
    double pmax = (1. + p)/2.,
           xsd = invCumNormal(pmax);
    return qMakePair(
      (xmax + xmin)/2.,
      (xmax - xmin)/2./xsd
    );
}

//! Produce a value from normal distribution re-scaled to x-range
/*!
 * \param u the probability of an outcome <= value
 * \param xmin the minimum allowed x value
 * \param xmax the maximum allowed x value
 * \param p the probability that x falls in the [xmin,xmax] range
 * \return value for which an outcome <= value has u probability
 */
double invNormalRange(double u, double xmin, double xmax, double p) {
    auto normal = inferNormal(xmin, xmax, p);
    double pmin = (1. - p)/2.,
           pscaled = pmin + u*p;
    return invCumNormal(pscaled, normal.first, normal.second);
}

//! Logistic growth integral
/*!
 * The integral of the logistic growth equation is used to calculate the approach of a variable
 * towards a target value through time
 * \param current is the value at the beginning of the time step
 * \param target is the value approached during the time step
 * \param rate is the rate of approach [inverse units of time step]
 * \param dt is the time step
 * \return the value predicted at the end of the time step
 */
double logistic(double current, double target, double rate, double dt) {
    double L = exp(rate*dt);
    return target*current*L/(target + current*(L-1));
}

double propIntegral(double current, double target, double rate, double dt) {
    double L = exp(rate*dt);
    return target - (target - current)/L;
}

double propExpIntegral(double current, double target, double rate, double dt, double exponent) {
    if (exponent == 1.)
        return(propIntegral(current, target, rate, dt));
    double z = (exponent-1)*rate*dt + pow(fabs(target-current), 1-exponent),
           distance = pow(z, 1/(1-exponent));
    return (current<target) ? target-distance : target+distance;
}

double invPropExpIntegral(double integral, double target, double rate, double dt, double exponent) {
    Q_ASSERT(exponent!=1.);
    double z =  pow(fabs(target-integral), 1-exponent) - (exponent-1)*rate*dt,
           distance = pow(z, 1/(1-exponent));
    return (integral<target) ? target-distance : target+distance;
}

//! Fit parabolic curve (3 points)
/*!
 * \brief A parabolic curve is fitted by difference interpolation for three points (0,y0), (1,y1), (2, y2)
  * \param y0 is the first y-value fitted to a parabolic curve
 * \param y1 is the second y-value fitted to a parabolic curve
 * \param y2 is the third y-value fitted to a parabolic curve
 * \return coefficients of the paracola
 */
Parabola fitParabola(double y0, double y1, double y2) {
    double a = (y2 - 2*y1 + y0)/2;
    return
        Parabola {
            a,
            y1 - y0 - a,
            y0
        };
}

Parabola fitParabola(const double *x, const double *y) {
    const double
        &x1(x[0]),
        &x2(x[1]),
        &x3(x[2]),
        &y1(y[0]),
        &y2(y[1]),
        &y3(y[2]),
        detA = p2(x1)*(x2 - x3) - p2(x2)*(x1 - x3) + p2(x3)*(x1 - x2);
    return
        Parabola {
            (y1*(x2 - x3) - y2*(x1 - x3) + y3*(x1 - x2)) / detA,
            (p2(x1)*(y2 - y3) - p2(x2)*(y1 - y3) + p2(x3)*(y1 - y2)) / detA,
            (p2(x1)*(x2*y3 - x3*y2) - p2(x2)*(x1*y3 - x3*y1) + p2(x3)*(x1*y2 - x2*y1)) / detA
    };
}

//! Extrapolate parabolic curve (2 points and 1 slope)
/*!
 * \brief A parabolic curve is fitted by two points (y0,y1) and the slope in the third point (y2_slope)
 * located equidistant on the x-axis. The y-value of the third point is returned
 * \param y0 is the first y-value fitted to a parabolic curve
 * \param y1 is the second y-value fitted to a parabolic curve
 * \param y2_slope is the slope at the third y-value fitted to a parabolic curve
 * \return predicted y at third x-point
 */
double fitPointPPS(double y0, double y1, double y2_slope) {
    // x-values at 0,1,2
    double
        a = (y2_slope - y1 + y0)/3,
        b = y2_slope - 4./3.*(y2_slope - y1 + y0),
        c = y0;
    return a*4 + b*2 + c; // ax2 + bx + c for x = 2
}

double fitSlopePPP(double y0, double y1, double y2) {
    // x-values at 0,1,2
    // y = ax2 + bx + c
    double
        a = (y2 - 2*y1 + y0)/2,
        b = y1 - y0 - a;
    // 2ax + b for x = 2
    return a*4 + b;
}

double sineWaveDown(double proportion, double y0, double y1) {
    double xr = (proportion - 0.5)*PI;
    return y0 + (y1-y0)*(sin(xr)+1.)/2.;
}

double sineWaveUp(double proportion, double y0, double y1) {
    return y0 + y1 - sineWaveDown(proportion, y1, y0);
}

//! Make hash value from two pointers
/*!
 * \brief Combine the most significant half of the bits in the two addresses.
 * \param p is a pointer
 * \param q is a pointer
 * \return hashed value
 */
size_t hashPointers(const void *p, const void *q) {
    constexpr int n = 8*sizeof(size_t), h=n/2;
    auto pi = (size_t) reinterpret_cast<uintptr_t>(p),
         qi = (size_t) reinterpret_cast<uintptr_t>(q);
    std::bitset<n>
        pb(pi),
        qb(qi),
        pqb = ((pb << h) >> h) | (qb << h);
    return (size_t) pqb.to_ullong();
}

} //namespace


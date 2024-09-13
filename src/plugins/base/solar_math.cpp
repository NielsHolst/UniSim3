/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDateTime>
#include "phys_math.h"
#include "solar_math.h"

using namespace std;

namespace phys_math {

qint64 excelDateValue(QDate date) {
    return QDate(1900,1,1).daysTo(date) + 2;
}

inline double IF (bool a, double b, double c) {
    return a ? b : c;
}

inline double MOD(double a, int b) {
    const double x = floor(a/b);
    return a - b*x;
}

inline double POWER(double a, double b) {
    return pow(a,b);
}

inline double RADIANS(double x) {
   return x*PI/180.;
}

inline double DEGREES(double x) {
    return x/PI*180.;
}

inline double SIN(double x) {
    return sin(x);
}

inline double COS(double x) {
    return cos(x);
}

inline double TAN(double x) {
    return tan(x);
}

inline double ASIN(double x) {
    return asin(x);
}

inline double ACOS(double x) {
    return acos(x);
}

inline double ATAN2(double a, double b) {
    return (a==0.) ? 0. : atan2(b, a); // swap arguments
}

Sun calcSun(double latitude, double longitude, QDateTime dateTime, double timeZone) {
  // Copied from Excel spreadsheet from https://gml.noaa.gov/grad/solcalc/calcdetails.html
  const double
  // Latitude (+ to N)
  B2 = latitude,
  //  Longitude (+ to E)
  B3 = longitude,
  // Time Zone (+ to E)
  B4 = timeZone,
  // Local Time (hrs)
  B5 = dateTime.time().msecsSinceStartOfDay()/1000./3600.,
  // Date
  D2 = excelDateValue(dateTime.date()),
  // Time (hrs past local midnight)
  E2 = B5/24.,
  // Julian Day
  F2 = D2 + 2415018.5 + E2 - B4/24.,
  // Julian Century
  G2 = (F2-2451545.)/36525.,
  // Geom Mean Long Sun (deg)
  I2 = MOD(280.46646+G2*(36000.76983 + G2*0.0003032), 360),
  // Geom Mean Anom Sun (deg)
  J2 = 357.52911+G2*(35999.05029 - 0.0001537*G2),
  // Eccent Earth Orbit
  K2 = 0.016708634-G2*(0.000042037+0.0000001267*G2),
  // Sun Eq of Ctr
  L2 = SIN(RADIANS(J2))*(1.914602-G2*(0.004817+0.000014*G2))+SIN(RADIANS(2*J2))*(0.019993-0.000101*G2)+SIN(RADIANS(3*J2))*0.000289,
  // Sun True Long (deg)
  M2 = I2+L2,
  // Sun True Anom (deg)
//  N2 = J2+L2,
  // Sun Rad Vector (AUs)
//  O2 = (1.000001018*(1-K2*K2))/(1+K2*COS(RADIANS(N2))),
  // Sun App Long (deg)
  P2 = M2-0.00569-0.00478*SIN(RADIANS(125.04-1934.136*G2)),
  // Mean Obliq Ecliptic (deg)
  Q2 = 23.+(26.+((21.448-G2*(46.815+G2*(0.00059-G2*0.001813))))/60.)/60.,
  // Obliq Corr (deg)
  R2 = Q2+0.00256*COS(RADIANS(125.04-1934.136*G2)),
  // Sun Rt Ascen (deg)
//  S2 = DEGREES(ATAN2(COS(RADIANS(P2)),COS(RADIANS(R2))*SIN(RADIANS(P2)))),
  // Sun Declin (deg)
  T2 = DEGREES(ASIN(SIN(RADIANS(R2))*SIN(RADIANS(P2)))),
  // var y
  U2 = TAN(RADIANS(R2/2))*TAN(RADIANS(R2/2)),
  // Eq of Time (minutes)
  V2 = 4*DEGREES(U2*SIN(2*RADIANS(I2))-2*K2*SIN(RADIANS(J2))+4*K2*U2*SIN(RADIANS(J2))*COS(2*RADIANS(I2))-0.5*U2*U2*SIN(4*RADIANS(I2))-1.25*K2*K2*SIN(2*RADIANS(J2))),
  // HA Sunrise (deg)
  W2 = DEGREES(ACOS(COS(RADIANS(90.833))/(COS(RADIANS(B2))*COS(RADIANS(T2)))-TAN(RADIANS(B2))*TAN(RADIANS(T2)))),
  // Solar Noon (LST)
  X2 = (720.-4.*B3-V2+B4*60.)/1440.,
  // Sunrise Time (LST)
  Y2 = (X2*1440.-W2*4.)/1440.,
  // Sunset Time (LST)
  Z2 = (X2*1440.+W2*4.)/1440.,
  // Sunlight Duration (minutes)
//  AA2 = 8*W2,
  // True Solar Time (min)
  AB2 = MOD(E2*1440+V2+4*B3-60*B4, 1440),
  // Hour Angle (deg)
  AC2 = IF(AB2/4<0, AB2/4+180, AB2/4-180),
  // Solar Zenith Angle (deg)
  AD2 = DEGREES(ACOS(SIN(RADIANS(B2))*SIN(RADIANS(T2))+COS(RADIANS(B2))*COS(RADIANS(T2))*COS(RADIANS(AC2)))),
  // Solar Elevation Angle (deg)
  AE2 = 90-AD2,
  // Approx Atmospheric Refraction (deg)
  AF2 = IF(AE2>85,0,IF(AE2>5,58.1/TAN(RADIANS(AE2))-0.07/POWER(TAN(RADIANS(AE2)),3)+0.000086/POWER(TAN(RADIANS(AE2)),5),IF(AE2>-0.575,1735+AE2*(-518.2+AE2*(103.4+AE2*(-12.79+AE2*0.711))),-20.772/TAN(RADIANS(AE2)))))/3600,
  // Solar Elevation corrected for atm refraction (deg)
  AG2 = AE2+AF2,
  // Solar Azimuth Angle (deg cw from N)
  AH2 = IF(AC2>0, MOD(DEGREES(ACOS(((SIN(RADIANS(B2))*COS(RADIANS(AD2)))-SIN(RADIANS(T2)))/(COS(RADIANS(B2))*SIN(RADIANS(AD2)))))+180,360), MOD(540-DEGREES(ACOS(((SIN(RADIANS(B2))*COS(RADIANS(AD2)))-SIN(RADIANS(T2)))/(COS(RADIANS(B2))*SIN(RADIANS(AD2))))),360));

  Sun sun;
  sun.sunrise   = QTime(0,0,0).addSecs(static_cast<int>(24*60*60*Y2));
  sun.noon      = QTime(0,0,0).addSecs(static_cast<int>(24*60*60*X2));
  sun.sunset    = QTime(0,0,0).addSecs(static_cast<int>(24*60*60*Z2));
  sun.solarTime = QTime(0,0,0).addSecs(static_cast<int>(60*AB2));
  sun.dayLength = 24.*(Z2 - Y2);
  sun.azimuth   = AH2;
  sun.elevation = AG2;
  return sun;
}

double extraterrestrialFlux(QDate date) {
    // From Solar Engineering of Thermal Processes, eq. 1.4.1a
    const int n = date.dayOfYear();
    return 1367.*(1. + 0.033*cos(2.*PI*n/365.));
}

inline double dsin(double x) {
    return sin(x*PI/180.);
}

inline double dcos(double x) {
    return cos(x*PI/180.);
}

inline double dacos(double x) {
    return acos(x)/PI*180.;
}

double clearSkyTransmittance(double sunElevation, double altitude) {
    // From Solar Engineering of Thermal Processes, eq. 2.8.1
    // Clear-sky horizontal beam radiation
    // sun elevation (0..90), altitude (m)
    if (sunElevation <= 0.)
        return 0.;
    const double
        A = altitude/1000.,
        a0 = 0.4237 - sqr(0.00821*(6.0 - A)),
        a1 = 0.5055 + sqr(0.00595*(6.5 - A)),
        k =  0.2711 + sqr(0.01858*(2.5 - A));
    return
        a0 + a1*exp(-k/dsin(sunElevation));
}

inline double saz(double naz) { // South-related azimuth from north-related azimuth
    return naz - 180.;
}

double angleOfIncidence(double sunElevation, double sunAzimuth, double surfaceSlope, double surfaceAzimuth) {
    // After Solar Engineering of Thermal Processes, eq. 1.6.3
    // sun_elevation: Above the horizon
    // sun_azimuth  : Solar azimuth angle (north = 0)
    // surf_slope   : Slope of the inclined surface = angle to the surface from horizontal (0..180)
    // surf_azimuth : Surface azimuth angle of horizontal side of surface
    // aoi < 0 means surface is backlit
    const double
        tz = 90. - sunElevation,
        g = saz(sunAzimuth),
        b = surfaceSlope;
    double
        gs = saz(surfaceAzimuth);
        gs = (gs < 0.) ? gs + 90. : gs - 90.;
    return
        90. - dacos( (dcos(tz)*dcos(b) + dsin(tz)*dsin(b)*dcos(gs-g)) );
}

} //namespace


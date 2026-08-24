/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/

#ifndef PSYCHOLIB_H
#define PSYCHOLIB_H

#include "base_dllexport.h"

/******************************************************************************************************
 * Helper functions
 *****************************************************************************************************/

enum UNI_API UnitSystem { UNDEFINED, IP, SI };

void UNI_API SetUnitSystem
  ( enum UnitSystem Units       // (i) System of units (IP or SI)
  );

enum UNI_API UnitSystem GetUnitSystem  // (o) System of units (SI or IP)
  (
  );


/******************************************************************************************************
 * Conversion between temperature units
 *****************************************************************************************************/

UNI_API double GetTRankineFromTFahrenheit(double T_F);

UNI_API double GetTFahrenheitFromTRankine(double T_R);

UNI_API double GetTKelvinFromTCelsius(double T_C);

UNI_API double GetTCelsiusFromTKelvin(double T_K);


/******************************************************************************************************
 * Conversions between dew point, wet bulb, and relative humidity
 *****************************************************************************************************/

UNI_API double GetTWetBulbFromTDewPoint // (o) Wet bulb temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TDewPoint            // (i) Dew point temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetTWetBulbFromRelHum    // (o) Wet bulb temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double RelHum               // (i) Relative humidity [0-1]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetRelHumFromTDewPoint   // (o) Relative humidity [0-1]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TDewPoint            // (i) Dew point temperature in °F [IP] or °C [SI]
  );

UNI_API double GetRelHumFromTWetBulb    // (o) Relative humidity [0-1]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TWetBulb             // (i) Wet bulb temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetTDewPointFromRelHum   // (o) Dew Point temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double RelHum               // (i) Relative humidity [0-1]
  );

UNI_API double GetTDewPointFromTWetBulb // (o) Dew Point temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TWetBulb             // (i) Wet bulb temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );


/******************************************************************************************************
 * Conversions between dew point, or relative humidity and vapor pressure
 *****************************************************************************************************/

UNI_API double GetVapPresFromRelHum     // (o) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double RelHum               // (i) Relative humidity [0-1]
  );

UNI_API double GetRelHumFromVapPres     // (o) Relative humidity [0-1]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double VapPres              // (i) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  );

UNI_API double GetTDewPointFromVapPres  // (o) Dew Point temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double VapPres              // (i) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  );

UNI_API double GetVapPresFromTDewPoint  // (o) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  ( double TDewPoint            // (i) Dew point temperature in °F [IP] or °C [SI]
  );


/******************************************************************************************************
 * Conversions from wet-bulb temperature, dew-point temperature, or relative humidity to humidity ratio
 *****************************************************************************************************/

UNI_API double GetTWetBulbFromHumRatio  // (o) Wet bulb temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetHumRatioFromTWetBulb  // (o) Humidity Ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TWetBulb             // (i) Wet bulb temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetHumRatioFromRelHum    // (o) Humidity Ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double RelHum               // (i) Relative humidity [0-1]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetRelHumFromHumRatio    // (o) Relative humidity [0-1]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetHumRatioFromTDewPoint // (o) Humidity Ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double TDewPoint            // (i) Dew point temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetTDewPointFromHumRatio // (o) Dew Point temperature in °F [IP] or °C [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );


/******************************************************************************************************
 * Conversions between humidity ratio and vapor pressure
 *****************************************************************************************************/

UNI_API double GetHumRatioFromVapPres   // (o) Humidity Ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double VapPres              // (i) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetVapPresFromHumRatio   // (o) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  ( double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );


/******************************************************************************************************
 * Conversions between humidity ratio and specific humidity
 *****************************************************************************************************/

UNI_API double GetSpecificHumFromHumRatio // (o) Specific humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double HumRatio               // (i) Humidity ratio in lb_H₂O lb_Dry_Air⁻¹ [IP] or kg_H₂O kg_Dry_Air⁻¹ [SI]
  );

UNI_API double GetHumRatioFromSpecificHum // (o) Humidity ratio in lb_H₂O lb_Dry_Air⁻¹ [IP] or kg_H₂O kg_Dry_Air⁻¹ [SI]
  ( double SpecificHum            // (i) Specific humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  );


/******************************************************************************************************
 * Dry Air Calculations
 *****************************************************************************************************/

UNI_API double GetDryAirEnthalpy                  // (o) Dry air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹ [SI]
  ( double TDryBulb                       // (i) Dry bulb temperature in °F [IP] or °C [SI]
  );

UNI_API double GetDryAirDensity                   // (o) Dry air density in lb ft⁻³ [IP] or kg m⁻³ [SI]
  ( double TDryBulb                       // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double Pressure                       // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetDryAirVolume                    // (o) Dry air volume ft³ lb⁻¹ [IP] or in m³ kg⁻¹ [SI]
  ( double TDryBulb                       // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double Pressure                       // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetTDryBulbFromEnthalpyAndHumRatio    // (o) Dry-bulb temperature in °F [IP] or °C [SI]
  ( double MoistAirEnthalpy                  // (i) Moist air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹
  , double HumRatio                          // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  );

UNI_API double GetHumRatioFromEnthalpyAndTDryBulb  // (o) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double MoistAirEnthalpy                // (i) Moist air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹
  , double TDryBulb                        // (i) Dry-bulb temperature in °F [IP] or °C [SI]
  );


/******************************************************************************************************
 * Saturated Air Calculations
 *****************************************************************************************************/

UNI_API double GetSatVapPres            // (o) Vapor Pressure of saturated air in Psi [IP] or Pa [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  );

UNI_API double GetSatHumRatio           // (o) Humidity ratio of saturated air in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetSatAirEnthalpy        // (o) Saturated air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );


/******************************************************************************************************
 * Moist Air Calculations
 *****************************************************************************************************/
UNI_API double GetVaporPressureDeficit  // (o) Vapor pressure deficit in Psi [IP] or Pa [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetDegreeOfSaturation    // (o) Degree of saturation []
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetMoistAirEnthalpy      // (o) Moist Air Enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  );

UNI_API double GetMoistAirVolume        // (o) Specific Volume ft³ lb⁻¹ [IP] or in m³ kg⁻¹ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetTDryBulbFromMoistAirVolumeAndHumRatio   // (o) Dry-bulb temperature in °F [IP] or °C [SI]
  ( double MoistAirVolume                         // (i) Specific volume of moist air in ft³ lb⁻¹ of dry air [IP] or in m³ kg⁻¹ of dry air [SI]
  , double HumRatio                               // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure                               // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );

UNI_API double GetMoistAirDensity       // (o) Moist air density in lb ft⁻³ [IP] or kg m⁻³ [SI]
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double HumRatio             // (i) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  );


/******************************************************************************************************
 * Standard atmosphere
 *****************************************************************************************************/

UNI_API double GetStandardAtmPressure   // (o) Standard atmosphere barometric pressure in Psi [IP] or Pa [SI]
  ( double Altitude             // (i) Altitude in ft [IP] or m [SI]
  );

UNI_API double GetStandardAtmTemperature // (o) Standard atmosphere dry bulb temperature in °F [IP] or °C [SI]
  ( double Altitude              // (i) Altitude in ft [IP] or m [SI]
  );

UNI_API double GetSeaLevelPressure   // (o) Sea level barometric pressure in Psi [IP] or Pa [SI]
  ( double StnPressure       // (i) Observed station pressure in Psi [IP] or Pa [SI]
  , double Altitude          // (i) Altitude above sea level in ft [IP] or m [SI]
  , double TDryBulb          // (i) Dry bulb temperature ft³ lb⁻¹ [IP] or in m³ kg⁻¹ [SI]
  );

UNI_API double GetStationPressure    // (o) Station pressure in Psi [IP] or Pa [SI]
  ( double SeaLevelPressure  // (i) Sea level barometric pressure in Psi [IP] or Pa [SI]
  , double Altitude          // (i) Altitude above sea level in ft [IP] or m [SI]
  , double TDryBulb          // (i) Dry bulb temperature in °F [IP] or °C [SI]
  );


/******************************************************************************************************
 * Functions to set all psychrometric values
 *****************************************************************************************************/

UNI_API void CalcPsychrometricsFromTWetBulb
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TWetBulb             // (i) Wet bulb temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  , double *HumRatio            // (o) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double *TDewPoint           // (o) Dew point temperature in °F [IP] or °C [SI]
  , double *RelHum              // (o) Relative humidity [0-1]
  , double *VapPres             // (o) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  , double *MoistAirEnthalpy    // (o) Moist air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹ [SI]
  , double *MoistAirVolume      // (o) Specific volume ft³ lb⁻¹ [IP] or in m³ kg⁻¹ [SI]
  , double *DegreeOfSaturation  // (o) Degree of saturation [unitless]
  );

UNI_API void CalcPsychrometricsFromTDewPoint
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double TDewPoint            // (i) Dew point temperature in °F [IP] or °C [SI]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  , double *HumRatio            // (o) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double *TWetBulb            // (o) Wet bulb temperature in °F [IP] or °C [SI]
  , double *RelHum              // (o) Relative humidity [0-1]
  , double *VapPres             // (o) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  , double *MoistAirEnthalpy    // (o) Moist air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹ [SI]
  , double *MoistAirVolume      // (o) Specific volume ft³ lb⁻¹ [IP] or in m³ kg⁻¹ [SI]
  , double *DegreeOfSaturation  // (o) Degree of saturation [unitless]
  );

UNI_API void CalcPsychrometricsFromRelHum
  ( double TDryBulb             // (i) Dry bulb temperature in °F [IP] or °C [SI]
  , double RelHum               // (i) Relative humidity [0-1]
  , double Pressure             // (i) Atmospheric pressure in Psi [IP] or Pa [SI]
  , double *HumRatio            // (o) Humidity ratio in lb_H₂O lb_Air⁻¹ [IP] or kg_H₂O kg_Air⁻¹ [SI]
  , double *TWetBulb            // (o) Wet bulb temperature in °F [IP] or °C [SI]
  , double *TDewPoint           // (o) Dew point temperature in °F [IP] or °C [SI]
  , double *VapPres             // (o) Partial pressure of water vapor in moist air in Psi [IP] or Pa [SI]
  , double *MoistAirEnthalpy    // (o) Moist air enthalpy in Btu lb⁻¹ [IP] or J kg⁻¹ [SI]
  , double *MoistAirVolume      // (o) Specific volume ft³ lb⁻¹ [IP] or in m³ kg⁻¹ [SI]
  , double *DegreeOfSaturation  // (o) Degree of saturation [unitless]
  );

#endif
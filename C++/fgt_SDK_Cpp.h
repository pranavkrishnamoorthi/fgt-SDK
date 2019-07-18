/*============================================================================*/
/*               Fluigent Software Developement Kit for C++                   */
/*----------------------------------------------------------------------------*/
/*         Copyright (c) Fluigent 2019.  All Rights Reserved.                 */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:   fgt_SDK_Cpp.h                                                     */
/* Purpose: Wrapper to fgt_SDK_*.dll				                          */
/*			Contains an interface to each dll function and type conversions	  */
/* Version: 1.0.0.0                                                           */
/* Date:	06/2019															  */
/*============================================================================*/


#pragma once

#include "fgt_SDK.h"

#include <iomanip> 
#include <cstdlib>
#include <iostream>
#include <windows.h>

#ifndef _FGT_SDK_CPP_H
#define _FGT_SDK_CPP_H
#pragma once


/*============================================================================*/
/*-------------  Custom definitions and functions section  -------------------*/
/*============================================================================*/

// Global variable, handles fgt_SDK_*.dll file
HINSTANCE proc_fgt_dll;

/** Internal function used to load fgt_SDK_*.dll file */
HINSTANCE fgt_Dll_Handle(void);

/** Manage pressure error and status, display details
 *  Change this function for custom error management, returned fgt_ERROR_CODE can directly be used in main application
 *  This functions calls Fgt_get_pressureStatus and displays error details */
void Fgt_Manage_Pressure_Status(unsigned int pressureIndex, std::string calledFunctionName);

/** Manage sensor error and status, display details
 *  Change this function for custom error management, returned fgt_ERROR_CODE can directly be used in main application
 *  This functions calls Fgt_get_sensorStatus and displays error details */
void Fgt_Manage_Sensor_Status(unsigned int sensorIndex, std::string calledFunctionName);

/** Manage generic functions error and status, display details
 *  Change this function for custom error management, returned fgt_ERROR_CODE can directly be used in main application */
void Fgt_Manage_Generic_Status(fgt_ERROR_CODE error, std::string calledFunctionName);


/*============================================================================*/
/*--------------------------  DLL Wrapper section  ---------------------------*/
/*============================================================================*/

/*============================================================================*/
/*-------------------------------  Init/close  -------------------------------*/
/*============================================================================*/

/**
 * @Description Initialize or reinitialize (if already opened) Fluigent SDK instance. All detected Fluigent instruments (MFCS, MFCS-EZ, FRP, LineUP) are initialized.
 * This function is optional, directly calling a function will automatically creates the instance.
 * @param void
 * @return fgt_ERROR_CODE
 * @see fgt_close
 * @see fgt_initEx
 */
fgt_ERROR_CODE Fgt_init(void);

/**
 * @Description Close communication with Fluigent instruments and free memory.
 * This function is mandatory, if not called the dll will be automatically closed when exiting your application.
 * Using this function will remove session preferences such as units and limits. If any regulation is running it will stop pressure control.
 */
fgt_ERROR_CODE Fgt_close(void);

/**
 * @Description Detects all connected Fluigent instrument(s), return their serial number and type
 * @param [SN]: Array of controllers serial number. This is a 256 pre-allocated table tailed with 0's when no instrument
 * @param [InstrType] : fgt_INSTRUMENT_TYPE. This is a 256 pre-allocated table tailed with 'None' value when no instrument
 * @return total number of detected instruments
 * @see fgt_initEx for specific initialization
 */
fgt_ERROR_CODE Fgt_detect(unsigned short SN[256], fgt_INSTRUMENT_TYPE type[256]);

/**
 * @Description Initialize specific Fluigent instrument(s) from their unique serial number. This function can be used when multiple instruments are connected in order to select your device(s)
 * @param [SN] Array of controllers serial number
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_initEx(unsigned short SN[256]);


/*============================================================================*/
/*-----------------------------  Channels info  ------------------------------*/
/*============================================================================*/

/**
 * @Description Retrieve information about session controllers. Controllers are MFCS, Flowboard, Link in an array.
 * @out [info] Array of structure of fgt_CONTROLLER_INFO
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_controllersInfo(fgt_CONTROLLER_INFO info[256]);

/**
 * @Description Get total number of initialized pressure channels. It is the sum of all MFCS, MFCS-EZ and FlowEZ pressure controllers
 * @param *nbPChan Total number of initialized pressure channels
 * @see fgt_get_pressureChannelsInfo array size is equal to nbPChan
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_pressureChannelCount(unsigned char* nbPChan);

/**
 * @Description Get total number of initialized sensor channels. It is the sum of all connected flow-units on Flowboard and FlowEZ
 * @param *nbQChan Total number of initialized sensor channels
 * @return fgt_ERROR_CODE
 * @see fgt_get_sensorChannelsInfo array size is equal to nbSChan
 */
fgt_ERROR_CODE Fgt_get_sensorChannelCount(unsigned char* nbSChan);


/**
 * @Description Get total number of initialized TTL channels. It is the sum of all connected Link TTL ports
 * @param *nbTtlChan Total number of initialized TTL channels
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_TtlChannelCount(unsigned char* nbTtlChan);

/**
 * @Description: Retrieve information about each initialized pressure channel. This function is useful in order to get channels order, controller, unique ID and InstrType.
 * By default this array is built with MFCS first, MFCS-EZ second and FlowEZ last. If only one instrument is used, index is the default channel indexing starting at 0.
 * You can initialize instruments in specific order using fgt_initEx function
 * @param info Array of structure of fgt_CHANNEL_INFO
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_pressureChannelsInfo(fgt_CHANNEL_INFO info[256]);

/**
 * @Description: Retrieve information about each initialized sensor channel. This function is useful in order to get channels order, controller, unique ID and InstrType.
 * By default this array is built with FRP first then FlowEZ and contains flow-units. If only one instrument is used, index is the default channel indexing starting at 0.
 * You can initialize instruments in specific order using fgt_initEx function
 * @param info Array of structure of fgt_CHANNEL_INFO
 * @param sensorType Array of fgt_SENSOR_TYPE
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_sensorChannelsInfo(fgt_CHANNEL_INFO info[256], fgt_SENSOR_TYPE sensorType[256]);

/**
 * @Description: Retrieve information about each initialized TTL channel. This function is useful in order to get channels order, controller, unique ID and InstrType.
 * TTL channels are only available for LineUP Series, 2 ports for each connected Link
 * @param info Array of structure of fgt_CHANNEL_INFO
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_TtlChannelsInfo(fgt_CHANNEL_INFO info[256]);


/*============================================================================*/
/*-----------------------------  Basic functions  ----------------------------*/
/*============================================================================*/

/**
 * @Description Send pressure command to selected device
 * @param pressureIndex Index of pressure channel or unique ID
 * @param pressure Pressure order in selected unit, default is "mbar"
 * @return errorCode
 * @see fgt_set_sensorRegulation
 * @see fgt_get_pressureStatus
 */
fgt_ERROR_CODE Fgt_set_pressure(unsigned int pressureIndex, float pressure);

/**
 * @Description Read pressure value of selected device
 * @param pressureIndex Index of pressure channel or unique ID
 * @out *pressure Read pressure value in selected unit, default is "mbar"
 * @return errorCode
 * @see fgt_get_pressureEx
 * @see fgt_get_pressureStatus
 */
fgt_ERROR_CODE Fgt_get_pressure(unsigned int pressureIndex, float* pressure);

/**
 * @Description Read pressure value and time stamp of selected device
 * @param pressureIndex Index of pressure channel or unique ID
 * @out *pressure Read pressure value in selected unit, default is "mbar"
 * @out *timeStamp Hardware timer in ms
 * @return fgt_ERROR_CODE
 * @see fgt_get_pressure
 * @see fgt_get_pressureStatus
 */
fgt_ERROR_CODE Fgt_get_pressureEx(unsigned int pressureIndex, float* pressure, unsigned short* timeStamp);

/**
 * @Description Start closed loop regulation between a sensor and a pressure controller. Pressure will be regulated in order to reach sensor setpoint.
 * Call again this function in order to change the setpoint. Calling fgt_set_pressure on same pressureIndex will stop regulation.
 * @param sensorIndex Index of sensor channel or unique ID
 * @param pressureIndex Index of pressure channel or unique ID
 * @param setpoint Regulation value to be reached in selected unit, default is "�l/min" for flowrate sensors
 * @return fgt_ERROR_CODE
 * @see fgt_set_pressure
 * @see fgt_set_customSensorRegulation
 * @see fgt_set_sensorRegulationResponse
 */
fgt_ERROR_CODE Fgt_set_sensorRegulation(unsigned int sensorIndex, unsigned int pressureIndex, float setpoint);

/**
 * @Description Read sensor value of selected device
 * @param sensorIndex Index of sensor channel or unique ID
 * @out value Read sensor value in selected unit, default is "�l/min" for flowrate sensors
 * @return fgt_ERROR_CODE
 * @see fgt_get_sensorStatus
 */
fgt_ERROR_CODE Fgt_get_sensorValue(unsigned int sensorIndex, float* value);

/**
 * @Description Read sensor value and timestamp of selected device
 * @param sensorIndex Index of sensor channel or unique ID
 * @out value Read sensor value in selected unit, default is "�l/min" for flowrate sensors
 * @out timeStamp Hardware timer in ms
 * @return fgt_ERROR_CODE
 * @see fgt_get_sensorStatus
 */
fgt_ERROR_CODE Fgt_get_sensorValueEx(unsigned int sensorIndex, float* value, unsigned short* timeStamp);


/*============================================================================*/
/*--------------------   Unit, calibration and limits  -----------------------*/
/*============================================================================*/

/**
 * @Description Set pressure unit for all initialized channels, default value is "mbar". If type is invalid an error is returned.
 * Every pressure read value and sent command will then use this unit.
 * Example of type: "mbar", "millibar", "kPa" ...
 * @param unit Array of char containing a unit string
 * @return fgt_ERROR_CODE
 * @see fgt_get_pressureStatus
 */
fgt_ERROR_CODE Fgt_set_sessionPressureUnit(std::string unit);

/**
 * @Description Set pressure unit on selected pressure device, default value is "mbar". If type is invalid an error is returned.
 * Every pressure read value and sent command will then use this unit.
 * Example of type: "mbar", "millibar", "kPa" ...
 * @param presureIndex Index of pressure channel or unique ID
 * @param unit[] Array of char containing a unit string
 * @return fgt_ERROR_CODE
 * @see fgt_get_pressureStatus
 */
fgt_ERROR_CODE Fgt_set_pressureUnit(unsigned int presureIndex, std::string unit);

/**
 * @Description Get used unit on selected pressure device, default value is "mbar".
 * Every pressure read value and sent command use this unit.
 * @param pressureIndex Index of pressure channel or unique ID
 * @out unit[] Array of char containing a unit string
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_pressureUnit(unsigned int pressureIndex, std::string* unit);

/**
 * @Description Set sensor unit on selected sensor device, default value is "�l/min" for flowunits. If type is invalid an error is returned.
 * Every sensor read value and regulation command will then use this unit.
 * Example of type: "�l/h", "ulperDay", "microliter/hour" ...
 * @param pressureIndex Index of pressure channel or unique ID
 * @param unit[] Array of char containing a unit string
 * @return fgt_ERROR_CODE
 * @see fgt_set_sensorStatus
 */
fgt_ERROR_CODE Fgt_set_sensorUnit(unsigned int sensorIndex, std::string unit);

/**
 * @Description Get used unit on selected sensor device, default value is "�l/min" for flowunits.
 * Every sensor read value and regulation command use this unit.
 * @param sensorIndex Index of sensor channel or unique ID
 * @out unit[] Array of char containing a unit string
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_sensorUnit(unsigned int sensorIndex, std::string* unit);

/**
 * @Description Set used sensor internal calibration table. Function is only available for specific sensors such as the flow-unit M accepting H2O and IPA
 * @param sensorIndex Index of sensor channel or unique ID
 * @param calibration fgt_SENSOR_CALIBRATION
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_sensorCalibration(unsigned int sensorIndex, fgt_SENSOR_CALIBRATION calibration);

/**
 * @Description Get internal used calibration table by the sensor.
 * @param sensorIndex Index of sensor channel or unique ID
 * @out *calibration fgt_SENSOR_CALIBRATION
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_sensorCalibration(unsigned int sensorIndex, fgt_SENSOR_CALIBRATION* calibration);

/**
 * @Description Apply a custom scale factor on sensor read value. This function is useful in order to adapt read sensor value to physical measurement.
 * For example if a flow-unit is used with a special oil and it's calibration table is set to H2O, read flowrate is not correct.
 * Scale factor is applied using following formula: scaled_value = a*sensor_value + b*sensor_value^2 + c*sensor_value^3
 * Note that this scale is also used for the regulation.
 * @param sensorIndex Index of sensor channel or unique ID
 * @param float a proportional multiplier value
 * @param float b square multiplier value
 * @param float c cubic multiplier value
 * @return fgt_ERROR_CODE
 * @see fgt_set_sensorCustomScaleEx
 */
fgt_ERROR_CODE Fgt_set_sensorCustomScale(unsigned int sensorIndex, float a, float b, float c);

/**
 * @Description Apply a custom scale factor on sensor read value. This function is useful in order to adapt read sensor value to physical measurement.
 * For example if a flow-unit is used with a special oil and it's calibration table is set to H2O, read flowrate is not correct.
 * Scale factor is applied using following formula: scaled_value = a*sensor_value + b*sensor_value^2 + c*sensor_value^3
 * When applying a custom scale factor, sensor range may increase very rapidly, SMax parameter is ment to limit this maximal value.
 * This function purpose is to be used with the regulation in order to avoid too high maximum range on the sensor.
 * @param sensorIndex Index of sensor channel or unique ID
 * @param float a proportional multiplier value
 * @param float b square multiplier value
 * @param float c cubic multiplier value
 * @param float SMax maximal tolerance of scale factor
 * @return fgt_ERROR_CODE
 * @see fgt_set_sensorCustomScale
 */
fgt_ERROR_CODE Fgt_set_sensorCustomScaleEx(unsigned int sensorIndex, float a, float b, float c, float SMax);

/**
 * @Description Calibrate internal pressure depending on atmospheric pressure. After calling this function 0 pressure value corresponds to atmospheric pressure.
 * During calibration step no pressure order is accepted. Total duration vary from 3s to 8s.
 * @param pressureIndex Index of pressure channel or unique ID
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_calibratePressure(unsigned int pressureIndex);


/**
 * @Description Start closed loop regulation between a sensor and a pressure controller. Pressure will be regulated in order to reach sensor setpoint.
 * Custom sensors, outside Fluigent ones, can be used such as different flow-units, pressure, level ...
 * However we do not guarantee full compatibility with all sensors. Regulation quality is linked to sensor precision and your set-up.
 * In order to use this function, custom used sensor maximum range and measured values has to be updated at least once per second.
 * Calling fgt_set_pressure on same pressureIndex or not updating sensor value over 1s will stop regulation.
 * @param measure custom sensor measured value, no unit is required
 * @param setpoint custom sensor regulation goal value, no unit is required
 * @param pressureIndex Index of pressure channel or unique ID
 * @param maxSensorRange custom sensor maximum range, no unit is required
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_customSensorRegulation(float measure, float setpoint, float maxSensorRange, unsigned int pressureIndex);

/**
 * @Description Get pressure controller minimum and maximum range. Returned values takes into account set unit, default value is 'mbar'.
 * @param pressureIndex Index of pressure channel or unique ID
 * @out Pmin minim device pressure
 * @out Pmax maximum device pressure
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_pressureRange(unsigned int pressureIndex, float* Pmin, float* Pmax);

/**
 * @Description Get sensor minimum and maximum range. Returned values takes into account set unit, default value is '�l/min' in case of flow-units.
 * @param sensorIndex Index of sensor channel or unique ID
 * @out Smin minimum measured sensor value
 * @out Smax maximum measured sensor value
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_sensorRange(unsigned int sensorIndex, float* Smin, float* Smax);

/**
 * @Description Set a limit, not to exceed on a pressure controller. It takes into account set unit, default value is 'mbar'.
 * This function is usefull when wanting to be sure not to go over a certain pressure value or during regulation.
 * Setting a limit is used for next called functions. Limits can not exceed controller range.
 * @param pressureIndex Index of pressure channel or unique ID
 * @param PlimMin minim admissible device pressure
 * @param PlimMax maximum admissible device pressure
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_pressureLimit(unsigned int pressureIndex, float PlimMin, float PlimMax);

/*============================================================================*/
/*------------------------   Regulation settings   ---------------------------*/
/*============================================================================*/

/**
 * @Description Set on a running regulation pressure response time. Minimal value is 2 for FlowEZ, 6 for MFCS controllers.
 * This function is useful if a more smooth response is wanted.
 * @param sensorIndex Index of sensor channel or unique ID
 * @param responseTime pressure response time in seconds
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_sensorRegulationResponse(unsigned int sensorIndex, unsigned int responseTime);

/**
 * @Description Set pressure controller response. This function can be used to customise response time for your set-up.
 * For FlowEZ available values are 0: use of fast switch vales or 1: do not use fast switch vales. Default value is 0.
 * For MFCS available values are from 1 to 255. Higher the value, longer is the response time. Default value is 5.
 * @param pressureIndex Index of pressure channel or unique ID
 * @param value desired pressure controller response time
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_pressureResponse(unsigned int pressureIndex, unsigned char value);


/*============================================================================*/
/*------------------------   Status information   ----------------------------*/
/*============================================================================*/

/**
 * @Description Get detailed information of pressure channel status. This function is ment to be invoked after calling a pressure related function which returns an error code.
 * Retrieved information of last error contains controller position and a string detail.
 * @param pressureIndex Index of pressure channel or unique ID
 * @out InstrType fgt_INSTRUMENT_TYPE, controller type
 * @out controllerSN serial number of controller (such as Link, MFCS)
 * @out information information status code, 1 if pressure module is controller locally
 * @out detail array of characters which details the error
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_pressureStatus(unsigned int pressureIndex, fgt_INSTRUMENT_TYPE* type, unsigned short* controllerSN, unsigned char* infoCode, std::string* detail);

/**
 * @Description Get detailed information of sensor status. This function is ment to be invoked after calling a sensor related function which returns an error code.
 * Retrieved information of last error contains sensor position and a string detail.
 * @param sensorIndex Index of sensor channel or unique ID
 * @out InstrType fgt_INSTRUMENT_TYPE, controller type
 * @out controllerSN serial number of controller (such as Link, Flowboard)
 * @out information information status code when regulating 0: Ok otherwise regulation might not be optimal
 * @out detail array of characters which details the error
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_sensorStatus(unsigned int sensorIndex, fgt_INSTRUMENT_TYPE* type, unsigned short* controllerSN, unsigned char* infoCode, std::string* detail);

/**
 * @Description Set power ON or OFF on a controller (such as Link, MFCS, Flowboard).
 * Not all controllers support this functionality.
 * @param controllerIndex Index of controller or unique ID
 * @param powerState fgt_POWER
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_power(unsigned short controllerIndex, fgt_POWER powerState);

/**
 * @Description Get power information about a controller (such as Link, MFCS, Flowboard).
 * Not all controllers support this functionality.
 * @param controllerIndex Index of controller or unique ID
 * @out powerState fgt_POWER
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_get_power(unsigned short controllerIndex, fgt_POWER* powerState);


/*============================================================================*/
/*-----------------------------  TTL functions  ------------------------------*/
/*============================================================================*/

/**
 * @Description Configure a specific TTL port (BNC ports) as input, output, rising or falling edge.
 * @param TtlIndex Index of TTL port or unique ID
 * @param mode fgt_TTL_MODE
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_TtlMode(unsigned int TtlIndex, fgt_TTL_MODE mode);

/**
 * @Description Read TTL ports (BNC ports) if set as input.
 * @param TtlIndex Index of TTL port or unique ID
 * @out state 0: no edge was detected; 1: an edge is detected
 * @return fgt_ERROR_CODE
 * @see fgt_set_TtlMode
 */
fgt_ERROR_CODE Fgt_read_Ttl(unsigned int TtlIndex, unsigned int* state);

/**
 * @Description Trigger a specific TTL port (BNC ports) if set as output.
 * @param TtlIndex Index of TTL port or unique ID
 * @return fgt_ERROR_CODE
 * @see fgt_set_TtlMode
 */
fgt_ERROR_CODE Fgt_trigger_Ttl(unsigned int TtlIndex);


/*============================================================================*/
/*-------------------------  Specific functions  -----------------------------*/
/*============================================================================*/

/**
 * @Description Activate/deactivate purge function.
 * This feature is only available on MFCS devices equipped with special valve.
 * @param controllerIndex Index of controller or unique ID
 * @param purge 0: OFF, 1:ON
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_purge(unsigned short controllerIndex, unsigned char purge);

/**
 * @Description Manually activate internal electrovalve. This stops pressure regulation.
 * This feature is only available on MFCS and MFCS-EZ devices.
 * @param pressureIndex Index of pressure channel or unique ID
 * @param value applied valve voltage from 0 to 100(%)
 * @return fgt_ERROR_CODE
 */
fgt_ERROR_CODE Fgt_set_manual(unsigned int pressureIndex, float value);


#endif
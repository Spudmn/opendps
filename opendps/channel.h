/*
 * Opendps
 * Copyright (C) 2018 Aaron Keith
 * 
 * Source modified from
 * EEZ PSU Firmware
 * Copyright (C) 2015-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OPENDPS_CHANNEL_H_
#define OPENDPS_CHANNEL_H_

#include <stdint.h>
#include "scpi/scpi.h"



void init_channel(void);





#define VOLTAGE_NUM_SIGNIFICANT_DECIMAL_DIGITS 3
#define CURRENT_NUM_SIGNIFICANT_DECIMAL_DIGITS 3
#define CURRENT_NUM_SIGNIFICANT_DECIMAL_DIGITS_R5B12 4
#define POWER_NUM_SIGNIFICANT_DECIMAL_DIGITS 3
#define DURATION_NUM_SIGNIFICANT_DECIMAL_DIGITS 4
#define TEMP_NUM_SIGNIFICANT_DECIMAL_DIGITS 0
#define RPM_NUM_SIGNIFICANT_DECIMAL_DIGITS 0
#define LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS 2


typedef enum {
    VALUE_TYPE_NONE,
    VALUE_TYPE_INT,
    VALUE_TYPE_FLOAT_FIRST,
  VALUE_TYPE_FLOAT,
    VALUE_TYPE_FLOAT_VOLT,
    VALUE_TYPE_FLOAT_MILLI_VOLT,
    VALUE_TYPE_FLOAT_AMPER,
    VALUE_TYPE_FLOAT_MILLI_AMPER,
  VALUE_TYPE_FLOAT_WATT,
  VALUE_TYPE_FLOAT_MILLI_WATT,
  VALUE_TYPE_FLOAT_SECOND,
    VALUE_TYPE_FLOAT_MILLI_SECOND,
  VALUE_TYPE_FLOAT_CELSIUS,
  VALUE_TYPE_FLOAT_RPM,
    VALUE_TYPE_FLOAT_OHM,
    VALUE_TYPE_FLOAT_KOHM,
    VALUE_TYPE_FLOAT_MOHM,
    VALUE_TYPE_FLOAT_LAST,
    VALUE_TYPE_LESS_THEN_MIN_FLOAT,
    VALUE_TYPE_GREATER_THEN_MAX_FLOAT = VALUE_TYPE_LESS_THEN_MIN_FLOAT + VALUE_TYPE_FLOAT_LAST - VALUE_TYPE_FLOAT_FIRST,
    VALUE_TYPE_CONST_STR = VALUE_TYPE_GREATER_THEN_MAX_FLOAT + VALUE_TYPE_FLOAT_LAST - VALUE_TYPE_FLOAT_FIRST,
    VALUE_TYPE_STR,
    VALUE_TYPE_CHANNEL_LABEL,
  VALUE_TYPE_CHANNEL_SHORT_LABEL,
  VALUE_TYPE_CHANNEL_BOARD_INFO_LABEL,
  VALUE_TYPE_LESS_THEN_MIN_INT,
  VALUE_TYPE_LESS_THEN_MIN_TIME_ZONE,
  VALUE_TYPE_GREATER_THEN_MAX_INT,
  VALUE_TYPE_GREATER_THEN_MAX_TIME_ZONE,
  VALUE_TYPE_EVENT,
  VALUE_TYPE_PAGE_INFO,
  VALUE_TYPE_ON_TIME_COUNTER,
  VALUE_TYPE_SCPI_ERROR_TEXT,
  VALUE_TYPE_TIME_ZONE,
  VALUE_TYPE_YEAR,
  VALUE_TYPE_MONTH,
  VALUE_TYPE_DAY,
  VALUE_TYPE_HOUR,
  VALUE_TYPE_MINUTE,
  VALUE_TYPE_SECOND,
  VALUE_TYPE_USER_PROFILE_LABEL,
  VALUE_TYPE_USER_PROFILE_REMARK,
    VALUE_TYPE_EDIT_INFO,
    VALUE_TYPE_IP_ADDRESS,
    VALUE_TYPE_ENUM
}ValueType ;



    /// Voltage and current data set and measured during runtime.
typedef struct {
        float set;
        float mon_dac;
        int16_t mon_adc;
        float mon;
        float step;
        float limit;

        float min;
        float def;
        float max;

        //void init(float set_, float step_, float limit_);
    }Value;



typedef struct
{

  Value u;
  Value i;

  float U_MIN_STEP;
  float U_DEF_STEP;
  float U_MAX_STEP;


  float I_MIN_STEP;
  float I_DEF_STEP;
  float I_MAX_STEP;



}Channel;




typedef struct  {
    const char *unitStr;
    int numSignificantDecimalDigits;
    float precision;
}ValueTypeTraits;

extern ValueTypeTraits g_valueTypeTraits[];
extern float g_precisions[];


Channel *param_channel(scpi_t *context);

float getPrecision(ValueType valueType);

bool isRemoteProgrammingEnabled();


#endif /* OPENDPS_CHANNEL_H_ */

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


#include <stdio.h>
#include <math.h>
#include "scpi_params.h"
#include "channel.h"
#include "channel_dispatcher.h"


Channel mChannel;


////                          I_MIN, I_DEF, I_MAX,  I_MAX_CONF, I_MIN_STEP, I_DEF_STEP, I_MAX_STEP, I_CAL_VAL_MIN, I_CAL_VAL_MID, I_CAL_VAL_MAX, I_VOLT_CAL
//#define CH_PARAMS_I_3A      0.0f,  0.0f,  3.125f, 3.125f,     0.01f,      0.01f,      1.0f,       0.05f,         1.525f,        3.0f,          0.05f,      CH_PARAMS_OCP
//#define CH_PARAMS_I_5A      0.0f,  0.0f,  5.0f,   5.125f,     0.01f,      0.01f,      1.0f,       0.05f,         2.425f,        4.8f,          0.05f,      CH_PARAMS_OCP
//#define CH_PARAMS_I_5A_R5B9 0.0f,  0.0f,  5.0f,   5.0f,       0.01f,      0.01f,      1.0f,       0.05f,         2.425f,        4.8f,          0.05f,      CH_PARAMS_OCP

////                           U_MIN, U_DEF, U_MAX, U_MAX_CONF, U_MIN_STEP, U_DEF_STEP, U_MAX_STEP, U_CAL_VAL_MIN, U_CAL_VAL_MID, U_CAL_VAL_MAX, U_CURR_CAL
//#define CH_PARAMS_U_30V      0.0f,  0.0f,  30.0f, 30.0f,      0.01f,      0.1f,       5.0f,       0.15f,         14.1f,         28.0f,         25.0f,     CH_PARAMS_OVP
//#define CH_PARAMS_U_40V      0.0f,  0.0f,  40.0f, 41.33f,     0.01f,      0.1f,       5.0f,       0.15f,         19.1f,         38.0f,         25.0f,     CH_PARAMS_OVP
//#define CH_PARAMS_U_40V_R5B9 0.0f,  0.0f,  40.0f, 40.0f,      0.01f,      0.1f,       5.0f,       0.15f,         19.1f,         38.0f,         25.0f,     CH_PARAMS_OVP
//#define CH_PARAMS_U_50V      0.0f,  0.0f,  50.0f, 50.0f,      0.01f,      0.1f,       5.0f,       0.15f,         24.1f,         48.0f,         25.0f,     CH_PARAMS_OVP



void init_channel(void)
{

  mChannel.U_MIN_STEP = 0.01f;
  mChannel.U_DEF_STEP = 0.1f;
  mChannel.U_MAX_STEP = 5.0f;


  mChannel.I_MIN_STEP = 0.01f;
  mChannel.I_DEF_STEP = 0.01f;
  mChannel.I_MAX_STEP = 1.0f;


  mChannel.u.step = mChannel.U_DEF_STEP;
  mChannel.i.step = mChannel.I_DEF_STEP;


}


ValueTypeTraits g_valueTypeTraits[] = {
    { "",     2                                         , powf(10.0f, 2.0f)                                               },
    { "V",    VOLTAGE_NUM_SIGNIFICANT_DECIMAL_DIGITS    , powf(10.0f, (float)VOLTAGE_NUM_SIGNIFICANT_DECIMAL_DIGITS)      },
    { "mV",   VOLTAGE_NUM_SIGNIFICANT_DECIMAL_DIGITS-3  , powf(10.0f, (float)(VOLTAGE_NUM_SIGNIFICANT_DECIMAL_DIGITS-3))  },
    { "A",    CURRENT_NUM_SIGNIFICANT_DECIMAL_DIGITS    , powf(10.0f, (float)CURRENT_NUM_SIGNIFICANT_DECIMAL_DIGITS)      },
    { "mA",   CURRENT_NUM_SIGNIFICANT_DECIMAL_DIGITS-3  , powf(10.0f, (float)(CURRENT_NUM_SIGNIFICANT_DECIMAL_DIGITS-3))  },
    { "W",    POWER_NUM_SIGNIFICANT_DECIMAL_DIGITS      , powf(10.0f, (float)POWER_NUM_SIGNIFICANT_DECIMAL_DIGITS)        },
    { "mW",   POWER_NUM_SIGNIFICANT_DECIMAL_DIGITS-3    , powf(10.0f, (float)(POWER_NUM_SIGNIFICANT_DECIMAL_DIGITS-3))    },
    { "s",    DURATION_NUM_SIGNIFICANT_DECIMAL_DIGITS   , powf(10.0f, (float)DURATION_NUM_SIGNIFICANT_DECIMAL_DIGITS)     },
    { "ms",   DURATION_NUM_SIGNIFICANT_DECIMAL_DIGITS-3 , powf(10.0f, (float)(DURATION_NUM_SIGNIFICANT_DECIMAL_DIGITS-3)) },
    { "oC",   TEMP_NUM_SIGNIFICANT_DECIMAL_DIGITS       , powf(10.0f, (float)TEMP_NUM_SIGNIFICANT_DECIMAL_DIGITS)         },
    { "rpm",  RPM_NUM_SIGNIFICANT_DECIMAL_DIGITS        , powf(10.0f, (float)RPM_NUM_SIGNIFICANT_DECIMAL_DIGITS)          },
    { "ohm",  LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS       , powf(10.0f, (float)LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS)         },
    { "Kohm", LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS       , powf(10.0f, (float)LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS)         },
    { "Mohm", LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS       , powf(10.0f, (float)LOAD_NUM_SIGNIFICANT_DECIMAL_DIGITS)         }
};

float g_precisions[] = {
    1.0f,
    10.0f,
    100.0f,
    1000.0f,
    10000.0f,
    100000.0f
};




int getNumSignificantDecimalDigits(ValueType valueType) {
    return g_valueTypeTraits[valueType - VALUE_TYPE_FLOAT].numSignificantDecimalDigits;
}







scpi_result_t result_float(scpi_t *context, Channel *channel, float value, ValueType valueType) {
  (void) channel;
  char buffer[32] = { 0 };

    int numSignificantDecimalDigits = getNumSignificantDecimalDigits(valueType);

    strcatFloat(buffer, value, numSignificantDecimalDigits);
    SCPI_ResultCharacters(context, buffer, strlen(buffer));
    return SCPI_RES_OK;
}


Channel *set_channel_from_command_number(scpi_t *context) {
  //opendps only has one channel
  (void) context;
  return &mChannel;
}


float getPrecision(ValueType valueType) {
    return g_valueTypeTraits[valueType - VALUE_TYPE_FLOAT].precision;
}



Channel *param_channel(scpi_t *context)
{
//opendps only has one channel
(void) context;
return &mChannel;
}


bool isRemoteProgrammingEnabled() {
    return false;
}




bool get_current_from_param(scpi_t *context, const scpi_number_t *param, float *value, const Channel *channel, const Value *cv) {
    if (param->special) {
        if (param->content.tag == SCPI_NUM_MAX) {
            *value = getIMax(channel);
        }
        else if (param->content.tag == SCPI_NUM_MIN) {
            *value = getIMin(channel);
        }
        else if (param->content.tag == SCPI_NUM_DEF) {
            *value = getIDef(channel);
        }
        else if (param->content.tag == SCPI_NUM_UP && cv) {
            *value = cv->set + cv->step;
            if (*value > getIMax(channel)) *value = getIMax(channel);
        }
        else if (param->content.tag == SCPI_NUM_DOWN && cv) {
            *value = cv->set - cv->step;
            if (*value < getIMin(channel)) *value = getIMin(channel);
        }
        else {
            SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return false;
        }
    }
    else {
        if (param->unit != SCPI_UNIT_NONE && param->unit != SCPI_UNIT_AMPER) {
            SCPI_ErrorPush(context, SCPI_ERROR_INVALID_SUFFIX);
            return false;
        }

        *value = (float)param->content.value;
        if (*value < getIMin(channel) || *value > getIMax(channel)) {
            SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
            return false;
        }
    }

    return true;
}


bool get_current_param(scpi_t *context, float *value, const Channel *channel, const Value *cv) {
    scpi_number_t param;
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, true)) {
        return false;
    }

    return get_current_from_param(context, &param, value, channel, cv);
}




bool get_voltage_from_param(scpi_t *context, const scpi_number_t *param, float *value, const Channel *channel, const Value *cv) {
    if (param->special) {
		if (channel) {
			if (param->content.tag == SCPI_NUM_MAX) {
				*value = getUMax(channel);
			}
			else if (param->content.tag == SCPI_NUM_MIN) {
				*value = getUMin(channel);
			}
			else if (param->content.tag == SCPI_NUM_DEF) {
				*value = getUDef(channel);
			}
			else if (param->content.tag == SCPI_NUM_UP && cv) {
				*value = cv->set + cv->step;
				if (*value > getUMax(channel)) *value = getUMax(channel);
			}
			else if (param->content.tag == SCPI_NUM_DOWN && cv) {
				*value = cv->set - cv->step;
				if (*value < getUMin(channel)) *value = getUMin(channel);
			}
			else {
				SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
				return false;
			}
		} else {
			SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
			return false;
		}
    }
    else {
        if (param->unit != SCPI_UNIT_NONE && param->unit != SCPI_UNIT_VOLT) {
            SCPI_ErrorPush(context, SCPI_ERROR_INVALID_SUFFIX);
            return false;
        }

        *value = (float)param->content.value;

		if (channel) {
			if (*value < getUMin(channel) || *value > getUMax(channel)) {
				SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
				return false;
			}
		}
    }

    return true;
}




bool get_voltage_param(scpi_t *context, float *value, const Channel *channel, const Value *cv) {
    scpi_number_t param;
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, true)) {
        return false;
    }

    return get_voltage_from_param(context, &param, value, channel, cv);
}



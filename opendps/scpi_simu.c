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



#include "scpi/scpi.h"
#include "scpi/types.h"
#include "simulator.h"
#include "scpi_params.h"
#include "channel_dispatcher.h"




#ifdef DPS_EMULATOR


scpi_result_t scpi_cmd_simulatorExit(scpi_t *context) {
  (void) context;
  mainwindow_exit();
  return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_simulatorQuit(scpi_t *context) {
    return scpi_cmd_simulatorExit(context);
}




scpi_result_t scpi_cmd_simulatorVoltageExternal(scpi_t *context) {



    /* read first parameter if present */
    scpi_bool_t res;
    scpi_number_t param1;
    res = SCPI_ParamNumber(context, scpi_special_numbers_def, &param1, TRUE);
    if (!res)
    {
        return SCPI_RES_ERR;
    }

    simulator_setVoltExt((uint32_t)(param1.content.value*1000.0));


    return SCPI_RES_OK;
}



scpi_result_t scpi_cmd_simulatorVoltageExternalQ(scpi_t *context) {

  double dExt_Volts = simulator_getVoltExt() / 1000.0;
  SCPI_ResultDouble(context, dExt_Volts);
 return SCPI_RES_OK;

}


////////////////////////////////////////////////////////////////////////////////



bool get_resistance_from_param(scpi_t *context, const scpi_number_t *param, float *value) {
    if (param->special) {
        if (param->content.tag == SCPI_NUM_MAX) {
            *value = SIM_LOAD_MAX;
        }
        else if (param->content.tag == SCPI_NUM_MIN) {
            *value = SIM_LOAD_MIN;
        }
        else if (param->content.tag == SCPI_NUM_DEF) {
            *value = SIM_LOAD_DEF;
        }
        else if (param->content.tag == SCPI_NUM_INF) {
            *value = INFINITY;
        }
        else {
            SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return false;
        }
    }
    else {
        if (param->unit != SCPI_UNIT_NONE && param->unit != SCPI_UNIT_OHM) {
            SCPI_ErrorPush(context, SCPI_ERROR_INVALID_SUFFIX);
            return false;
        }

        *value = (float)param->content.value;
        if (*value < SIM_LOAD_MIN || *value > SIM_LOAD_MAX) {
            SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
            return false;
        }
    }
    return true;
}

static bool get_resistance_param(scpi_t *context, float *value) {
    scpi_number_t param;
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, true)) {
        return false;
    }

    return get_resistance_from_param(context, &param, value);
}


scpi_result_t scpi_cmd_simulatorLoad(scpi_t *context) {
    float value;
    if (!get_resistance_param(context, &value)) {
        return SCPI_RES_ERR;
    }

    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    setLoad(channel, value);

    return SCPI_RES_OK;
}




scpi_result_t scpi_cmd_simulatorLoadQ(scpi_t *context) {
    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    float value;

    int32_t spec;
    if (!SCPI_ParamChoice(context, scpi_special_numbers_def, &spec, false)) {
        if (SCPI_ParamErrorOccurred(context)) {
            return SCPI_RES_ERR;
        }

        value = simulator_getLoad();
    }
    else {
        if (spec == SCPI_NUM_MIN) {
            value = SIM_LOAD_MIN;
        }
        else if (spec == SCPI_NUM_MAX) {
            value = SIM_LOAD_MAX;
        }
        else if (spec == SCPI_NUM_DEF) {
            value = SIM_LOAD_DEF;
        }
        else {
            SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
        }
    }

    return result_float(context, channel, value, VALUE_TYPE_FLOAT_OHM);
}





#else  //Not DPS_EMULATOR


scpi_result_t scpi_cmd_simulatorExit(scpi_t *context) {
    SCPI_ErrorPush(context, SCPI_ERROR_UNDEFINED_HEADER);
    return SCPI_RES_ERR;
}

scpi_result_t scpi_cmd_simulatorQuit(scpi_t *context) {
    SCPI_ErrorPush(context, SCPI_ERROR_UNDEFINED_HEADER);
    return SCPI_RES_ERR;
}


scpi_result_t scpi_cmd_simulatorVoltageProgramExternal(scpi_t *context) {
    SCPI_ErrorPush(context, SCPI_ERROR_UNDEFINED_HEADER);
    return SCPI_RES_ERR;
}

scpi_result_t scpi_cmd_simulatorVoltageProgramExternalQ(scpi_t *context) {
    SCPI_ErrorPush(context, SCPI_ERROR_UNDEFINED_HEADER);
    return SCPI_RES_ERR;
}

#endif

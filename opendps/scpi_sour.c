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
#include "uui.h"
#include "channel.h"
#include "channel_dispatcher.h"
#include "scpi_params.h"
#include "opendps.h"
#include "hw.h"


#define SCPI_ERROR_CURRENT_LIMIT_EXCEEDED                   152  //todo need to fix up scpi user config
#define SCPI_ERROR_POWER_LIMIT_EXCEEDED                       150
#define SCPI_ERROR_VOLTAGE_LIMIT_EXCEEDED                     151
////////////////////////////////////////////////////////////////////////////////


static scpi_result_t get_source_value_min_max_def(scpi_t *context, Channel *channel, ValueType valueType, float value, float min, float max, float def) {
    int32_t spec;
    if (!SCPI_ParamChoice(context, scpi_special_numbers_def, &spec, false)) {
        if (SCPI_ParamErrorOccurred(context)) {
            return SCPI_RES_ERR;
        }
    }
    else {
        if (spec == SCPI_NUM_MIN) {
            value = min;
        }
        else if (spec == SCPI_NUM_MAX) {
            value = max;
        }
        else if (spec == SCPI_NUM_DEF) {
            value = def;
        }
        else {
            SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
        }
    }

    return result_float(context, channel, value, valueType);
}





scpi_result_t scpi_cmd_sourceCurrentLevelImmediateAmplitude(scpi_t * context) {
	Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }


    float current;
    if (!get_current_param(context, &current, channel, &channel->i)) {
        return SCPI_RES_ERR;
    }

	if (greater(current, getILimit(channel), getPrecision(VALUE_TYPE_FLOAT_AMPER))) {
        SCPI_ErrorPush(context, SCPI_ERROR_CURRENT_LIMIT_EXCEEDED);
        return SCPI_RES_ERR;
	}

    if (greater(current * getUSet(channel), getPowerLimit(channel), getPrecision(VALUE_TYPE_FLOAT_WATT))) {
        SCPI_ErrorPush(context, SCPI_ERROR_POWER_LIMIT_EXCEEDED);
        return SCPI_RES_ERR;
    }

    setCurrent(channel, current);

    return SCPI_RES_OK;
}


scpi_result_t scpi_cmd_sourceCurrentLevelImmediateAmplitudeQ(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    return get_source_value_min_max_def(context,
        channel,
        VALUE_TYPE_FLOAT_AMPER,
        getISet(channel),
        getIMin(channel),
        getIMax(channel),
        getIDef(channel));
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitude(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }



    float voltage;
    if (!get_voltage_param(context, &voltage, channel, &channel->u)) {
        return SCPI_RES_ERR;
    }

	if (isRemoteProgrammingEnabled()) {
        SCPI_ErrorPush(context, SCPI_ERROR_EXECUTION_ERROR);
        return SCPI_RES_ERR;
	}

	if (greater(voltage, getULimit(channel), getPrecision(VALUE_TYPE_FLOAT_VOLT))) {
        SCPI_ErrorPush(context, SCPI_ERROR_VOLTAGE_LIMIT_EXCEEDED);
        return SCPI_RES_ERR;
	}

	if (greater(voltage * getISet(channel), getPowerLimit(channel), getPrecision(VALUE_TYPE_FLOAT_WATT))) {
        SCPI_ErrorPush(context, SCPI_ERROR_POWER_LIMIT_EXCEEDED);
        return SCPI_RES_ERR;
    }

    setVoltage(channel, voltage);

    return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitudeQ(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

	float u;
	if (isRemoteProgrammingEnabled()) {
		u = channel->u.mon_dac;
	} else {
		u = getUSet(channel);
	}

    return get_source_value_min_max_def(context,
        channel,
        VALUE_TYPE_FLOAT_VOLT,
        u,
        getUMin(channel),
        getUMax(channel),
        getUDef(channel));
}



////////////////////////////////////////////////////////////////////////////////

static scpi_result_t set_step(scpi_t * context, Value *cv, float min_value, float max_value, float def, enum _scpi_unit_t unit) {
    scpi_number_t step_param;
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &step_param, true)) {
        return SCPI_RES_ERR;
    }

    float step;

    if (step_param.special) {
        if (step_param.content.tag == SCPI_NUM_DEF) {
            step = def;
        }
        else {
            SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
        }
    }
    else {
        if (step_param.unit != SCPI_UNIT_NONE && step_param.unit != unit) {
            SCPI_ErrorPush(context, SCPI_ERROR_INVALID_SUFFIX);
            return SCPI_RES_ERR;
        }

        step = (float)step_param.content.value;
        if (step < min_value || step > max_value) {
            SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
            return SCPI_RES_ERR;
        }
    }

    cv->step = step;
//    profile::save();  //todo

    return SCPI_RES_OK;

}



static scpi_result_t get_source_value(scpi_t *context, Channel *channel, ValueType valueType, float value, float def) {
    int32_t spec;
    if (!SCPI_ParamChoice(context, scpi_special_numbers_def, &spec, false)) {
        if (SCPI_ParamErrorOccurred(context)) {
            return SCPI_RES_ERR;
        }
    }
    else {
        if (spec == SCPI_NUM_DEF) {
            value = def;
        }
        else {
            SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
        }
    }

    return result_float(context, channel, value, valueType);
}


/////////////////////////////Step///////////////////////////////////////////////////

scpi_result_t scpi_cmd_sourceCurrentLevelImmediateStepIncrement(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    return set_step(context, &channel->i, channel->I_MIN_STEP, channel->I_MAX_STEP, channel->I_DEF_STEP, SCPI_UNIT_AMPER);
}

scpi_result_t scpi_cmd_sourceCurrentLevelImmediateStepIncrementQ(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    return get_source_value(context, channel, VALUE_TYPE_FLOAT_AMPER, channel->i.step, channel->I_DEF_STEP);
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateStepIncrement(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    return set_step(context, &channel->u, channel->U_MIN_STEP, channel->U_MAX_STEP, channel->U_DEF_STEP, SCPI_UNIT_VOLT);
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateStepIncrementQ(scpi_t * context) {
    Channel *channel = set_channel_from_command_number(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    return get_source_value(context, channel, VALUE_TYPE_FLOAT_VOLT, channel->u.step, channel->U_DEF_STEP);
}

////////////////////////////////////////////////////////////////////////////////












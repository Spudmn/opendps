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
#include "scpi_params.h"
#include "channel_dispatcher.h"

#include "pwrctl.h"
#include "hw.h"





scpi_result_t scpi_cmd_measureScalarVoltageDcQ(scpi_t * context) {
    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    char buffer[256] = { 0 };
    strcatFloat(buffer, getUMon(channel), getNumSignificantDecimalDigits(VALUE_TYPE_FLOAT_VOLT));
    SCPI_ResultCharacters(context, buffer, strlen(buffer));

    return SCPI_RES_OK;
}



////////////////////////////////////////////////////////////////////////////////


scpi_result_t scpi_cmd_measureScalarInputVoltageDcQ(scpi_t * context) {
    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    char buffer[256] = { 0 };
    strcatFloat(buffer, getInUMon(channel), getNumSignificantDecimalDigits(VALUE_TYPE_FLOAT_VOLT));
    SCPI_ResultCharacters(context, buffer, strlen(buffer));

    return SCPI_RES_OK;
}



////////////////////////////////////////////////////////////////////////////////

scpi_result_t scpi_cmd_measureScalarCurrentDcQ(scpi_t * context) {
    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    char buffer[256] = { 0 };
    strcatFloat(buffer, getIMon(channel), getNumSignificantDecimalDigits(VALUE_TYPE_FLOAT_AMPER));
    SCPI_ResultCharacters(context, buffer, strlen(buffer));

    return SCPI_RES_OK;
}


////////////////////////////////////////////////////////////////////////////////

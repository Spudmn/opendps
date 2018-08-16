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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "scpi/scpi.h"
#include "scpi_params.h"
#include "channel_dispatcher.h"

#include "pwrctl.h"
#include "hw.h"




////////////////////////////////////////////////////////////////////////////////


scpi_result_t scpi_cmd_outputState(scpi_t * context) {
    bool enable;
    if (!SCPI_ParamBool(context, &enable, TRUE)) {
        return SCPI_RES_ERR;
    }

    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

//    if (enable != channel->isOutputEnabled()) {
//        if (enable) {
//            if (channel_dispatcher::isTripped(*channel)) {
//                SCPI_ErrorPush(context, SCPI_ERROR_CANNOT_EXECUTE_BEFORE_CLEARING_PROTECTION);
//                return SCPI_RES_OK;
//            }
//        }
//        else {
//            if (calibration::isEnabled()) {
//                SCPI_ErrorPush(context, SCPI_ERROR_CAL_OUTPUT_DISABLED);
//            }
//        }

        outputEnable(channel, enable);
//    }

    return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_outputStateQ(scpi_t * context) {
    Channel *channel = param_channel(context);
    if (!channel) {
        return SCPI_RES_ERR;
    }

    SCPI_ResultBool(context, pwrctl_vout_enabled());

    return SCPI_RES_OK;
}




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

#ifndef OPENDPS_SCPI_PARAMS_H_
#define OPENDPS_SCPI_PARAMS_H_

#include "scpi/scpi.h"
#include "channel.h"
#include "util.h"

int getNumSignificantDecimalDigits(ValueType valueType);
scpi_result_t result_float(scpi_t *context, Channel *channel, float value, ValueType valueType);
Channel *set_channel_from_command_number(scpi_t *context);

bool get_current_param(scpi_t *context, float *value, const Channel *channel, const Value *cv);
bool get_voltage_param(scpi_t *context, float *value, const Channel *channel, const Value *cv);



#endif /* OPENDPS_SCPI_PARAMS_H_ */

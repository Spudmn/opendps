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

#ifndef OPENDPS_CHANNEL_DISPATCHER_H_
#define OPENDPS_CHANNEL_DISPATCHER_H_


#include "channel.h"


float getUMon(const Channel *channel);
float getInUMon(const Channel *channel);
float getIMon(const Channel *channel);

void outputEnable(Channel* channel, bool enable);


#ifdef DPS_EMULATOR
void setLoadEnabled(Channel *channel, bool state);
void setLoad(Channel *channel, float load);
#endif


float getILimit(const Channel *channel);

float getUSet(const Channel *channel);
float getPowerLimit(const Channel* channel);

void setCurrent(Channel *channel, float current);




float getISet(const Channel *channel);
float getIMin(const Channel *channel);

float getIDef(const Channel *channel);
float getIMax(const Channel *channel);


float getULimit(const Channel *channel);

float getUMin(const Channel *channel);
float getUDef(const Channel *channel);
float getUMax(const Channel *channel);
void setVoltage(Channel *channel, float voltage);



#endif /* OPENDPS_CHANNEL_DISPATCHER_H_ */

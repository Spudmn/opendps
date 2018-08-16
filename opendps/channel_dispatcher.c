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


#include "channel_dispatcher.h"
#include "pwrctl.h"
#include "hw.h"
#include "opendps.h"



float getUMon(const Channel *channel) {
	(void) channel;
	uint16_t i_out_raw, v_in_raw, v_out_raw;
	hw_get_adc_values(&i_out_raw, &v_in_raw, &v_out_raw);

	float fOut_Volts = pwrctl_calc_vout(v_out_raw) ;

	return fOut_Volts/1000.0f;
}



float getInUMon(const Channel *channel) {
	(void) channel;
	uint16_t i_out_raw, v_in_raw, v_out_raw;
	hw_get_adc_values(&i_out_raw, &v_in_raw, &v_out_raw);

	float fIn_Volts = pwrctl_calc_vin(v_in_raw) ;

	return fIn_Volts/1000.0f;
}




float getIMon(const Channel *channel) {
	(void) channel;
	uint16_t i_out_raw, v_in_raw, v_out_raw;
	hw_get_adc_values(&i_out_raw, &v_in_raw, &v_out_raw);

	float fCurrent = pwrctl_calc_iout(i_out_raw);

	return fCurrent / 1000.0f;
}



void outputEnable(Channel* channel, bool enable) {
	(void) channel;

	opendps_enable_output(enable);
}




#ifdef DPS_EMULATOR
//void setLoadEnabled(Channel *channel, bool state) {
//	(void) channel;
//}

void setLoad(Channel *channel, float load) {
	(void) channel;
	simulator_setLoad((uint32_t)load);
}
#endif




float getILimit(const Channel *channel) {
	(void) channel;
    return CONFIG_DPS_MAX_CURRENT;
}


float getUSet(const Channel *channel) {
	(void) channel;

	ui_parameter_t *params;
	char value[16];

	opendps_get_curr_function_params(&params);
	opendps_get_curr_function_param_value(params[0].name, value, sizeof(value));

	int ivoltage = atoi(value);
	return ivoltage / 1000.0f;
}



float getPowerLimit(const Channel* channel) {
	(void) channel;
    return 500.0f; //todo
}




void setCurrent(Channel *channel, float current) {
	channel->i.set = current;
	char buffer[20];
	snprintf(buffer,sizeof(buffer), "%f",current*1000.0f );
	opendps_set_parameter("current",buffer);


//todo    channel.setCurrent(current);

}





float getISet(const Channel *channel) {
	(void) channel;

	ui_parameter_t *params;
	char value[16];

	opendps_get_curr_function_params(&params);
	opendps_get_curr_function_param_value(params[1].name, value, sizeof(value));

	int icurrent = atoi(value);
	return  icurrent / 1000.0f;
}


float getIMin(const Channel *channel) {
	(void) channel;
    return 0.0f;  //todo
}


float getIDef(const Channel *channel) {
	(void) channel;
    return CONFIG_DEFAULT_ILIMIT / 1000.0f;
}

float getIMax(const Channel *channel) {
	(void) channel;
    return CONFIG_DPS_MAX_CURRENT / 1000.0f;  //todo
}





float getULimit(const Channel *channel) {
	(void) channel;
    return CONFIG_DPS_MAX_VOLTAGE/1000.0f;
}



float getUMin(const Channel *channel) {
	(void) channel;
    return 0.0f;  //todo
}

float getUDef(const Channel *channel) {
	(void) channel;
    return CONFIG_DEFAULT_VOUT / 1000.0f;
}

float getUMax(const Channel *channel) {
	(void) channel;
    return CONFIG_DPS_MAX_VOLTAGE/1000.0f;
}



void setVoltage(Channel *channel, float voltage) {

	channel->u.set = voltage;

	char buffer[20];

	Set_Fake_V_Out(voltage*1000.0f);

	snprintf(buffer,sizeof(buffer), "%f",voltage*1000.0f );
	opendps_set_parameter("voltage",buffer);

}







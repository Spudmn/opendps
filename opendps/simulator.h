/*
 * Opendps
 * Copyright (C) 2018 Aaron Keith
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
 *
 */

#ifndef OPENDPS_SIMULATOR_H_
#define OPENDPS_SIMULATOR_H_

#ifdef DPS_EMULATOR


// SIMULATOR SPECIFC CONFIG
#define SIM_LOAD_MIN 0
#define SIM_LOAD_DEF 1000.0f
#define SIM_LOAD_MAX 10000000.0F

void mainwindow_exit();

void simulator_setVoltExt(uint32_t v_ext);
uint32_t simulator_getVoltExt(void);
void simulator_setLoad(uint32_t load);
uint32_t simulator_getLoad(void);
void simulator_setLoad(uint32_t load);
void Set_Fake_V_Out(uint32_t v_out);



#endif

#endif /* OPENDPS_SIMULATOR_H_ */

/*
 * opendps_sim.h
 *
 *  Created on: 3/08/2018
 *      Author: aaron.keith
 */

#ifndef SIMULATOR_OPENDPS_SIM_H_
#define SIMULATOR_OPENDPS_SIM_H_

#include "past.h"

int sim_main(int argc, char const *argv[]);
void dps_emul_init(past_t *past, int argc, char const *argv[]);
void sim_event_handler(void);


#endif /* SIMULATOR_OPENDPS_SIM_H_ */

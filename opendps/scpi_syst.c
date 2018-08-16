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



/// Text returned by the SYStem:CAPability command
#define STR_SYST_CAP "DCSUPPLY WITH (MEASURE|MULTIPLE|TRIGGER)"


scpi_result_t scpi_cmd_systemCapabilityQ(scpi_t * context) {
    SCPI_ResultText(context, STR_SYST_CAP);

    return SCPI_RES_OK;
}


scpi_result_t scpi_cmd_systemErrorNextQ(scpi_t * context) {
    return SCPI_SystemErrorNextQ(context);
}

scpi_result_t scpi_cmd_systemErrorCountQ(scpi_t * context) {
    return SCPI_SystemErrorCountQ(context);
}

scpi_result_t scpi_cmd_systemVersionQ(scpi_t * context) {
    return SCPI_SystemVersionQ(context);
}

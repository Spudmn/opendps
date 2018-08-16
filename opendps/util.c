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


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void strcatFloat(char *str, float value, int numSignificantDecimalDigits) {
    // mitigate "-0.00" case
    float min = (float) (1.0f / pow(10, numSignificantDecimalDigits));
    if (fabs(value) < min) {
        value = 0;
    }

    str = str + strlen(str);

//#if defined(_VARIANT_ARDUINO_DUE_X_) || defined(EEZ_PSU_SIMULATOR)
    sprintf(str, "%.*f", numSignificantDecimalDigits, value);
//#else
    //dtostrf(value, 0, precision, str);
//#endif

    // remove trailing zeros
    /*
    str = str + strlen(str) - 1;
    while (*str == '0' && *(str - 1) != '.') {
        *str-- = 0;
    }
    */
}


bool equal(float a, float b, float prec) {
	return roundf(a * prec) == roundf(b * prec);
}


bool greater(float a, float b, float prec) {
	return a > b && !equal(a, b, prec);
}

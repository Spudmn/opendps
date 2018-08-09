/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Johan Kanflo (github.com/kanflo)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "dps-model.h"
#include "pwrctl.h"
//#include "channel.h"

static uint32_t Fake_In_Voltage_mV = 24100;
static uint32_t Fake_Out_Voltage_mV = 5000;
static uint32_t Fake_Load = 100;



void simulator_setVoltExt(uint32_t v_in)
{
  Fake_In_Voltage_mV = v_in;
  printf("simulator_setVoltExt %d\n",Fake_In_Voltage_mV);
}


uint32_t simulator_getVoltExt(void)
{
  return Fake_In_Voltage_mV;
}


void simulator_setLoad(uint32_t load)
{
  Fake_Load = load;
  printf("simulator_setLoad %d\n",load);
}

uint32_t simulator_getLoad(void)
{
  return Fake_Load;
}



void Set_Fake_V_Out(uint32_t v_out)
{
  Fake_Out_Voltage_mV = v_out;
}



/**
  * @brief Initialize the hardware
  * @retval None
  */
void hw_init(void)
{
//  init_channel();  todo put back
}

/**
  * @brief Read latest ADC mesurements
  * @param i_out_raw latest I_out raw value
  * @param v_in_raw latest V_in raw value
  * @param v_out_raw latest V_out raw value
  * @retval none
  */
void hw_get_adc_values(uint16_t *i_out_raw, uint16_t *v_in_raw, uint16_t *v_out_raw)
{


  uint32_t v_out_mV = Fake_Out_Voltage_mV;
  uint32_t i_out_mA = 0;


  *v_in_raw =  ((Fake_In_Voltage_mV-64.112)/16.746)+1;

  if (pwrctl_vout_enabled())
  {

    if (Fake_Load == 0)
    {//load is open circuit
      i_out_mA = 0;  //No Current and Max Volts
      v_out_mV = Fake_Out_Voltage_mV;
    }
    else
    {
      i_out_mA = Fake_Out_Voltage_mV/Fake_Load;

      if (i_out_mA  > pwrctl_get_ilimit())
      {
        i_out_mA = pwrctl_get_ilimit();
        v_out_mV = i_out_mA * Fake_Load;
      }
    }


    *v_out_raw = ((((double)v_out_mV)-V_ADC_C)/V_ADC_K);
    *i_out_raw= ((((double)i_out_mA)-A_ADC_C)/A_ADC_K);

    if(v_out_mV == 0)
    {
      *v_out_raw = 9;
    }


  }
  else
  {
    *v_out_raw = 9;
    *i_out_raw= 75;
  }


}

/**
  * @brief Initialize TIM4 that drives the backlight of the TFT
  * @retval None
  */
void hw_enable_backlight(void)
{
}

/**
  * @brief Get the ADC valut that triggered the OCP
  * @retval Trivver value in mA
  */
uint16_t hw_get_itrig_ma(void)
{
    return 0;
}

/**
  * @brief Check if it current press is a long press, inject event if so
  * @retval None
  */
void hw_longpress_check(void)
{
}

/**
  * @brief Check if SEL button is pressed
  * @retval true if SEL button is pressed, false otherwise
  */
bool hw_sel_button_pressed(void)
{
    return false;
}

/* 
 * A QT OpenDPS Simulator
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
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

Ui::MainWindow *fui;
QMainWindow *fMainWindow;


extern "C" {

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#ifdef __MINGW32__
  #if !(__has_include(<pthread.h>))
    #error MinGW users. Please install Pthreads-w32 from www.sourceware.org/pthreads-win32/
  #endif
#endif
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#ifdef __MINGW32__
  #include <Winsock2.h>  //MinGW needs to use Winsocks
  #include <ws2tcpip.h>
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
#endif

#include "mock/systick.h"

#include "esp8266_emu.h"
#include "event.h"
#include "opendps_sim.h"
#include "tick.h"
#include "mock/gpio.h"
#include "hw.h"
#include "ili9163c_settings.h"
#include "ili9163c_registers.h"
#include "flash.h"
#include "simulator.h"

}



void MainWindow::On_Click_M1()
{
    event_put(event_button_m1, press_short);
    on_timer_Event();
}

void MainWindow::On_Click_M2()
{
    event_put(event_button_m2, press_short);
    on_timer_Event();
}

void MainWindow::On_Click_Set()
{
    event_put(event_button_sel, press_short);
    on_timer_Event();
}



void MainWindow::On_Click_On_Off()
{
    event_put(event_button_enable, press_short);
    on_timer_Event();
}


void MainWindow::dial_incrementer()
{
  ui->eb_Debug->appendPlainText("up");
  event_put(event_rot_right, press_short);
  on_timer_Event();
}

void MainWindow::dial_decrementer()
{
  ui->eb_Debug->appendPlainText("down");
  event_put(event_rot_left, press_short);
  on_timer_Event();
}


void MainWindow::On_dial_Double_Click()
{
    event_put(event_rot_press, press_short);
    on_timer_Event();
}

void MainWindow::on_Dial_V_In_Changed(int iValue)
{
 qDebug() << int(iValue);
 ui->lb_V_in->setText("V In: "+QString::number(iValue/1000.0));
// simulator_setVoltExt(iValue);
}


void MainWindow::on_Dial_Load(int iValue)
{
 qDebug() << int(iValue) ;
  ui->lb_Load->setText("Load : "+QString::number(iValue));
//  simulator_setLoad(iValue);
}


void MainWindow::SysTick_Timeout()
{
    for (int i = 0; i < 10; ++i) {
    sys_tick_handler();
    }

ui->widget->update();
}


void MainWindow::on_timer_Event(void)
{
    timer_Event->stop();
    sim_event_handler();
    timer_Event->start(1);
}


///////////////////////////////////////////////////////////////////////////////////
void mainwindow_exit()
{
  printf("simulator_exit!\n");
  fMainWindow->close();
}

void mainwindow_processEvents()
{
 QCoreApplication::processEvents();
}


//A0 = 0 Command Mode
//A0 = 1 Dispaly Mode

bool A0_Pin = FALSE;

void mainwindow_gpio_set (uint32_t gpioport, uint8_t gpios)
{
    if ((gpioport == TFT_A0_PORT) && (gpios == TFT_A0_PIN))
    {
        A0_Pin = TRUE;
    }
}

void mainwindow_gpio_clear (uint32_t gpioport, uint8_t gpios)
{
    if ((gpioport == TFT_A0_PORT) && (gpios == TFT_A0_PIN))
    {
        A0_Pin = FALSE;
    }
}

void mainwindow_SaveRam(uint8_t *tx_buf, uint32_t tx_len)
{
    while(tx_len)
    {
        uint16_t color = ((uint16_t)tx_buf[0] << 8) + tx_buf[1];

        uint32_t Red = ((color&0xf800)>>8) | 0x07 ;
        uint32_t Green =  ((color&0x07E0) >> 3) | 0x03;
        uint32_t Blue = ((color & 0x1F) << 3) | 0x07;

        uint32_t RGB = Red << 16;
        RGB |= Green << 8;
        RGB |= Blue << 0;


        if ((fui->widget->Pos_X >= _TFTWIDTH) || (fui->widget->Pos_Y >= _TFTHEIGHT))
        {
        	qDebug() << "SaveRam overflow";
            //fui->eb_Debug->appendPlainText("Over Flow" );
        }
        else
        {
            fui->widget->PixBuff[fui->widget->Pos_X][fui->widget->Pos_Y] = RGB;
        }

        fui->widget->Pos_X++;
        if (fui->widget->Pos_X > fui->widget->Limit_X1)
        {
            fui->widget->Pos_X = fui->widget->Limit_X0;

            fui->widget->Pos_Y++;
            if (fui->widget->Pos_Y > fui->widget->Limit_Y1)
            {//back to start
                fui->widget->Pos_Y = fui->widget->Limit_Y0;
            }

        }
        tx_buf +=2;
        tx_len -=2;
    }
}


bool mainwindow_spi_dma_transceive(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len)
{
    (void) rx_buf;
    (void) rx_len;

    static uint8_t Last_CMD;
    static uint32_t Param_Index;

    int32_t Count = (int32_t)tx_len;


//    while(Count > 0)
    {
        if (A0_Pin)
        {//Display
//            QString sNum;
//            sNum.sprintf("%02X", Last_CMD);
//            fui->eb_Debug->appendPlainText("Last CMD: "+ sNum +   " Tx Len: "  + QString::number(tx_len) + " Rx Len: " + QString::number(rx_len) );


            if (Last_CMD == CMD_CLMADRS)
            {

                if (Param_Index == 0)
                {
                    fui->widget->Limit_X0 = ((uint16_t)tx_buf[0] << 8) + tx_buf[1];
                }
                else if (Param_Index == 1)
                {
                    fui->widget->Limit_X1 = ((uint16_t)tx_buf[0] << 8) + tx_buf[1];
                    //                fui->eb_Debug->appendPlainText("Col Add: "+ QString::number(fui->widget->Limit_X0) + " " + QString::number(fui->widget->Limit_X1) );
                }
                else
                {
                    fui->eb_Debug->appendPlainText("opps");
                }

                Param_Index++;

            }
            if (Last_CMD == CMD_PGEADRS)
            {

                if (Param_Index == 0)
                {
                    fui->widget->Limit_Y0 = ((uint16_t)tx_buf[0] << 8) + tx_buf[1];
                }
                else if (Param_Index == 1)
                {
                    fui->widget->Limit_Y1 = ((uint16_t)tx_buf[0] << 8) + tx_buf[1];
                    //            fui->eb_Debug->appendPlainText("Page Add: "+ QString::number(fui->widget->Limit_Y2) + " " + QString::number(fui->widget->Limit_Y2) );
                }
                else
                {
                    fui->eb_Debug->appendPlainText("opps");
                }
                Param_Index++;
            }

            if (Last_CMD == CMD_RAMWR)
            {
                if (Param_Index == 0)
                {
//                    fui->eb_Debug->appendPlainText("X0: "+ QString::number(fui->widget->Limit_X0) + " X1: " + QString::number(fui->widget->Limit_X1) +" Y0: "+ QString::number(fui->widget->Limit_Y0) + " Y1: " + QString::number(fui->widget->Limit_Y1) + " Count: " + QString::number(tx_len) );
//                    fui->eb_Debug->appendPlainText("RAM: "  + QString::number(tx_buf[0]) + " " + QString::number(tx_buf[1]) + " " + QString::number(tx_buf[2]) + " " + " " + QString::number(tx_buf[3]) + " " );

                    fui->widget->Pos_X =  fui->widget->Limit_X0;
                    fui->widget->Pos_Y =  fui->widget->Limit_Y0;

                    mainwindow_SaveRam(tx_buf,tx_len);
                }
                else
                {
//                    fui->eb_Debug->appendPlainText("X: "+ QString::number(fui->widget->Pos_X) +" Y: "+ QString::number(fui->widget->Pos_Y) + " Count: " + QString::number(tx_len) );
                	mainwindow_SaveRam(tx_buf,tx_len);
                }

                Param_Index ++;
                //fui->eb_Debug->appendPlainText("Param_Index: "+ QString::number(Param_Index) );

            }

            Count -= 2;
        }
        else
        {//Command
            Last_CMD = *tx_buf;

            if (Last_CMD == CMD_CLMADRS)
            {
                Param_Index = 0;
            }
            if (Last_CMD == CMD_PGEADRS)
            {
                Param_Index = 0;
            }
            if (Last_CMD == CMD_RAMWR)
            {

                Param_Index = 0;
            }
            Count--;
        }
    }
    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Emulator init
 *
 * @param[in]  past       pointer to the past
 * @param[in]  argc argv  program argument
 */

void dps_emul_init(past_t *past, int argc, char const *argv[])
{
  printf("OpenDPS Simulator\n");

    size_t optind;
    char *file_name = 0;
    bool write_past = false;
    for (optind = 1; optind < (unsigned)argc; optind++) {
        switch (argv[optind][1]) {
          case 'p':
            file_name = (char*) argv[optind+1];
            optind++;
            break;
          case 'w':
          write_past = true;
            break;
          default:
              fprintf(stderr, "Usage: %s [-p past.bin] [-w]\n", argv[0]);
              exit(EXIT_FAILURE);
        }
    }

  flash_emul_init(past, file_name, write_past);
}


///////////////////////////////////////////////////////////////////////////////////


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fui = ui;
    fMainWindow = this;

    Init_ESP8266_emu();

#ifdef __MINGW32__
  setbuf(stdout, NULL); //Fix for No Console Output in Eclipse with CDT in Windows
#endif

  	connect(ui->dial_V_In, SIGNAL (valueChanged(int)), this, SLOT (on_Dial_V_In_Changed(int)));
  	on_Dial_V_In_Changed(ui->dial_V_In->value());

  	connect(ui->dial_Load, SIGNAL (valueChanged(int)), this, SLOT (on_Dial_Load(int)));
  	on_Dial_Load(ui->dial_Load->value());

  	connect(ui->bt_M1, SIGNAL (released()), this, SLOT (On_Click_M1()));
  	connect(ui->bt_M2, SIGNAL (released()), this, SLOT (On_Click_M2()));
  	connect(ui->bt_Set, SIGNAL (released()), this, SLOT (On_Click_Set()));
  	connect(ui->bt_On_Off, SIGNAL (released()), this, SLOT (On_Click_On_Off()));

  	connect(ui->dial, SIGNAL(Increment()), this,SLOT(dial_incrementer()));
  	connect(ui->dial, SIGNAL(Decrement()), this,SLOT(dial_decrementer()));
  	connect(ui->dial, SIGNAL (Double_Click()), this, SLOT (On_dial_Double_Click()));


  	//Timer
    timer_SysTick = new QTimer(this);
    connect(timer_SysTick, SIGNAL(timeout()), this, SLOT(SysTick_Timeout()));
    timer_SysTick->start(10);

    timer_Event = new QTimer(this);
    connect(timer_Event, SIGNAL(timeout()), this, SLOT(on_timer_Event()));



  	//rebuilds the command line args to pass onto the simulator main
    const QStringList args = QCoreApplication::arguments();
    int m_argc = args.count();
    char **m_argv = new char*[m_argc];  //creates a new array of pointers

    for(int i =0 ;i < m_argc;i++)
    {
      QString sTemp = args.at(i);
      char * ptr = new char[sTemp.length() + 1];
      strncpy (ptr,sTemp.toStdString().c_str(),sTemp.length());
      ptr[sTemp.length()]=0;
      m_argv[i]=(char*)ptr;
//      printf("my_argv[%d] = %s\n",i,m_argv[i]);
    }

    sim_main(m_argc,(const char **)m_argv);
    ui->widget->update();


    timer_Event->start(1);

}

MainWindow::~MainWindow()
{
	  for(int i =0 ;i < m_argc;i++)
	  {
	    delete m_argv[i];
	  }

	  delete m_argv;
	  delete ui;
}























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

}



void MainWindow::On_Click_M1()
{
//    event_put(event_button_m1, press_short);
//    on_timer_Event();
}

void MainWindow::On_Click_M2()
{
//    event_put(event_button_m2, press_short);
//    on_timer_Event();
}

void MainWindow::On_Click_Set()
{
//    event_put(event_button_sel, press_short);
//    on_timer_Event();
}



void MainWindow::On_Click_On_Off()
{
//    event_put(event_button_enable, press_short);
//    on_timer_Event();
}


void MainWindow::dial_incrementer()
{
  ui->eb_Debug->appendPlainText("up");
//  event_put(event_rot_right, press_short);
//  on_timer_Event();
}

void MainWindow::dial_decrementer()
{
  ui->eb_Debug->appendPlainText("down");
//  event_put(event_rot_left, press_short);
//  on_timer_Event();
}


void MainWindow::On_dial_Double_Click()
{
//    event_put(event_rot_press, press_short);
//    on_timer_Event();
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


bool event_put(event_t event, uint8_t data)//todo remove place holder
{
	return true;
}


void mainwindow_processEvents()
{
 QCoreApplication::processEvents();
}



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























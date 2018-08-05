#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T20:27:23
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dpssimulator
TEMPLATE = app

INCLUDEPATH += mock \
				../opendps/ \
				../scpi-parser/libscpi/inc/

QMAKE_CC += -std=c99

DEFINES += "CONFIG_DPS_MAX_CURRENT=5000"
DEFINES += "dbg_printf=printf"
DEFINES += "DPS5005"
DEFINES += "DPS_EMULATOR"
DEFINES += "HAVE_STDBOOL"
DEFINES += "HAVE_STRUCT_TIMESPEC"
DEFINES += COLORSPACE=1

#todo remove these later
DEFINES += "CONFIG_DEFAULT_VOUT=5000"
DEFINES += "CONFIG_DEFAULT_ILIMIT=500"

#INCLUDEPATH += third-party\scpi-parser\libscpi\inc

#LIBS += -L"C:\Users\aaron.keith\Documents\QT\ili9163c_LCD\third-party\scpi-parser\libscpi\dist" -lscpi


LIBS += -lOpengl32 -lpthreadGC2 -lWs2_32 
#LIBS +=  ../scpi-parser/libscpi/dist/libscpi.a

SOURCES += main.cpp\
    mainwindow.cpp \
    dialplus.cpp \
    ili9163c_widget.cpp \
    esp8266_emu.c \
    opendps_sim.c \
    tick.c \
    mock/systick_mock.c \
    ../opendps/tft.c \
    ../opendps/ili9163c.c \
    mock/gpio_mock.c \
    ../opendps/font-0.c \
    ../opendps/font-1.c \   
    #
    #opendps_sim.c \
    #
    #spi_driver.c \
    #
    #

    #../opendps/event.c \
    #ringbuf.c \
    #pwrctl.c \
    #mock/dac_mock.c \
    #../opendps/past.c \
    #
    #hw.c \
    #../opendps/protocol_handler.c \
    #../opendps/protocol.c \
    #../opendps/crc16.c \
    #../opendps/bootcom.c \
   	#../opendps/uui.c \
	#../opendps/uui_number.c \
	#../opendps/func_cv.c \
	#../opendps/uframe.c \
	#../opendps/scpi_simu.c \
	#../opendps/scpi_sour.c \
	#../opendps/scpi_meas.c \
	#../opendps/scpi_params.c \
	#flash.c \ 
	
	

HEADERS  += mainwindow.h \
    dialplus.h \
    ili9163c_widget.h \
    esp8266_emu.h \
    ../opendps/event.h \
    opendps_sim.h \
    mock/systick.h \
    mock/rcc.h \
    mock/timer.h \
    mock/adc.h \
    mock/exti.h \
    mock/usart.h \
    mock/dac.h \
    
    
    #../opendps/ili9163c.h \
    #../opendps/ili9163c_registers.h \
    #ili9163c_settings.h \
    #mock/gpio.h \
    #../opendps/spi_driver.h \
    #../opendps/hw.h \
    #../opendps/tick.h \
    #../opendps/tft.h \
    #../opendps/font-0.h \
    #../opendps/font-1.h \
    #
    #../opendps/logo.h \
    #../opendps/padlock.h \
    #../opendps/power.h \
    #../opendps/wifi.h \
   
    #ringbuf.h \
    #
    #pwrctl.h \
    #../opendps/past.h \
    #mock/nvic.h \
    #../opendps/serialhandler.h \
    #
    #
    #
    #
    #
    #../opendps/dbg_printf.h \
    #../opendps/protocol.h \
   	#../opendps/crc16.c \
   	#../opendps/bootcom.h \
   	#../opendps/func_cv.h \
   	#../opendps/uframe.h \
   	#../opendps/simulator.h \
   	#flash.h \
	
	
    

FORMS    += mainwindow.ui

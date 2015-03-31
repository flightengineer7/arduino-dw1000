/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 *                       Brian Plank <bdplank@gmail.com>
 * Decawave DW1000 library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _DW1000_H_INCLUDED
#define _DW1000_H_INCLUDED

// enum to determine RX or TX mode of device
#define IDLE_MODE 0x00
#define RX_MODE 0x01
#define TX_MODE 0x02

// used for SPI ready w/o actual writes
#define JUNK 0x00

// no sub-address for register write
#define NO_SUB 0x00
#define SUB_2  0x02
#define SUB_4  0x04
#define SUB_6  0x06
#define SUB_7  0x07
#define SUB_8  0x08
#define SUB_B  0x0B
#define SUB_C  0x0C
#define SUB_26 0x26
#define SUB_1806 0x1806
#define SUB_2804 0x2804

// device id register
#define DEV_ID 0x00
#define LEN_DEV_ID 4

// device configuration register
#define SYS_CFG 0x04
#define LEN_SYS_CFG 4
#define FFEN_BIT 0
#define DIS_DRXB_BIT 12
#define PHR_MODE_LSB 16
#define PHR_MODE_MSB 17
#define RXAUTR_BIT 29

// device control register
#define SYS_CTRL 0x0D
#define LEN_SYS_CTRL 4
#define SFCST_BIT 0
#define TXSTRT_BIT 1
#define TXDLYS_BIT 2
#define TRXOFF_BIT 6
#define WAIT4RESP_BIT 7
#define RXENAB_BIT 8
#define RXDLYS_BIT 9
#define FS_CTRL 0x2B

// system event status register
#define SYS_STATUS 0x0F
#define LEN_SYS_STATUS 5
#define TXFRS_BIT 7
#define LDEDONE_BIT 10
#define RXDFR_BIT 13
#define RXFCG_BIT 14
#define RXFCE_BIT 15
#define RXRFSL_BIT 16
#define LDEERR_BIT 18

// RX timestamp register
#define RX_TIME 0x15
#define LEN_RX_TIME 14
#define RX_STAMP_SUB 0x00
#define LEN_RX_STAMP_SUB 5

// timing register (for delayed RX/TX)
#define DX_TIME 0x0A
#define LEN_DX_TIME 5

// transmit data buffer
#define TX_BUFFER 0x09
#define LEN_TX_BUFFER 1024
#define LEN_UWB_FRAMES 127
#define LEN_EXT_UWB_FRAMES 1023

// transmit control
#define TX_FCTRL 0x08
#define LEN_TX_FCTRL 5
#define TX_CAL 0x2A

// receive control register
#define DRX_TUNE 27
#define RF_CONF 28
#define LDE_IF 0x2E

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DEBUG
#include <Arduino.h>
#include "../SPI/SPI.h"
#else
#include <stdint.h>
#define boolean uint8_t
#define byte uint8_t
#define word uint16_t
#define word uint32_t
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#endif

class DW1000 {
public:
	/* TODO impl: later
	 * - TXBOFFS in TX_FCTRL for offset buffer transmit
 	 * - TR in TX_FCTRL for flagging for ranging messages
	 * - CANSFCS in SYS_CTRL to cancel frame check suppression
	 * - HSRBP in SYS_CTRL to determine in double buffered mode from which buffer to read
	 */

	// construction with chip select pin number
	DW1000(int ss);
	~DW1000();

	int getChipSelect();
	
	// Default Chip Setup Options
	void setDefaultMode(short MODE);

	// DEV_ID, device identifier
	char* readDeviceIdentifier();
	
	// SYS_CFG, general device configuration
	byte* getSystemConfiguration();
	void loadSystemConfiguration();
	void readSystemConfiguration(byte syscfg[]);
	void setFrameFilter(boolean val);
	void setDoubleBuffering(boolean val); // NOTE should be set to false
	void setReceiverAutoReenable(boolean val);

	// SYS_CTRL, TX_FCTRL, transmit and receive configuration
	void suppressFrameCheck();
	void delayedTransceive(unsigned int delayNanos); // TODO impl
	void transmitRate(byte rate);
	void pulseFrequency(byte freq);
	void preambleLength(byte prealen);
	void transmitFrameLength(word dataLength);
	void tuneReceiver(byte rate, byte PRF, byte preamble, byte pac);
	void setRFChannel(short channel);
	void waitForResponse(boolean val);
	void setData(byte data[], int n);
	int getData(byte data[]);

	// RX/TX default settings
	void setDefaults();
	void setNASA_RMC_2015();

	// SYS_STATUS, device status flags
	boolean isLDEDone();
	boolean isTransmitDone();
	boolean isReceiveDone();
	boolean isReceiveSuccess();

	void clearReceiveStatus();
	void clearTransmitStatus(); // TODO impl

	// RX_TIME, ..., timing, timestamps, etc.
	// TODO void readReceiveTimestamp(byte[] timestamp);

	// idle
	void idle();

	// reception
	void newReceive();
	void startReceive();
	void cancelReceive();

	// transmission
	void newTransmit();	// ADD IFSDELAY
	void startTransmit();
	void cancelTransmit();

	// reception channel
	static const long RX_CHANNEL_1 = 0x
	static const long RX_CHANNEL_2 = 0x
	static const long RX_CHANNEL_3 = 0x
	static const long RX_CHANNEL_4 = 0x
	static const long RX_CHANNEL_5 = 0x
	static const long RX_CHANNEL_7 = 0x
	
	// transmission channel
	static const long TX_CHANNEL_1 = 0x00005C40;
	static const long TX_CHANNEL_2 = 0x00045CA0;
	static const long TX_CHANNEL_3 = 0x00086CC0;
	static const long TX_CHANNEL_4 = 0x00045C80;
	static const long TX_CHANNEL_5 = 0x001E3FE0;
	static const long TX_CHANNEL_7 = 0x001E7DE0;
	
	// reception bit rate
	static const byte RX_RATE_110KBPS = 0x0A;
	static const byte RX_RATE_850KBPS = 0x01;		// Error in dw1000_user_manual_2_02.pdf ??
	static const byte RX_RATE_6800KBPS = 0x01;		// Error in dw1000_user_manual_2_02.pdf ??
	
	// transmission bit rate
	static const byte TX_RATE_110KBPS = 0x00;
	static const byte TX_RATE_850KBPS = 0x01;
	static const byte TX_RATE_6800KBPS = 0x02;
	
	// reception pulse frequency
	static const byte RX_PULSE_FREQ_16MHz = 0x87;
	static const byte RX_PULSE_FREQ_64MHz = 0x8D;

	// transmission pulse frequency
	// 0x00 is 4MHZ, but receiver in DW1000 does not support it (!??)
	static const byte TX_PULSE_FREQ_16MHZ = 0x01;
	static const byte TX_PULSE_FREQ_64MHZ = 0x02;

	// preamble lengths (PE + TXPSR bits)
	static const byte TX_PREAMBLE_LEN_64 = 0x01;
	static const byte TX_PREAMBLE_LEN_128 = 0x05;
	static const byte TX_PREAMBLE_LEN_256 = 0x09;
	static const byte TX_PREAMBLE_LEN_512 = 0x0D;
	static const byte TX_PREAMBLE_LEN_1024 = 0x02;
	static const byte TX_PREAMBLE_LEN_1536 = 0x06;
	static const byte TX_PREAMBLE_LEN_2048 = 0x0A;
	static const byte TX_PREAMBLE_LEN_4096 = 0x03;
	
	// transmit power control - smart
	static const long SMART_TX_CH_1_PRF_16MHz = 0x15355575;
	static const long SMART_TX_CH_2_PRF_16MHz = 0x15355575;
	static const long SMART_TX_CH_3_PRF_16MHz = 0x0F2F4F6F;	
	static const long SMART_TX_CH_4_PRF_16MHz = 0x1F1F3F5F;
	static const long SMART_TX_CH_5_PRF_16MHz = 0x0E082848;
	static const long SMART_TX_CH_7_PRF_16MHz = 0x32527292;
	static const long SMART_TX_CH_1_PRF_64MHz = 0x07274767;
	static const long SMART_TX_CH_2_PRF_64MHz = 0x07274767;
	static const long SMART_TX_CH_3_PRF_64MHz = 0x2B4B6B8B;
	static const long SMART_TX_CH_4_PRF_64MHz = 0x3A5A7A9A;
	static const long SMART_TX_CH_5_PRF_64MHz = 0x25456585;
	static const long SMART_TX_CH_7_PRF_64MHz = 0x5171B1D1;
	
	// transmit power control - manual
	static const long MANUAL_TX_CH_1_PRF_16MHz = 0x75757575;
	static const long MANUAL_TX_CH_2_PRF_16MHz = 0x75757575;
	static const long MANUAL_TX_CH_3_PRF_16MHz = 0x6F6F6F6F;
	static const long MANUAL_TX_CH_4_PRF_16MHz = 0x5F5F5F5F;
	static const long MANUAL_TX_CH_5_PRF_16MHz = 0x48484848;
	static const long MANUAL_TX_CH_7_PRF_16MHz = 0x92929292;
	static const long MANUAL_TX_CH_1_PRF_64MHz = 0x67676767;
	static const long MANUAL_TX_CH_2_PRF_64MHz = 0x67676767;
	static const long MANUAL_TX_CH_3_PRF_64MHz = 0x8B8B8B8B;
	static const long MANUAL_TX_CH_4_PRF_64MHz = 0x9A9A9A9A;
	static const long MANUAL_TX_CH_5_PRF_64MHz = 0x85858585;
	static const long MANUAL_TX_CH_7_PRF_64MHz = 0xD1D1D1D1;

	// frequency synthesizer PLL configuration
	static const long PLL_CONFIG_CH_1 = 0x09000407;
	static const long PLL_CONFIG_CH_2 = 0x08400508;
	static const long PLL_CONFIG_CH_3 = 0x08401009;
	static const long PLL_CONFIG_CH_4 = 0x08400508;
	static const long PLL_CONFIG_CH_5 = 0x0800041D;
	static const long PLL_CONFIG_CH_7 = 0x0800041D;
	
	// frequency synthesizer PLL tuning
	static const byte PLL_TUNE_CH_1 = 0x1E;
	static const byte PLL_TUNE_CH_2 = 0x26;
	static const byte PLL_TUNE_CH_3 = 0x5E;
	static const byte PLL_TUNE_CH_4 = 0x26;
	static const byte PLL_TUNE_CH_5 = 0xA6;
	static const byte PLL_TUNE_CH_7 = 0xA6;
	
	// start frame delimiter selection
	static const byte SFD_STD_RATE_110KBPS   = 0x0A;
	static const byte SFD_NSTD_RATE_110KBPS  = 0x16;
	static const byte SFD_STD_RATE_850KBPS   = 0x01;		// Error in dw1000_user_manual_2_02.pdf ??
	static const byte SFD_NSTD_RATE_850KBPS  = 0x06;
	static const byte SFD_STD_RATE_6800KBPS  = 0x01;		// Error in dw1000_user_manual_2_02.pdf ??
	static const byte SFD_NSTD_RATE_6800KBPS = 0x02;
	
	// receiver configuration
	static const byte DRX_TUNE_RATE_110KBPS = 0x64;
	static const byte DRX_TUNE_RATE_850_6800KBPS = 0x20;
	static const byte DRX_TUNE_RATE_6800KBPS = 0x10;
	
	static const byte DRX_TUNE4H_PREAMBLE_SHORT = 0x10;
	static const byte DRX_TUNE4H_PREAMBLE_LONG  = 0x28;
	
	// receive PAC size selection
	static const long PAC_8_PRF_16MHz  = 0x311A002D;
	static const long PAC_8_PRF_64MHz  = 0x313B006B;
	static const long PAC_16_PRF_16MHz = 0x331A0052;
	static const long PAC_16_PRF_64MHz = 0x333B00BE;
	static const long PAC_32_PRF_16MHz = 0x351A009A;
	static const long PAC_32_PRF_64MHz = 0x353B015E;
	static const long PAC_64_PRF_16MHz = 0x371A011D;
	static const long PAC_64_PRF_64MHz = 0x373B0296;
	
	// analog RF control
	static const byte RX_ANALOG_STD  = 0xD8;
	static const byte RX_ANALOG_NSTD = 0xBC;
	
	// automatic gain control configuration
	static const word RX_AGC_TUNE_PRF_16MHz = 0x8870;
	static const word RX_AGC_TUNE_PRF_64MHz = 0x889B;
	
	// leading edge detection interface configuration
	static const word LDE_PRF_16MHz = 0x1607;
	static const word LDE_PRF_64MHz = 0x0607;
	
	// leading edge detection interface replica coefficient
	static const word LDE_REPC_RX_PCODE_1  = 0x5998;
	static const word LDE_REPC_RX_PCODE_2  = 0x5998;
	static const word LDE_REPC_RX_PCODE_3  = 0x51EA;
	static const word LDE_REPC_RX_PCODE_4  = 0x428E;
	static const word LDE_REPC_RX_PCODE_5  = 0x451E;
	static const word LDE_REPC_RX_PCODE_6  = 0x2E14;
	static const word LDE_REPC_RX_PCODE_7  = 0x8000;
	static const word LDE_REPC_RX_PCODE_8  = 0x51EA;
	static const word LDE_REPC_RX_PCODE_9  = 0x28F4;
	static const word LDE_REPC_RX_PCODE_10 = 0x3332;
	static const word LDE_REPC_RX_PCODE_11 = 0x3AE0;
	static const word LDE_REPC_RX_PCODE_12 = 0x3D70;
	static const word LDE_REPC_RX_PCODE_13 = 0x3AE0;
	static const word LDE_REPC_RX_PCODE_14 = 0x35C2;
	static const word LDE_REPC_RX_PCODE_15 = 0x2B84;
	static const word LDE_REPC_RX_PCODE_16 = 0x35C2;
	static const word LDE_REPC_RX_PCODE_17 = 0x3332;
	static const word LDE_REPC_RX_PCODE_18 = 0x35C2;
	static const word LDE_REPC_RX_PCODE_19 = 0x35C2;
	static const word LDE_REPC_RX_PCODE_20 = 0x47AE;
	static const word LDE_REPC_RX_PCODE_21 = 0x3AE0;
	static const word LDE_REPC_RX_PCODE_22 = 0x3850;
	static const word LDE_REPC_RX_PCODE_23 = 0x30A2;
	static const word LDE_REPC_RX_PCODE_24 = 0x3850;
	// ?TODO? implement selection procedure based on channel
	// TODO implement writing to LDE_IF, SUB_2804 register
	
	// transmitter pulse generator delay
	static const byte PGD_CH_1 = 0xC9;
	static const byte PGD_CH_2 = 0xC2;
	static const byte PGD_CH_3 = 0xC5;
	static const byte PGD_CH_4 = 0x95;
	static const byte PGD_CH_5 = 0xC0;
	static const byte PGD_CH_7 = 0x93;
	
#ifdef DEBUG
	byte debugBuffer[1024];
	inline void clearDebugBuffer() {
		memset(debugBuffer, 0, 1024);
	}
#endif

private:
	unsigned int _ss;

	byte _syscfg[LEN_SYS_CFG];
	byte _sysctrl[LEN_SYS_CTRL];

	boolean _frameCheckSuppressed;
	boolean _extendedFrameLength;

	byte _txfctrl[LEN_TX_FCTRL];

	// whether RX or TX is active
	int _deviceMode; 

	void readBytes(byte cmd, byte data[], int n);
	void writeBytes(byte cmd, word offset, byte data[], int n);

	boolean getBit(byte data[], int n, int bit);
	void setBit(byte data[], int n, int bit, boolean val);
	
	/* Register is 6 bit, 7 = write, 6 = sub-adressing, 5-0 = register value
	 * Total header with sub-adressing can be 15 bit
	 */
	static const byte WRITE = 0x80; // regular write
	static const byte WRITE_SUB = 0xC0; // write with sub address
	static const byte READ = 0x00; // regular read
};

#endif

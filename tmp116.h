/*
 * tmp116.h
 *
 *  Created on: May 11, 2020
 *      Author: Fabian Niehaus
 *		E-Mail: fabian.niehaus@tuhh.de
 *
 */

//Change to HAL-library of your device
#warning "Change to HAL-library of your device"
#include "stm32l4xx_hal.h"

/**
 * DEVICE ADDRESS ADD0 PIN CONNECTION
 *
 * 1001000x 	ADD0_GND		=	(0x48 << 1)
 * 1001001x 	ADD0_V			=	(0x49 << 1)
 * 1001010x 	ADD0_SDA		=	(0x4A << 1)
 * 1001011x 	ADD0_SCL		=	(0x4B << 1)
 */
#define 	ADD0_GND			//DEFAULT
//#define	ADD0_V
//#define	ADD0_SDA
//#define	ADD0_SCL

/*
 * Table 3. Register Map
 *
 * ADDRESS	|	TYPE  	| 	RESET	| 	ACRONYM			| 		REGISTER NAME
 * ---------------------------------------------------------------------------------
 * 	00h 	|	R 	 	|	8000h 	|	TEMP 			|	Temperature register
 * 	01h		|	R/W	 	|	0220h	| 	(1) CFGR		| 	Configuration register
 * 	02h		|	R/W		| 	6000h	| 	(1) HIGH_LIM 	|	High limit register
 * 	03h		|	R/W 	|	8000h	| 	(1) LOW_LIM 	|	Low limit register
 *  04h 	|	R/W		| 	0000h	|	EEPROM_UL		| 	EEPROM unlock register
 * 	05h 	|	R/W		|	0000h	|	(1) EEPROM1 	|	EEPROM1 register
 * 	06h 	|	R/W 	|	0000h	|	(1) EEPROM2 	|	EEPROM2 register
 * 	07h 	|	R/W 	|	0000h	|	(1) EEPROM3 	|	EEPROM3 register
 * 	08h 	|	R/W 	|	0000h	|	(1) EEPROM4 	|	EEPROM4 register
 * 	0Fh 	|	R 		|	1116h 	|	DEVICE_ID Device| 	ID register
 *
 */
#define     TMP116_TemperatureRegister     	0x00
#define     TMP116_ConfigurationRegister   	0x01
#define     TMP116_TemperatureHighLimit    	0x02
#define     TMP116_TemperatureLowLimit     	0x03
#define     TMP116_EEPROM_Unlock           	0x04
#define     TMP116_EEPROM1                 	0x05
#define     TMP116_EEPROM2                 	0x06
#define     TMP116_EEPROM3                 	0x07
#define     TMP116_EEPROM4                 	0x08
#define     TMP116_ID_Register             	0x0F

#define 	CC_MODE							0x00
#define 	SD_MODE							0x01
#define		CC2_MODE						0x02
#define		OS_MODE							0x03

#define 	AVG_1							0x00
#define 	AVG_8							0x01
#define 	AVG_32							0x02
#define 	AVG_64							0x03

#define 	CONV_0							0x00
#define 	CONV_1							0x01
#define		CONV_2							0x02
#define 	CONV_3							0x03
#define 	CONV_4							0x04
#define 	CONV_5							0x05
#define 	CONV_6							0x06
#define 	CONV_7							0x07

#define		POL_AH							0x01
#define		POL_AL							0x00

#define		THERM_MODE						0x01
#define		ALERT_MODE						0x00

#define		DATA_R							0x01
#define		AF								0x00

typedef enum{
	TMP116_OK 	= 0,
	TMP116_ERR	,
	TMP116_TX_ERR,
	TMP116_RX_ERR,
	TMP116_INIT_ERR
}temp_err;


/*
 * Table 6. Configuration Register Field Descriptions
 */
typedef union{
	struct{
		uint8_t HIGH_ALERT:1;
		uint8_t LOW_ALERT:1;
		uint8_t DATA_READY:1;
		uint8_t EEPROM_BUSY:1;
		uint8_t MOD:2;
		uint8_t CONV:3;
		uint8_t AVG:2;
		uint8_t T_A_mode:1;
		uint8_t POL:1;
		uint8_t	DR_ALERT:1;
		uint8_t NOT_USED1;
	}bits;
	struct{
		uint8_t MSB;
		uint8_t	LSB;
	}byte;
	uint16_t REG;
}tmp116_config_t;

/* Exported functions ------------------------------------------------------- */
extern temp_err TEMP116_init(I2C_HandleTypeDef *hi2c,tmp116_config_t *TMP116_Conf);
extern temp_err TMP116_set_Configuration(tmp116_config_t *tempConfig);
extern temp_err TMP116_get_Configuration(tmp116_config_t *tempConfig);
extern temp_err TMP116_LockEEPROM(void);
extern temp_err TMP116_UnlockEEPROM(void);
extern temp_err TMP116_set_HighLimit(int16_t val);
extern temp_err TMP116_get_HighLimit(int16_t *high_limit);
extern temp_err TMP116_set_LowLimit(int16_t val);
extern temp_err TMP116_get_LowLimit(int16_t *low_limit);
extern temp_err TMP116_getUID(uint16_t *uid);
extern temp_err TMP116_get_Temperature(float *tempVal);


# TMP116
STM32 driver for  TI TMP116 ±0.2°C Accurate Digital Temperature Sensor With NV Memory

# Error typedef
```c
typedef enum{
	TMP116_OK 	= 0,
	TMP116_ERR	,
	TMP116_TX_ERR,
	TMP116_RX_ERR,
	TMP116_INIT_ERR
}temp_err;
```

# Config typedef
```c
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
```

# Functions

```c
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
```
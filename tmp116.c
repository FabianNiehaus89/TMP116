/*
 * tmp116.c
 *
 *  Created on: May 11, 2020
 *      Author: Fabian Niehaus
 *		E-Mail: fabian.niehaus@tuhh.de
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "ti_temp_sensor.h"



/* Private variables --------------------------------------------------------*/
I2C_HandleTypeDef	*TMP_BUS_HANDLER 	= 0;
uint16_t 							uid	= 0;


#ifdef ADD0_GND
uint8_t TMP116_DeviceID 	= (0x48 << 1);
#endif
#ifdef ADD0_V
uint8_t TMP116_DeviceID 	= (0x49 << 1);
#endif
#ifdef ADD0_SDA
uint8_t TMP116_DeviceID 	= (0x4A << 1);
#endif
#ifdef ADD0_SCL
uint8_t TMP116_DeviceID 	= (0x4B << 1);
#endif


/* Functions ------------------------------------------------------------------*/
temp_err TEMP116_init(I2C_HandleTypeDef *hi2c,tmp116_config_t *TMP116_Conf){

	TMP_BUS_HANDLER = hi2c;

	TMP116_getUID(&uid);

	if((uid&0x0fff) == 0x0116){

		TMP116_set_Configuration(TMP116_Conf);

		return TMP116_OK;

	}else{
		return TMP116_ERR;
	}
}

/*
 * temp_err TMP116_getUID(uint16_t *uid)
 *
 * @brief Get the sensor unique id
 *
 * @param 	uint16_t*	- pointer to uid variable
 * @return 	temp_err
 */
temp_err TMP116_getUID(uint16_t *uid){
	*uid = 0;
	if(TMP_BUS_HANDLER){
	      static uint8_t buf[2];
	      buf[0]=TMP116_ID_Register;

	      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
	    	  return TMP116_TX_ERR;
	      }
	      buf[0] = 0;
	      HAL_Delay(20);
	      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
	    	  return TMP116_RX_ERR;
	      }

	      *uid = ((buf[0]<<8)|buf[1]);
	      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
	return TMP116_OK;
}

/*
   @Brief         Set configuration
   @Description   Set Configuration Register for Features of the TMP116
   @Parameter     tmp116_config_t*
   @Return value  temp_err
 */
temp_err TMP116_set_Configuration(tmp116_config_t *tempConfig)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=0x01;
      buf[1]=tempConfig->byte.LSB;
      buf[2]=tempConfig->byte.MSB;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }

      return TMP116_OK;

	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Get configuration
   @Description   Get configuration Register of the TMP116
   @Parameter     tmp116_config_t *

   @Return value  temp_err
 */
temp_err TMP116_get_Configuration(tmp116_config_t *tempConfig)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[2];
      buf[0]=0x01;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }
      buf[0] = 0;
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
    	  return TMP116_RX_ERR;
      }

      tempConfig->REG = ((buf[0]<<8)|buf[1]);
      return TMP116_OK;
	}else{
			return TMP116_INIT_ERR;
	}
}


/*
 @Brief         Get temperature basically
 @Description   Function returns ambient temperature
 @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
 @Return value  Float
*/

temp_err TMP116_get_Temperature(float *tempVal)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_TemperatureRegister;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
    	  return TMP116_RX_ERR;
      }
      HAL_Delay(20);

      *tempVal = ((((buf[0]<<8)|buf[1]))*0.0078125);

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Set HighLimit
   @Description   Set HighLimit for Alert
   @Parameter     int val - temperature Value
   @Return value  temp_err
 */
temp_err TMP116_set_HighLimit(int16_t val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      uint16_t limit_reg = (val/0.0078125);
      buf[0]=TMP116_TemperatureHighLimit;
      buf[1]=( limit_reg >> 8 );
      buf[2]=( limit_reg >> 0 );

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Get Highlimit
   @Description   Get Highlimit Register Value
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  temp_err
 */
temp_err TMP116_get_HighLimit(int16_t *high_limit)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_TemperatureHighLimit;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
    	  return TMP116_RX_ERR;
      }
      HAL_Delay(20);
      *high_limit = ((buf[0]<<8)|buf[1]);

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}

}

/*
   @Brief         Set LowLimit
   @Description   Set LowLimit for Alert
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  temp_err
 */
temp_err TMP116_set_LowLimit(int16_t val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];

      uint16_t limit_reg = (val/0.0078125);
      buf[0]=TMP116_TemperatureHighLimit;
      buf[1]=( limit_reg >> 8 );
      buf[2]=( limit_reg >> 0 );

      buf[0]=TMP116_TemperatureLowLimit;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Get LowLimit
   @Description   Get Lowlimit Register Value
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  temp_err
 */
temp_err TMP116_get_LowLimit(int16_t *low_limit)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_TemperatureLowLimit;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_TX_ERR;
      }
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
    	  return TMP116_RX_ERR;
      }

      HAL_Delay(20);
      *low_limit = ((buf[0]<<8)|buf[1]);

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Get EEPROM Unlock Register Value
   @Description   Check EEPROM for Unlock/Lock
   @Parameter     uint16_t *unlockVal pointer for the unlock stat return value
   @Return value  temp_err
 */
temp_err TMP116_get_Unlock_Reg(uint16_t *unlockStatVal)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM_Unlock;

      HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100);
      HAL_Delay(20);
      HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100);
      *unlockStatVal =  ((buf[0]<<8)|buf[1]);
      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Set EEPROM Unlock Register Value
   @Description   Active/Inactive for EEPROM read/write situation
   @Parameter     bool = true to unlock EEPROM
   @Return value  temp_err
 */
temp_err TMP116_UnlockEEPROM()
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM_Unlock;
      buf[1]=0x80;
      buf[2]=0x00;

      HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100);
      HAL_Delay(20);
      return TMP116_OK;
	}else{

		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Set EEPROM Unlock Register Value
   @Description   Active/Inactive for EEPROM read/write situation
   @Parameter     bool = true to unlock EEPROM
   @Return value  temp_err
 */
temp_err TMP116_LockEEPROM()
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM_Unlock;
      buf[1]=0x00;
      buf[2]=0x00;

      HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100);
      HAL_Delay(20);
      return TMP116_OK;
	}else{

		return TMP116_INIT_ERR;
	}
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  temp_err
 */
temp_err TMP116_set_EEPROM1(uint16_t eeprom1Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM1;
      buf[1]=( eeprom1Val >> 8 );     //Value MSB
      buf[2]=( eeprom1Val >> 0 );     //Value LSB

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);
      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  temp_err
 */
temp_err TMP116_get_EEPROM1           (uint16_t *eeprom1Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM1;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
    	  return TMP116_ERR;
      }

      *eeprom1Val = ((buf[0]<<8)|buf[1]);

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  temp_err
 */
temp_err TMP116_set_EEPROM2(uint16_t eeprom2Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM2;
      buf[1]=( eeprom2Val >> 8 );
      buf[2]=( eeprom2Val >> 0 );

      HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100);
      HAL_Delay(20);
      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  temp_err
 */
temp_err TMP116_get_EEPROM2(uint16_t *eeprom2Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM2;

      HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100);

      *eeprom2Val =  ((buf[0]<<8)|buf[1]);

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  temp_err
 */
temp_err TMP116_set_EEPROM3(uint16_t eeprom3Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM3;
      buf[1]=( eeprom3Val >> 8 );
      buf[2]=( eeprom3Val >> 0 );

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);
      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Get EEPROM3 Value
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  temp_err
 */
temp_err TMP116_get_EEPROM3(uint16_t *eeprom3Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM3;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,0,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,1,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);
      *eeprom3Val = ((buf[0]<<8)|buf[1]);
      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

/*
   @Brief         Set Temperature Offset Value
   @Description   Set Temperature Offset Value for Calibrating
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  temp_err
 */
temp_err TMP116_set_EEPROM4       (uint16_t eeprom4Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM4;
      buf[1]=( eeprom4Val >> 8 );
      buf[2]=( eeprom4Val >> 0 );

      HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,3,100);
      HAL_Delay(20);
      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}

}

/*
   @Brief         Get Temperature Offset Value
   @Description   Get Temperature Offset Value for Calibrating
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  temp_err
 */
temp_err TMP116_get_EEPROM4(uint16_t *eeprom4Val)
{
	if(TMP_BUS_HANDLER){
      static uint8_t buf[3];
      buf[0]=TMP116_EEPROM4;

      if(HAL_I2C_Master_Transmit(TMP_BUS_HANDLER,TMP116_DeviceID,buf,0,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);
      if(HAL_I2C_Master_Receive(TMP_BUS_HANDLER,TMP116_DeviceID,buf,2,100) != HAL_OK){
    	  return TMP116_ERR;
      }
      HAL_Delay(20);

      *eeprom4Val = ((buf[0]<<8)|buf[1]);

      return TMP116_OK;
	}else{
		return TMP116_INIT_ERR;
	}
}

// IMU SPI implementation

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "IMU.h"
#include "Semihosting.h"
#include <string.h>
#include "bitmanip.h"
#include "Sqrt.h"

/// IMU Map
/// @todo Consider using malloc/free to control the size of this buffer dynamically.
IMU_MappingStruct IMUDevice[IMU_LAST];

/// IMU data buffer
/// @todo Consider using malloc/free to control the size of this buffer dynamically.
IMU_RAWFrame IMU_RAWFramebuffer[IMU_FRAMEBUFFER_SIZE];		/// @todo Implement FIFO 

/// Number of IMU framebuffers pending
uint8_t volatile IMU_RAWFramebuffersPending = 0;
uint8_t volatile IMU_RAWFramebuffer_ReadIndex = 0;
uint8_t volatile IMU_RAWFramebuffer_WriteIndex = 0;

// Performance tracking variables
uint32_t volatile IMU_framecount;

/// Tracks if imu dma is enabled.
bool IMU_Enabled = false;

/// @todo implement FPS tracking

/// Transfer state
IMU_TransferStateType IMU_TransferState;

/// Debugging utility function.
void DBG_SPICheckState(HAL_SPI_StateTypeDef state);

/// Test IVector3 alignment
bool DIAG_IMU_CheckAlignment(void)
{
	/// @todo automate this test to provide PASS/FAIL results.
	// Quick analysis of IVector3 alignment
	IVector3 testbyte;

	testbyte.txbyte = 0xDE;
	testbyte.x = 0;
	testbyte.y = INT16_MAX;
	testbyte.z = 0;

	// printout data
	printf_semi("IMU_CheckAlignment() - Test byte\n");
	printf_semi("testbyte.txbyte = %d [0x%02x] \t\t[address = %p]\n", testbyte.txbyte, testbyte.txbyte, &testbyte.txbyte);
	printf_semi("testbyte.x = %d [0x%04x] \t[address = %p]\n", testbyte.x, testbyte.x, &testbyte.x);
	printf_semi("testbyte.y = %d [0x%04x] \t[address = %p]\n", testbyte.y, testbyte.y, &testbyte.y);
	printf_semi("testbyte.z = %d [0x%04x] \t[address = %p]\n", testbyte.z, testbyte.z, &testbyte.z);

	// With padding, size should be 8 bytes.
	uint8_t expectedSize = sizeof(uint8_t) + (sizeof(int16_t) * 3);

#ifdef IMU_RAW_PADDING
	expectedSize += 1;
#endif // IMU_RAW_PADDING

	printf_semi("size = %d, (expected %d bytes)\nbase addr = %p\n", sizeof(testbyte),  &testbyte);

	for (int i = 0; i < sizeof(testbyte); i++)
	{
		printf_semi("\t[%d][%p] = 0x%02x\n", i, &((uint8_t*)&testbyte)[i],((uint8_t*)&testbyte)[i]);
	}

	// Don't fix this until we're actually able to determine the results programmatically
	return false;

}


/// Determines if the SPI port can communicate with the IMU.
bool DIAG_IMU_Test(IMU_PortType imuport)
{
	/// @bug - we are currently assuming Alpha-0 IMUs (LSM330DLC)
	switch (IMUDevice[imuport].DeviceName)
	{
		
		case IMU_DEVICE_LSM330DLC:

			{
				// Is SPI ready?
				HAL_SPI_StateTypeDef spistateresult = HAL_SPI_GetState(IMUDevice[imuport].hspi);

				if (spistateresult == HAL_SPI_STATE_READY)
				{

					// Select the gyroscope
					IMU_SelectSubDevice(imuport, IMU_SUBDEV_GYRO);

					/// Test for combination tx/rx buffer
					uint8_t spi_txrx[2] = { 0, 0 };
					spi_txrx[0] = LSM330DLC_FormatAddress(true, false, LSM330DLC_REG_WHO_AM_I_G);

					// Attempt combination TX/RX
					if (HAL_SPI_TransmitReceive(IMUDevice[imuport].hspi, spi_txrx, spi_txrx, 2, SPI_TIMEOUT) != HAL_OK)
					{
						// No! Bad! Bad STM32! 
						printf_semi("SPI IMU Test (transmit/receive) failed.\n");
				
					}

					// Reset \CS
					IMU_SelectSubDevice(imuport, IMU_SUBDEV_NONE);

					// ...Result should equal 0b11010100
					if (spi_txrx[1] == LSM330DLC_WHO_AM_I_G_VALUE)
					{
						// Ready to go!
						printf_semi("SPI IMU Test successful. (TxRx buffer: 0x%02x 0x%02x).\n", spi_txrx[0], spi_txrx[1]);

						return true;
						
					} 
					else
					{
						// Ready to go!
						printf_semi("SPI IMU Test unsuccessful (received 0x%02x 0x%02x).\n", spi_txrx[0], spi_txrx[1]);
					}
				}
				else
				{
				#ifdef DEBUG
					DBG_SPICheckState(spistateresult);
				#endif
				}

			}
			break;

		default:
			
			printf_semi("Cannot test implemented device IMU_Device (%d)", IMUDevice[imuport].DeviceName);
			break;
		
	}
	return false;
}

void IMU_SelectSubDevice(IMU_PortType port, IMU_SubDeviceType component)
{
	IMU_TransferState.SelectedSubDevice[port] = component;
	/// @bug It may be better to specifically deactivate all components, first.

	// Select the requested component
	for (int i = 0; i < IMU_SUBDEV_NONE; i++)
	{
		
		if (IMUDevice[port].CSMappings[i].port != NULL)		// Paranoia, until it isn't.
		{
			HAL_GPIO_WritePin(IMUDevice[port].CSMappings[i].port, IMUDevice[port].CSMappings[i].pin, (i == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
		}
	}

#ifdef DEBUG_MONITOR_SPI6_ONBOARD_CS
	// Mirror onto PB4/PB5 to use a logic analyzer connected to SPI6 to monitor onboard SPI communications. (EVP4)
	if (IMU_ONBOARD == port)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (IMU_SUBDEV_ACC == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (IMU_SUBDEV_GYRO == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
	}
#endif

}



void IMU_Setup(void)
{
	
	/// @bug - I don't know if this is necessary on arm-eabi....
	memset(IMUDevice, 0, sizeof(IMUDevice));

	/// @bug - I don't know if this is necessary on arm-eabi....
	memset(&IMU_TransferState, 0, sizeof(IMU_TransferState));
	for (int i = 0; i < IMU_LAST; i++)
	{
		IMU_TransferState.SelectedSubDevice[i] = IMU_SUBDEV_NONE;
	}

	/// @bug - I don't know if this is necessary on arm-eabi....
	memset(&IMU_RAWFramebuffer, 0, sizeof(IMU_RAWFramebuffer));
	
	/// @bug This needs to be done via auto-detection.
	for (int i = 0; i < IMU_LAST; i++)
	{
		switch (i)
		{
		case IMU_ONBOARD:				/// LSM330DLC (EVP4)
		// SPI6
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi6;
			// CS2 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOB;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_8;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS3 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOB;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_9;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// INT2 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOG;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_10;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// INT3 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOG;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_11;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// INT4 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOE;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_0;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// INT5 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOE;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_1;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity



			break;
		case IMU_P1:
		// SPI2
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;		/// @todo This should be indicated by the device driver directly.
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi2;
			// CS0 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOD;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_8;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS1 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOD;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_9;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// SPI6_INT0 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOD;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_10;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// SPI6_INT1 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOD;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_11;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// SPI6_RESV0 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOG;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_2;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// SPI6_RESV1 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOG;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_3;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity

			// Configure IMU settings.
			break;
		case IMU_P2:
		// SPI3
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;		/// @todo This should be indicated by the device driver directly.
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi3;
			// CS0 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOD;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_0;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS1 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOD;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_1;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// SPI6_INT0 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOD;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_2;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// SPI6_INT1 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOH;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_13;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// SPI6_RESV0 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOH;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_14;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// SPI6_RESV1 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOH;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_15;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity

			// Configure IMU settings.
			break;
		case IMU_P3:

		// SPI6
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;		/// @todo This should be indicated by the device driver directly.
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi6;
			// CS0 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOB;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_4;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS1 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOB;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_5;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// SPI6_INT0 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOD;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_7;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// SPI6_INT1 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOG;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_9;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// SPI6_RESV0 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOI;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_4;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// SPI6_RESV1 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOE;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_3;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity

			// Configure IMU settings.
			break;
		case IMU_P4:
		// SPI4
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;		/// @todo This should be indicated by the device driver directly.
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi4;
			// CS0 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOI;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_10;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS1 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOI;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_11;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// SPI6_INT0 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOC;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_13;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// SPI6_INT1 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOI;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_9;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// SPI6_RESV0 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOE;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_4;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// SPI6_RESV1 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOI;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_8;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity

			// Configure IMU settings.
			break;
		case IMU_P5:
		// SPI5
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;		/// @todo This should be indicated by the device driver directly.
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi5;
			// CS0 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOF;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_2;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS1 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOF;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_3;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// SPI6_INT0 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOF;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_4;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// SPI6_INT1 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOF;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_5;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// SPI6_RESV0 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOF;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_6;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// SPI6_RESV1 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOF;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_10;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity

			// Configure IMU settings.
			break;
		case IMU_P6:
		// SPI1
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;		/// @todo This should be indicated by the device driver directly.
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi1;
			// CS0 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOH;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_2;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS1 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOH;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_3;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			IMU_SelectSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// Configure Interrupts
			// SPI6_INT0 (INT1_A)
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].port = GPIOH;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_4;
			IMUDevice[i].INTMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
/*			// SPI6_INT1 (INT2_A)
			IMUDevice[i].INTMappings[1].port = GPIOH;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_5;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;

			// SPI6_RESV0 (INT1_G)
			IMUDevice[i].INTMappings[2].port = GPIOA;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_3;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
*/
			// SPI6_RESV1 (DRDY_G/INT2_G)
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].port = GPIOA;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_4;
			IMUDevice[i].INTMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			// Test IMU connectivity

			break;
		default:
			// This is serious.
			#ifdef DEBUG
			// UsageFault_Handler();
			printf_semi("IMU_Setup() encountered an unimplemented IMU_PortType (%d).\n", i);
			__BKPT(0);
			#endif
			break;

		}

		// Configure IMU settings.
		IMU_Configure(i);

	}


	// Initialize the framebuffers

	// Quaternion Framebuffer
	memset(SPATIAL_QUATERNION_Framebuffer, 0, sizeof(SPATIAL_QUATERNION_Framebuffer));
	for (int f = 0; f < SPATIAL_IMUFRAMEBUFFER_SIZE; f++)
	{
		// Normalized to zero rotation.
		for (int imu = 0; imu < IMU_LAST; imu++)
		{
			SPATIAL_QUATERNION_Framebuffer[f].q[imu].w = 1;
		}
	}

}


void DBG_SPICheckState(HAL_SPI_StateTypeDef state)
{
	switch (state)
	{
		HAL_SPI_STATE_RESET:
			printf_semi("SPI not yet initialized or disabled.\n");
			break;
		HAL_SPI_STATE_READY:
			printf_semi("SPI initialized and ready for use.\n");
			break;
		HAL_SPI_STATE_BUSY:
			printf_semi("SPI process is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_TX:
			printf_semi("SPI transmission is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_RX:
			printf_semi("SPI reception is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_TX_RX:
			printf_semi("SPI transmission/reception is ongoing.\n");
			break;
		HAL_SPI_STATE_ERROR:
			printf_semi("SPI error state.\n");
			break;
		default:
			printf_semi("SPI unknown state.\n");
			break;
	}
	
}

void IMU_SPI1_Handle_IT(void)
{

	// Kick-off a burst receive operation, if needed.
	

	for (int subdev = 0; subdev < IMU_SUBDEV_NONE; subdev++)
	{

		switch (IMU_TransferState.TransferStep[IMU_P6][subdev])
		{
		case IMU_XFER_CHECKING:
		{
			// A polling operation is in progress.
			// Receive 1 byte into the polling buffer.
			uint8_t* pPollingBuffer = (void*)&IMU_TransferState.PollingBuffer[IMU_P6] + 1;
			HAL_StatusTypeDef result = HAL_SPI_Receive_DMA(IMUDevice[IMU_P6].hspi, pPollingBuffer, 1);	// size = addr + xyz
			if (HAL_OK != result)
			{
				// Transfer start failed.
				printf_semi("IMU_SPI1_Handle_IT() port %d, subdev %d - HAL_SPI_Receive_DMA failed(%d)\n", IMU_P6, IMU_TransferState.SelectedSubDevice[IMU_P6], result);
				/// @todo introduce a DBG_HAL_StatusTypeDef helper function.
			}
		}

			break;

		case IMU_XFER_WAIT:
			// A transfer operation is in progress.
			IMU_GetRAWBurst(IMU_P6, subdev);
			break;
		default:
			// Nothing to see here.
			break;
		}
	}
}

void IMU_GetRAWBurst(IMU_PortType port, IMU_SubDeviceType subdev)
{
	// This is a special circumstance.
	// The imu is in a known state, and ready for an immediate receive operation.

	IVector3* pIVector3 = NULL;
	switch (subdev)
	{
	case IMU_SUBDEV_ACC:
		pIVector3 = (void*)&IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[port].accelerometer;
		break;

	case IMU_SUBDEV_GYRO:
		pIVector3 = (void*)&IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[port].gyroscope;
		break;

	default:
		printf_semi("IMU_GetRAWBurst(port %d, subdev %d) - Unimplemented subdevice.\n", port, subdev);
#ifdef DEBUG
		__BKPT(0);
#endif // DEBUG
		break;
	}

	pIVector3->txbyte = 0;
	pIVector3->x = 0;
	pIVector3->y = 0;
	pIVector3->z = 0;

	// Declare the transfer
	IMU_TransferState.TransferStep[port][subdev] = IMU_XFER_WAIT;

	// Start DMA transfer to appropriate frame
	// Internally, the HAL uses TxRx, but we need Receive-only in order to accomodate the SPI1_TX/DMA workaround.
	HAL_StatusTypeDef result = HAL_SPI_Receive_DMA(IMUDevice[port].hspi, /*(uint8_t*)&pIVector3->x, */(uint8_t*)&pIVector3->x, 6);	// size = xyz
	if (HAL_OK != result)
	{
		// Transfer start failed.
		printf_semi("IMU_GetRAW(port %d, subdev %d) - HAL_SPI_Receive_DMA failed(%d)\n", port, subdev, result);
		/// @todo introduce a DBG_HAL_StatusTypeDef helper function.
	}

	// All done

}

void IMU_GetRAW(IMU_PortType port, IMU_SubDeviceType subdev)
{

	/**
	* @todo LSM6DS0 support will support a different approach.
	* Using a single CS, subdevice settings will not be required. 
	* Burst transfers of all gyroscope and accelerometer settings
	* will need a new RAW structure.	
	*/

	/// @todo Break apart all device-specific options and methods into a driver-architecture.

	// Select subdevice.
	IMU_SelectSubDevice(port, subdev);

	// Address
	IVector3* pIVector3 = NULL;
	uint8_t addressByte = 0;
	switch (subdev)
	{
	case IMU_SUBDEV_ACC:
		pIVector3 = (void*)&IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[port].accelerometer;
		addressByte = LSM330DLC_REG_OUT_X_L_A;
		break;

	case IMU_SUBDEV_GYRO:
		pIVector3 = (void*)&IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[port].gyroscope;
		addressByte = LSM330DLC_REG_OUT_X_L_G;
		break;

	default:
		printf_semi("IMU_GetRAW(port %d, subdev %d) - Unimplemented subdevice.\n", port, subdev);
#ifdef DEBUG
		__BKPT(0);
#endif // DEBUG
		break;
	}

	// Setup address byte
	/// @bug - This needs to be accessed via a function pointer that matches devices w/ports based on detection.
	pIVector3->txbyte = LSM330DLC_FormatAddress(true, true, addressByte);

	// Declare the transfer
	IMU_TransferState.TransferStep[port][subdev] = IMU_XFER_WAIT;

	// Start DMA transfer to appropriate frame
	HAL_StatusTypeDef result;
	/**
	 * @bug SPI1_TX DMA Workaround
	 * Hardware can't support EVERY possible SPI port simultaneously.  SPI1_TX got squeezed out.
	 * Instead of sharing with another Stream/Channel, SPI1 will send using an interrupt Transfer.
	 */
	if (IMUDevice[port].hspi == &hspi1)
	{
		// Use an interrupt transfer.  Once the notification comes back, we'll fire a burst operation to get the data.
		result = HAL_SPI_Transmit_IT(IMUDevice[port].hspi, &pIVector3->txbyte, 1);	// size = addr + xyz
	}
	else
	{
		result = HAL_SPI_TransmitReceive_DMA(IMUDevice[port].hspi, &pIVector3->txbyte, &pIVector3->txbyte, 7);	// size = addr + xyz
	}


	if (HAL_OK != result)
	{

		// Transfer start failed.
		printf_semi("IMU_GetRAW(port %d, subdev %d) - HAL_SPI_TransmitReceive_DMA failed(%d)\n", port, subdev, result);
		/// @todo introduce a DBG_HAL_StatusTypeDef helper function.

	}

	// All done 

}

void IMU_Poll(IMU_PortType port, IMU_SubDeviceType subdev)
{
	/// @todo Change polling to deposit data directly into the the RAW data structure (requires an alternate RAW structure w/2 accessible padding bytes.)
	// Fire a full-duplex request at the IMU.
	// Select subdevice.
	IMU_SelectSubDevice(port, subdev);

	// Address
	uint8_t* pPollingBuffer = (void*)&IMU_TransferState.PollingBuffer[port];
	uint8_t addressByte = 0;
	switch (subdev)
	{
	case IMU_SUBDEV_ACC:
		addressByte = LSM330DLC_REG_STATUS_REG_A;
		break;

	case IMU_SUBDEV_GYRO:
		addressByte = LSM330DLC_REG_STATUS_REG_G;
		break;

	default:
		printf_semi("IMU_Poll(port %d, subdev %d) - Unimplemented subdevice.\n", port, subdev);
#ifdef DEBUG
		__BKPT(0);
#endif // DEBUG
		break;
	}

	// Setup address byte
	/// @bug - This needs to be accessed via a function pointer that matches devices w/ports based on detection.
	pPollingBuffer[0] = LSM330DLC_FormatAddress(true, true, addressByte);
	pPollingBuffer[1] = 0;

	// Start DMA transfer to appropriate frame
	HAL_StatusTypeDef result = HAL_SPI_TransmitReceive_DMA(IMUDevice[port].hspi, pPollingBuffer, pPollingBuffer, 2);	// size = addr + xyz
	if (HAL_OK != result)
	{
		// Transfer start failed.
		printf_semi("IMU_Poll(port %d, subdev %d) - HAL_SPI_TransmitReceive_DMA failed(%d)\n", port, subdev, result);
		/// @todo introduce a DBG_HAL_StatusTypeDef helper function.
	}

	// All done
}

/// Checks For Interrupt Events
void IMU_CheckIMUInterrupts(void)
{
	/// @bug This code assumes only one imu/port.  It will eventually need a rewrite.
	// For the each port...
	for (int port = 0; port < IMU_LAST; port++)
	{
		// and each subdevice...
		for (int subdev = 0; subdev < IMU_SUBDEV_NONE; subdev++)
		{
			// ... check each interrupt

			/// @bug - This code is inappropriately specific to the LSM330DLC.  This WILL need to be changed to a single-interrupt system!

			/// @bug - Interrupts MAY need to be followed by a polling operation. (in the case of not all axis values being updated)

			// If subdevice interrupt is configured, and transferstate is idle...
			if ((IMUDevice[port].INTMappings[subdev].type == subdev) && (IMU_TransferState.TransferStep[port][subdev] == IMU_XFER_IDLE))
			{
				// Check for active interrupt.
				if (GPIO_PIN_SET == HAL_GPIO_ReadPin(IMUDevice[port].INTMappings[subdev].port, IMUDevice[port].INTMappings[subdev].pin))
				{
					/// @bug - Unknown if interrupt is active high or low. A device-driver function may be required here.
					// Change status to pending.
					IMU_TransferState.TransferStep[port][subdev] = IMU_XFER_PENDING;
				}
			}

		}
	}
}

/// Evaluates Polling Results
IMU_TransferStepType IMU_CheckPollingResult(IMU_PortType port, IMU_SubDeviceType subdev)
{
	/// @todo Implement a more dynamic device driver here.
	// Evaluate polling buffer.
	switch (IMUDevice[port].DeviceName)
	{
	case IMU_DEVICE_LSM330DLC:
		/// Ensure X, Y, & Z data has been updated.

		if (CHECK_FLAGS(IMU_TransferState.PollingBuffer[port][1], LSM330DLC_STATUS_ZYXDA))
		{
			// Change status to pending.
			IMU_TransferState.TransferStep[port][subdev] = IMU_XFER_PENDING;
		} 
		else
		{
			// Not ready... Back to idle.
			IMU_TransferState.TransferStep[port][subdev] = IMU_XFER_IDLE;
		}

		break;
	default:
		printf_semi("IMU_CheckPollingResult(port %d, subdev %d) - Unimplemented device.\n", port, subdev);
	}

	return IMU_TransferState.TransferStep[port][subdev];
}

/// Moves to next transfer stage
void IMU_HandleSPIEvent(IMU_PortType port)
{
	// Determine selected imu and subdevice
	IMU_SubDeviceType subdev = IMU_TransferState.SelectedSubDevice[port];
	
	// Per state
	switch (IMU_TransferState.TransferStep[port][subdev])
	{
	case IMU_XFER_IDLE:
		// A polling event was just completed.
		

		// If the polling result changed the state to pending, drop through.
		if (IMU_XFER_IDLE == IMU_CheckPollingResult(port, subdev))
		{
			// Nothing pending.
			// Release the CS line. We'll be back later.
			IMU_SelectSubDevice(port, IMU_SUBDEV_NONE);

			return;
		}
		// The device is already selected and addressed.
		// Followup the polling operation with a burst receive.
		IMU_GetRAWBurst(port, subdev);
		break;

	case IMU_XFER_PENDING:
		// Poll has determined that data is available
		//
		// Prepare and start the transfer
		IMU_GetRAW(port, subdev);

		break;

	case IMU_XFER_WAIT:
		// Data transfer was in progress and has completed.

		// Timestamp the data
		IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[port].tickstamp = HAL_GetTick();

		// Mark as complete.
		IMU_TransferState.TransferStep[port][subdev] = IMU_XFER_COMPLETE;

		// Deselect the device.
		IMU_SelectSubDevice(port, IMU_SUBDEV_NONE);

		break;

	case IMU_XFER_COMPLETE:

		// A polling event was just completed.
		printf_semi("IMU_HandleSPIEvent(port %d) - IMU_XFER_COMPLETE isn't a SPI event...\n", port);
		break;

	default:
		printf_semi("IMU_HandleSPIEvent(port %d) - Unimplemented IMU_XFER state.\n", port);
#ifdef DEBUG
		__BKPT(0);
#endif // DEBUG

		break;
	}

}


void IMU_Configure(IMU_PortType port)
{


	switch (IMUDevice[port].DeviceName)
	{

	case IMU_DEVICE_LSM330DLC:
		{
			IMU_SelectSubDevice(port, IMU_SUBDEV_ACC);

			uint8_t configurationPacket[7] = { 0, 0, 0, 0, 0, 0, 0 };
		// Accelerometer setup.
			// Configure address packet
			configurationPacket[0] = LSM330DLC_FormatAddress(false, true, LSM330DLC_REG_CTRL_REG1_A);

			// Reset IMU

			// Power on.
			configurationPacket[1] = LSM330DLC_CTRL_REG1_A_ODR_100HZ | LSM330DLC_CTRL_REG1_A_X_ENABLE | LSM330DLC_CTRL_REG1_A_Y_ENABLE | LSM330DLC_CTRL_REG1_A_Z_ENABLE;
			
			// Setup internal filter
			configurationPacket[2] = LSM330DLC_CTRL_REG2_A_HPM_NORMAL_RESET | LSM330DLC_CTRL_REG2_A_FDS_ENABLE;	

			// Interrupt configuration
			configurationPacket[3] = LSM330DLC_CTRL_REG3_A_I1_DRDY1_ENABLE | LSM330DLC_CTRL_REG3_A_I1_DRDY2_ENABLE;

			// Full scale, endianness, resolution 
			configurationPacket[4] = LSM330DLC_CTRL_REG4_A_BLE_LITTLEENDIAN | LSM330DLC_CTRL_REG4_A_FS_2G | LSM330DLC_CTRL_REG4_A_HR_ENABLE;
			
			// FIFO reset, FIFO settings, Interrupt latch, 4D detection
			configurationPacket[5] = LSM330DLC_CTRL_REG5_A_BOOT;

			// Active high interrupt, no INT2_A, for now.
			configurationPacket[6] = 0;
									
			// Cache the fullscale value.
			IMUDevice[port].FullScale[IMU_SUBDEV_ACC] = 2.0f;

			// Cache the ODR
			IMUDevice[port].ODR[IMU_SUBDEV_ACC] = 100.0f;

			// Send data
			HAL_StatusTypeDef result = HAL_SPI_Transmit(IMUDevice[port].hspi, configurationPacket, sizeof(configurationPacket), 200);
			if(result != HAL_OK)
			{

#ifdef DEBUG
				printf_semi("ConfigureIMU(%d), IMU acc setup failed (%d)\n", port, result);

				IMU_SelectSubDevice(port, IMU_SUBDEV_NONE);
				__BKPT(0);
				return;
#endif // DEBUG

			}
			
			IMU_SelectSubDevice(port, IMU_SUBDEV_GYRO);
			
			// clear
			memset(configurationPacket, 0, sizeof(configurationPacket));

		// Gyroscope

			// Address packet
			configurationPacket[0] = LSM330DLC_FormatAddress(false, true, LSM330DLC_REG_CTRL_REG1_G);

			// CTRL_REG1_G - power on.
			configurationPacket[1] = LSM330DLC_CTRL_REG1_G_ODR_95HZ_BW25 | LSM330DLC_CTRL_REG1_G_X_ENABLE | LSM330DLC_CTRL_REG1_G_Y_ENABLE | LSM330DLC_CTRL_REG1_G_Z_ENABLE;

			// CTRL_REG2_G - Reset filter, default HP cutoff frequency.
			configurationPacket[2] = LSM330DLC_CTRL_REG2_G_HPM_NORMAL_RESET | LSM330DLC_CTRL_REG2_G_HPCF_0;

			// CTRL_REG3_G - Interrupt settings
			configurationPacket[3] = LSM330DLC_CTRL_REG3_A_I1_DRDY1_ENABLE;

			// CTRL_REG4_G - Endianness, Full scale selection
			configurationPacket[4] = LSM330DLC_CTRL_REG4_G_FS_250DPS;

			// CTRL_REG5_G - FIFO, high pass filter, Output selection, Interrupt selection
			configurationPacket[5] = LSM330DLC_CTRL_REG5_G_HPen | LSM330DLC_CTRL_REG5_G_OUT_SEL_HPF;

			// Send
			result = HAL_SPI_Transmit(IMUDevice[port].hspi, configurationPacket, sizeof(configurationPacket) - 1, 200);
			if(result != HAL_OK)
			{
				printf_semi("ConfigureIMU(%d) - IMU gyro setup failed (%d)\n", port, result);
				return;

			}

			IMU_SelectSubDevice(port, IMU_SUBDEV_NONE);


			// Cache the fullscale value.
			IMUDevice[port].FullScale[IMU_SUBDEV_GYRO] = 250.0f;

			// Cache the ODR
			IMUDevice[port].ODR[IMU_SUBDEV_GYRO] = 95.0f;

			// ? Power on
			// ODR / Power On
				// Accelerometer ODR (1Hz, 10Hz, 25Hz, 50Hz, 100Hz, 200Hz, 400Hz, 1.62kHz, 1.344kHz)
				// Gyroscope ODR (95Hz, 190Hz, 380Hz, 760Hz)
		}
		break;

	/// @todo Support for 9-axis devices (LSM9DS0)
	/// @todo Support for external-brains (LSM6DB0)


		break;
	}

	// Setup complete.
	IMU_Enabled = true;
	SPATIAL_IMUFrameBuffer_ReadIndex = 0;
	SPATIAL_IMUFrameBuffer_WriteIndex = 0;

}

IMU_PortType IMU_GetPortFromSPIHandle(SPI_HandleTypeDef* hspi)
{

	/// @bug This may need to be manually unrolled.	Analyze/profile this!

	/* Flesh this out to manually unroll...
	intptr_t ihspi = (intptr_t)(void *)hspi;	// I know what you're thinking...

	switch (ihspi)
	{
	case (intptr_t)(void*)&hspi6:
		return IMU_ONBOARD;
		break;
	default:
		/// Unrecognized hspi
		printf_semi("GetIMU(%p) - IMU not found/implemented", (void *)hspi);
		break;
	}
	*/

	// Select the requested component
	for (int i = 0; i < IMU_LAST; i++)
	{
		// Disable
		
		if (IMUDevice[i].hspi == hspi)
		{
			return i;
		}
	}

	printf_semi("GetIMU(%p) - IMU not found/implemented.", (void*)hspi);
#ifdef DEBUG
	__BKPT(0);
#endif

	return IMU_LAST;


}


void IMU_SystickHandler(void)
{
	// Wait until enabled.
	if (IMU_Enabled == false)
		return;

	// Update interrupt status
	IMU_CheckIMUInterrupts();

	bool bFrameComplete = true;

	/// @todo Profile and improve this.  1mS is "good enough" for polling", But this ISR is running at a really high (Max) priority... Maybe a low-priority timer needs to be setup?
	/// @todo This could use up a lot less memory for TransferState, also.
	// Check frame status
	for (int port = 0; port < IMU_LAST; port++)
	{

	/// @bug temporary change to test DMA bandwidth.
		if (port != IMU_ONBOARD)
		{
			IMU_TransferState.TransferStep[port][IMU_SUBDEV_ACC] = IMU_XFER_COMPLETE;
			IMU_TransferState.TransferStep[port][IMU_SUBDEV_GYRO] = IMU_XFER_COMPLETE;
		}

		for (int subdev = 0; subdev < IMU_SUBDEV_NONE; subdev++)
		{

			if (IMU_TransferState.TransferStep[port][subdev] != IMU_XFER_COMPLETE)
			{
				// Not complete yet.
				bFrameComplete = false;
			}

			if(IMU_TransferState.TransferStep[port][subdev] == IMU_XFER_PENDING)
			{
				bFrameComplete = false;
				// Start a retrieval operation
				IMU_GetRAW(port, subdev);
	
				// Skip remaining subdevices.
				break;
			}

		}

	}

	if (bFrameComplete)
	{

	// @bug Temp debug section.
	IMU_Enabled = false;

		// Frame is ready
		IMU_framecount++;
		/*
			printf_semi("Frame %d:\n ax %d\n ay %d\n az %d\n gx %d\n gy %d\n gz %d\n ",
			IMU_RAWFramebuffer_WriteIndex,
			IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[IMU_ONBOARD].accelerometer.x,
			IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[IMU_ONBOARD].accelerometer.y,
			IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[IMU_ONBOARD].accelerometer.z,
			IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[IMU_ONBOARD].gyroscope.x,
			IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[IMU_ONBOARD].gyroscope.y,
			IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex].imu[IMU_ONBOARD].gyroscope.z);
		*/
			
		// Advance WriteIndex.
		if (++IMU_RAWFramebuffer_WriteIndex == IMU_FRAMEBUFFER_SIZE)
			IMU_RAWFramebuffer_WriteIndex = 0;

		// Initialize next frame.
		memset(&IMU_RAWFramebuffer[IMU_RAWFramebuffer_WriteIndex], 0, sizeof(IMU_RAWFrame));

		// Reset Transfer State
		memset(&IMU_TransferState, 0, sizeof(IMU_TransferState));
		for (int i = 0; i < IMU_LAST; i++)
		{
			IMU_TransferState.SelectedSubDevice[i] = IMU_SUBDEV_NONE;
		}

		// Frame Ready
		// This *would* be a nice time to process the finished frame, but lets queue it for processing instead.

	}

}

uint32_t SPATIAL_IMUFrameBuffer_NumProcessed = 0;

// Process RAWFrame
void IMU_ProcessRAWFrame(void)
{
	// Make sure there is a frame to process
	if (IMU_RAWFramebuffer_ReadIndex == IMU_RAWFramebuffer_WriteIndex)
		return;

	// Process frame data

	// Clear the destination frame
	memset(&SPATIAL_IMUFrameBuffer[SPATIAL_IMUFrameBuffer_WriteIndex], 0, sizeof(IMU_SCALED_FramebufferType));
	
	for (int i = 0; i < IMU_LAST; i++)
	{
		// Rescale values
		SPATIAL_IMUFrameBuffer[SPATIAL_IMUFrameBuffer_WriteIndex].imu[i].tickstamp = IMU_RAWFramebuffer[IMU_RAWFramebuffer_ReadIndex].imu[i].tickstamp;
		SPATIAL_RAWToVector3(&IMU_RAWFramebuffer[IMU_RAWFramebuffer_ReadIndex].imu[i].accelerometer, &SPATIAL_IMUFrameBuffer[SPATIAL_IMUFrameBuffer_WriteIndex].imu[i].accelerometer, IMU_GetFullScaleMultiplier(i, IMU_SUBDEV_ACC));
		SPATIAL_RAWToVector3(&IMU_RAWFramebuffer[IMU_RAWFramebuffer_ReadIndex].imu[i].gyroscope, &SPATIAL_IMUFrameBuffer[SPATIAL_IMUFrameBuffer_WriteIndex].imu[i].gyroscope, IMU_GetFullScaleMultiplier(i, IMU_SUBDEV_GYRO));
//		SPATIAL_RAWToVector3(&IMU_RAWFramebuffer[IMU_RAWFramebuffer_ReadIndex].imu[i].magnetometer, &SPATIAL_IMUFrameBuffer[SPATIAL_IMUFramwBuffer_WriteIndex].imu[i].magnetometer, IMU_GetFullScaleMultiplier(i,IMU_SUBDEV_MAG));

		// Convert scaled Gyro values from degrees/sec into Radians/sec
		SPATIAL_SCALEDToRadiansPerSec(&SPATIAL_IMUFrameBuffer[SPATIAL_IMUFrameBuffer_WriteIndex].imu[i].gyroscope);

	}

	/// @todo IMU_ProcessOrientation() during IMU_ProcessRAWFrame() should be selectable as a configuration item, sometimes you don't need quaternion data.
	IMU_ProcessOrientation();

	// 

	// Increment RAW ReadIndex and SCALED Write index
	IMU_RAWFramebuffer_ReadIndex++;

	if (IMU_RAWFramebuffer_ReadIndex == IMU_FRAMEBUFFER_SIZE)
	{
		IMU_RAWFramebuffer_ReadIndex = 0;
	}

	SPATIAL_IMUFrameBuffer_WriteIndex++;

	if (SPATIAL_IMUFrameBuffer_WriteIndex == SPATIAL_IMUFRAMEBUFFER_SIZE)
	{
		SPATIAL_IMUFrameBuffer_WriteIndex = 0;
	}

	SPATIAL_IMUFrameBuffer_NumProcessed++;

}

// Process SPATIAL frame into a QUATERNION frame
void IMU_ProcessOrientation(void)
{

	// Clear the destination frame

	for (int i = 0; i < IMU_LAST; i++)
	{
		// Quaternion Conversion.
		IMU_QuaternionUpdate( &SPATIAL_QUATERNION_Framebuffer[0].q[i], &SPATIAL_IMUFrameBuffer[SPATIAL_IMUFrameBuffer_WriteIndex].imu[i], IMUDevice[i].ODR[IMU_SUBDEV_GYRO], 0.1f);		/// @todo beta needs to be a setting.
		
	}
}

float IMU_GetFullScaleMultiplier(IMU_PortType port, IMU_SubDeviceType subdev)
{
	/// @todo Breakout device-specific settings into their own files, methods, etc.
	return IMUDevice[port].FullScale[subdev];	// That was easy.

}

void IMU_AutoSetFullScaleSetting(IMU_PortType port, IMU_SubDeviceType subdev)
{

	// Evaluate the framebuffer for IMU values and rescale to enchance precision.
	// If the value is consistently in the middle 50%, do nothing.
	// If the value is consistently in the bottom or top 25%, scale in that direction.
	// If the value is consistently in the middle 50%, do nothing.
	
	// Evaluate:
	// Maximum
	// Minimum
	// Average

}

float IMU_ForceFullScaleSetting(IMU_PortType port, IMU_SubDeviceType subdev, float g_max)
{
	float g_new = 0.0f;

	switch (IMUDevice[port].DeviceName)
	{
	case IMU_DEVICE_LSM330DLC:

		switch (subdev)
		{
		case IMU_SUBDEV_ACC:
			// 
			if ((g_max <= 2.0f) && (g_max >= -2.0f))
			{
				g_new = 2.0f;
			}
			else if ((g_max <= 4.0f) && (g_max >= -4.0f))
			{
				g_new = 4.0f;
			}
			else if ((g_max <= 8.0f) && (g_max >= -8.0f))
			{
				g_new = 8.0f;
			}
			else if ((g_max <= 16.0f) && (g_max >= -16.0f))
			{
				g_new = 16.0f;
			}
			else
			{
				// Requested scale is out of range.
				/// @todo return some kind of indicator?  set a special flag?
				printf_semi("IMU_ForceFullScaleSetting() - Requested scale out of range.\n");
				g_new = 16.0f;
			}

			// Set full-scale value.
			/// @todo Each port needs it's own generic buffer and locking.
			
			/// @bug send code.

			break;

		case IMU_SUBDEV_GYRO:
			if ((g_max <= 250.0f) && (g_max >= -250.0f))
			{
				g_new = 250.0f;
			}
			else if ((g_max <= 500.0f) && (g_max >= -500.0f))
			{
				g_new = 500.0f;
			}
			else if ((g_max <= 2000.0f) && (g_max >= -2000.0f))
			{
				g_new = 2000.0f;
			}
			else
			{
				// Requested scale is out of range.
				/// @todo return some kind of indicator?  set a special flag?
				printf_semi("IMU_ForceFullScaleSetting() - Requested scale out of range.\n");
				g_new = 2000.0f;
			}

			/// @bug send code.


			break;
		default:
			printf_semi("IMU_ForceFullScaleSetting() - Unimplemented/Supported subdevice type.\n");
			break;
		}
		break;

	/// @todo IMU_DEVICE_LSM6DO0:

	default:
		printf_semi("IMU_ForceFullScaleSetting() - Unimplemented/Supported device type.\n");
		break;
	}

	/// @bug systick and old scaling needs to be stored, as older readings 
	/// @bug need to be tossed or evaluated differently.

	// Cache new value
	IMUDevice[port].FullScale[subdev] = g_new;

	// All done.
	return g_new;
}

void IMU_QuaternionUpdate(Quaternion* pQ, IMU_SCALED* pIMU, float ODR, float beta)
{
	/// @todo IMU_QuaternionUpdate needs a configurable/optional Mahony Implementation on top of the Madgwick algorithm.

	/// @todo IMU_QuaternionUpdate is ripe for quite a bit of optimization.

	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	float _4q0, _4q1, _4q2 ,_8q1, _8q2; 

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalization)
	bool bUseMagnetometer = true;
	if ((pIMU->magnetometer.x == 0.0f) && (pIMU->magnetometer.y == 0.0f) && (pIMU->magnetometer.z == 0.0f))
	{
		bUseMagnetometer = false;
	}

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-pQ->x * pIMU->gyroscope.x - pQ->y * pIMU->gyroscope.y - pQ->z * pIMU->gyroscope.z);
	qDot2 = 0.5f * (pQ->w * pIMU->gyroscope.x + pQ->y * pIMU->gyroscope.z - pQ->z * pIMU->gyroscope.y);
	qDot3 = 0.5f * (pQ->w * pIMU->gyroscope.y - pQ->x * pIMU->gyroscope.z + pQ->z * pIMU->gyroscope.x);
	qDot4 = 0.5f * (pQ->w * pIMU->gyroscope.z + pQ->x * pIMU->gyroscope.y - pQ->y * pIMU->gyroscope.x);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalization)
	if (!((pIMU->accelerometer.x == 0.0f) && (pIMU->accelerometer.y == 0.0f) && (pIMU->accelerometer.z == 0.0f))) {

		// Normalize accelerometer measurement
		recipNorm = OPTIMIZED_invSqrt(pIMU->accelerometer.x * pIMU->accelerometer.x + pIMU->accelerometer.y * pIMU->accelerometer.y + pIMU->accelerometer.z * pIMU->accelerometer.z);
		pIMU->accelerometer.x *= recipNorm;
		pIMU->accelerometer.y *= recipNorm;
		pIMU->accelerometer.z *= recipNorm;

		// Normalize magnetometer measurement
		recipNorm = OPTIMIZED_invSqrt(pIMU->magnetometer.x * pIMU->magnetometer.x + pIMU->magnetometer.y * pIMU->magnetometer.y + pIMU->magnetometer.z * pIMU->magnetometer.z);
		pIMU->magnetometer.x *= recipNorm;
		pIMU->magnetometer.y *= recipNorm;
		pIMU->magnetometer.z *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic

		_2q0 = 2.0f * pQ->w;
		_2q1 = 2.0f * pQ->x;
		_2q2 = 2.0f * pQ->y;
		_2q3 = 2.0f * pQ->z;
		q0q0 = pQ->w * pQ->w;
		q1q1 = pQ->x * pQ->x;
		q2q2 = pQ->y * pQ->y;
		q3q3 = pQ->z * pQ->z;

		if (bUseMagnetometer)
		{
			_2q0mx = 2.0f * pQ->w * pIMU->magnetometer.x;
			_2q0my = 2.0f * pQ->w * pIMU->magnetometer.y;
			_2q0mz = 2.0f * pQ->w * pIMU->magnetometer.z;
			_2q1mx = 2.0f * pQ->x * pIMU->magnetometer.x;
			_2q0q2 = 2.0f * pQ->w * pQ->y;
			_2q2q3 = 2.0f * pQ->y * pQ->z;
			q0q1 = pQ->w * pQ->x;
			q0q2 = pQ->w * pQ->y;
			q0q3 = pQ->w * pQ->z;
			q1q2 = pQ->x * pQ->y;
			q1q3 = pQ->x * pQ->z;
			q2q3 = pQ->y * pQ->z;
		}
		else
		{

			_2q0 = 2.0f * pQ->w;
			_2q1 = 2.0f * pQ->x;
			_2q2 = 2.0f * pQ->y;
			_2q3 = 2.0f * pQ->z;
			_4q0 = 4.0f * pQ->w;
			_4q1 = 4.0f * pQ->x;
			_4q2 = 4.0f * pQ->y;
			_8q1 = 8.0f * pQ->x;
			_8q2 = 8.0f * pQ->y;

		}

		if (bUseMagnetometer)
		{
			// Reference direction of Earth's magnetic field
			hx = pIMU->magnetometer.x * q0q0 - _2q0my * pQ->z + _2q0mz * pQ->y + pIMU->magnetometer.x * q1q1 + _2q1 * pIMU->magnetometer.y * pQ->y + _2q1 * pIMU->magnetometer.z * pQ->z - pIMU->magnetometer.x * q2q2 - pIMU->magnetometer.x * q3q3;
			hy = _2q0mx * pQ->z + pIMU->magnetometer.y * q0q0 - _2q0mz * pQ->x + _2q1mx * pQ->y - pIMU->magnetometer.y * q1q1 + pIMU->magnetometer.y * q2q2 + _2q2 * pIMU->magnetometer.z * pQ->z - pIMU->magnetometer.y * q3q3;
			arm_sqrt_f32(hx * hx + hy * hy, &_2bx);	/// @todo consider changing this to use an internal sqrt w/debug features.
			_2bz = -_2q0mx * pQ->y + _2q0my * pQ->x + pIMU->magnetometer.z * q0q0 + _2q1mx * pQ->z - pIMU->magnetometer.z * q1q1 + _2q2 * pIMU->magnetometer.y * pQ->z - pIMU->magnetometer.z * q2q2 + pIMU->magnetometer.z * q3q3;
			_4bx = 2.0f * _2bx;
			_4bz = 2.0f * _2bz;

			// Gradient decent algorithm corrective step
			s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - pIMU->accelerometer.x) + _2q1 * (2.0f * q0q1 + _2q2q3 - pIMU->accelerometer.y) - _2bz * pQ->y * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - pIMU->magnetometer.x) + (-_2bx * pQ->z + _2bz * pQ->x) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - pIMU->magnetometer.y) + _2bx * pQ->y * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - pIMU->magnetometer.z);
			s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - pIMU->accelerometer.x) + _2q0 * (2.0f * q0q1 + _2q2q3 - pIMU->accelerometer.y) - 4.0f * pQ->x * (1 - 2.0f * q1q1 - 2.0f * q2q2 - pIMU->accelerometer.z) + _2bz * pQ->z * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - pIMU->magnetometer.x) + (_2bx * pQ->y + _2bz * pQ->w) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - pIMU->magnetometer.y) + (_2bx * pQ->z - _4bz * pQ->x) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - pIMU->magnetometer.z);
			s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - pIMU->accelerometer.x) + _2q3 * (2.0f * q0q1 + _2q2q3 - pIMU->accelerometer.y) - 4.0f * pQ->y * (1 - 2.0f * q1q1 - 2.0f * q2q2 - pIMU->accelerometer.z) + (-_4bx * pQ->y - _2bz * pQ->w) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - pIMU->magnetometer.x) + (_2bx * pQ->x + _2bz * pQ->z) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - pIMU->magnetometer.y) + (_2bx * pQ->w - _4bz * pQ->y) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - pIMU->magnetometer.z);
			s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - pIMU->accelerometer.x) + _2q2 * (2.0f * q0q1 + _2q2q3 - pIMU->accelerometer.y) + (-_4bx * pQ->z + _2bz * pQ->x) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - pIMU->magnetometer.x) + (-_2bx * pQ->w + _2bz * pQ->y) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - pIMU->magnetometer.y) + _2bx * pQ->x * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - pIMU->magnetometer.z);
		}
		else
		{
			// Gradient decent algorithm corrective step
			s0 = _4q0 * q2q2 + _2q2 * pIMU->accelerometer.x + _4q0 * q1q1 - _2q1 * pIMU->accelerometer.y;
			s1 = _4q1 * q3q3 - _2q3 * pIMU->accelerometer.x + 4.0f * q0q0 * pQ->x - _2q0 * pIMU->accelerometer.y - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * pIMU->accelerometer.z;
			s2 = 4.0f * q0q0 * pQ->y + _2q0 * pIMU->accelerometer.x + _4q2 * q3q3 - _2q3 * pIMU->accelerometer.y - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * pIMU->accelerometer.z;
			s3 = 4.0f * q1q1 * pQ->z - _2q1 * pIMU->accelerometer.x + 4.0f * q2q2 * pQ->z - _2q2 * pIMU->accelerometer.y;
		}
		recipNorm = OPTIMIZED_invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalize step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	pQ->w += qDot1 * (1.0f / ODR);
	pQ->x += qDot2 * (1.0f / ODR);
	pQ->y += qDot3 * (1.0f / ODR);
	pQ->z += qDot4 * (1.0f / ODR);

	// Normalize quaternion
	recipNorm = OPTIMIZED_invSqrt(pQ->w * pQ->w + pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z);
	pQ->w *= recipNorm;
	pQ->x *= recipNorm;
	pQ->y *= recipNorm;
	pQ->z *= recipNorm;

}




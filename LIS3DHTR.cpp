
// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// LIS3DHTR
// This code is designed to work with the LIS3DHTR_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=LIS3DHTR_I2CS#tabs-0-product_tabset-2
// And modified to work on the Bela Mini platfrom by Razvan Paisa
#include <Bela.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "LIS3DHTR.h"

LIS3DHTR::LIS3DHTR()
{
	bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, LIS3DHTR I2C address is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x19);

	// Select control register1(0x20)
	// X, Y and Z axis enabled, power on mode, data rate o/p 10Hz(27)
	config[2] = {0};
	config[0] = 0x20;
	config[1] = 0x27;
	write(file, config, 2);
	// Select control register4(0x23)
	// Full scale +/- 2g, continuous update(0x00)
	config[0] = 0x23;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data
	// lsb first
	// Read xAccl lsb data from register(0x28)
	reg[1] = {0x28};
	write(file, reg, 1);
	data[1] = {0};
	if(read(file, data, 1) != 1)
	{
		printf("Error : Input/Output error \n");
		exit(1);
	}
	data_0 = data[0];
}

void LIS3DHTR::process()
{
	// Read xAccl msb data from register(0x29)
	reg[0] = 0x29;
	write(file, reg, 1);
	read(file, data, 1);
	data_1 = data[0];

	// Read yAccl lsb data from register(0x2A)
	reg[0] = 0x2A;
	write(file, reg, 1);
	read(file, data, 1);
	data_2 = data[0];

	// Read yAccl msb data from register(0x2B)
	reg[0] = 0x2B;
	write(file, reg, 1);
	read(file, data, 1);
	data_3 = data[0];

	// Read zAccl lsb data from register(0x2C)
	reg[0] = 0x2C;
	write(file, reg, 1);
	read(file, data, 1);
	data_4 = data[0];

	// Read zAccl msb data from register(0x2D)
	reg[0] = 0x2D;
	write(file, reg, 1);
	read(file, data, 1);
	data_5 = data[0];

	// Convert the data
	xAccl = (data_1 * 256 + data_0);
	if(xAccl > 32767)
	{
		xAccl -= 65536;
	}

	yAccl = (data_3 * 256 + data_2);
	if(yAccl > 32767)
	{
		yAccl -= 65536;
	}

	zAccl = (data_5 * 256 + data_4);
	if(zAccl > 32767)
	{
	zAccl -= 65536;
	}

	// Output data to screen
	rt_printf("Acceleration in X-Axis : %d \n", xAccl);
	rt_printf("Acceleration in Y-Axis : %d \n", yAccl);
	rt_printf("Acceleration in Z-Axis : %d \n", zAccl);
}



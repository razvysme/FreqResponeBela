#pragma once

class LIS3DHTR
{
	public:
		int file;
		const char *bus;
		char config[3];
		char reg[2];
		char data[2];
		char data_0, data_1, data_2, data_3, data_4, data_5;
	
		int xAccl, yAccl, zAccl;
		void process();
		LIS3DHTR(); 
};

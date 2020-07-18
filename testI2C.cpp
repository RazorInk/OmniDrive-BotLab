#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>

#include <bcm2835.h>  

int main(int argc, char **argv)  
{  
	char buf[1];

	if (!bcm2835_init())return 1;  
	bcm2835_i2c_begin();                //Start I2C operations.
	bcm2835_i2c_setSlaveAddress(0x0F);  //I2C address
	bcm2835_i2c_set_baudrate(100000);   //1M baudrate
	std::cout << "i2c init done" << std::endl;

	buf[0] = 0xEF;    //LED ON
	bcm2835_i2c_write(buf,1);
	bcm2835_delay(500);
	std::cout << "first write done" << std::endl;
	buf[0] = 0xFF;     //LED OFF
	bcm2835_i2c_write(buf,1);
	bcm2835_delay(500);    
	bcm2835_i2c_end();  
	bcm2835_close();  
	return 0;  
} 

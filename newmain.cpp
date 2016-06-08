/* 
 * File:   newmain.cpp
 * Author: thomas.duppenthaler
 *
 * Created on 1. Juni 2016, 13:43
 */

#include "lamp.h"
#include <cstdlib>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{

   class mqtt_lamp *lamp;
	int rc;

	mosqpp::lib_init();

	lamp = new mqtt_lamp("lamp", "localhost", 1883);
	
	while(1){
		rc = lamp->loop();
		if(rc){
			lamp->reconnect();
		}
	}

	mosqpp::lib_cleanup();

	return 0;
}


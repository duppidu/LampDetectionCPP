/* 
 * File:   temperature_conversion.h
 * Author: thomas.duppenthaler
 *
 * Created on 1. Juni 2016, 13:44
 */

#ifndef LAMP_H
#define	LAMP_H



#include <mosquittopp.h>

class mqtt_lamp : public mosqpp::mosquittopp
{
	public:
		mqtt_lamp(const char *id, const char *host, int port);
		~mqtt_lamp();

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
};

#endif
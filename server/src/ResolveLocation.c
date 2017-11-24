/*
 * kalman.c
 *
 *  Created on: 23 nov 2017
 *      Author: alexs
 */


#include <math.h>
#include <stdint.h>
#include "../include/ResolveLocation.h"

float rssi2Power(float rssi)
{

	uint32_t powerCalibrator = -45; //dBm
	return (rssi + powerCalibrator);
}


float kalmanProcess(float rssiBuffer[NUMBER_SAMPLES])
{
	float Estimate[NUMBER_SAMPLES+1];
	float kalmanGain = 0;
	float initialErrorEstimate = INITIAL_ERROR_ESTIMATE;
	//float initialMeasurement;

	Estimate[0] = INITIAL_ESTIMATE;

	for (uint16_t i = 0; i < NUMBER_SAMPLES ; i++)
	{
		kalmanGain           = initialErrorEstimate / (initialErrorEstimate + INITIAL_ERROR_MEASUREMENT);
		Estimate[i + 1]      = Estimate[i] + (kalmanGain * (rssiBuffer[i] - Estimate[i]));
		initialErrorEstimate = (1 - kalmanGain) * initialErrorEstimate;
	}

	return Estimate[NUMBER_SAMPLES+1];
}



float power2distance(float Power_d)
{
	return (distance_o * pow((POWER_do - Power_d)/(10 * n_factor),10));
}


uint32_t trilateration_process(cord_t* nodePosition, struct co_ordinate_distance_tag trilaterationParams[TRILATERAT_NUMBER_NODES])
{
	float x_1 = trilaterationParams[0].position.co_ord[0];
	float y_1 = trilaterationParams[0].position.co_ord[1];
	float z_1 = trilaterationParams[0].position.co_ord[2];

	float x_2 = trilaterationParams[1].position.co_ord[0];
	float y_2 = trilaterationParams[1].position.co_ord[1];
	float z_2 = trilaterationParams[1].position.co_ord[2];

	float x_3 = trilaterationParams[2].position.co_ord[0];
	float y_3 = trilaterationParams[2].position.co_ord[1];
	float z_3 = trilaterationParams[2].position.co_ord[2];

	float d_1 = trilaterationParams[0].distance;
	float d_2 = trilaterationParams[0].distance;
	float d_3 = trilaterationParams[0].distance;

	float A = sqrt(x_1) + sqrt(y_1) - sqrt(d_1);
	float B = sqrt(x_2) + sqrt(y_2) - sqrt(d_2);
	float C = sqrt(x_3) + sqrt(y_3) - sqrt(d_3);

	float X_32 = (x_3 - x_2);
	float X_13 = (x_1 - x_3);
	float X_21 = (x_2 - x_1);
	float Y_32 = (y_3 - y_2);
	float Y_13 = (y_1 - y_3);
	float Y_21 = (y_2 - y_1);


	nodePosition->co_ord[0] = (A * Y_32 + B * Y_13 + C * Y_21) / (2 * (x_1 * Y_32 + x_2 * Y_13 + x_3 * Y_21));
	nodePosition->co_ord[1] = (A * X_32 + B * X_13 + C * X_21) / (2 * (y_1 * X_32 + y_2 * Y_13 + y_3 * X_21));
	nodePosition->co_ord[2] = (int)((trilaterationParams[0].position.co_ord[2] +  trilaterationParams[0].position.co_ord[2] + trilaterationParams[0].position.co_ord[2]) / 3);
}


cord_t * computeLocation(wifi_n *wifiNodeParams[TRILATERAT_NUMBER_NODES])
{
	uint32_t i;
	float filteredRssi;
	struct co_ordinate_distance_tag trilaterationParams[TRILATERAT_NUMBER_NODES];
	cord_t* nodePosition;


	// Call Kalman to get Filtered rssi

	for (i = 0; i < TRILATERAT_NUMBER_NODES; i++)
	{
		wifiNodeParams[i]->rssi  			 = kalmanProcess(wifiNodeParams[i]->rssiBuffer);
		wifiNodeParams[i]->power  			 = rssi2Power(wifiNodeParams[i]->rssi);
		wifiNodeParams[i]->cordinatesDistance.distance    = power2distance(wifiNodeParams[i]->power);
		trilaterationParams[i].distance	                 = wifiNodeParams[i]->cordinatesDistance.distance;

		for (uint32_t j = 0 ; j < CORDINATE_DIMENSION ; j++)
		{
			trilaterationParams[i].position.co_ord[j] = wifiNodeParams[i]->cordinatesDistance.position.co_ord[j];
		}

	}

	trilateration_process(nodePosition, trilaterationParams);

	return nodePosition;
}




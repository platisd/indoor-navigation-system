#ifndef WIFI_NODE_H
#define WIFI_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

#define SAMPLERSSIDATA 600
#define CARTESIANSIZE 3
#define CORDINATE_DIMENSION 3

#define NO_FLOORS 3

#define TRILATERAT_NUMBER_NODES 3
#define MAXIMUM_NUMBER_NODES 15
#define DEV_NAME 64

#define SAMPLING_FREQUENCY 10
#define SAMPLING_TIME 1
#define NUMBER_SAMPLES 600

#define INITIAL_ERROR_ESTIMATE 20
#define INITIAL_ERROR_MEASUREMENT 5
#define INITIAL_ESTIMATE -50
#define n_factor 12
#define POWER_do -20  //tobe caliberated
#define distance_o 1   //tobe determined by caliberation

#define LCFG_LEAF_STR "/WifiNodes/wifiFloor%d/wifiNodeBlock%d/"

typedef struct coordinates
{
	float co_ord[CARTESIANSIZE];
}cord_t;

typedef struct kalmanParams
{
	float initialErrorEstimate;
	float initialErrormeasurement;
	float initialpowerEstimate;

	float estimate;
	float kalmanGain;

}kalmanParams_t;

typedef struct pathLossParams_tag
{
	float nFactor;
	float powerdo;
	float doDistance;
	float powerd;
	float dDistance;
}pathLossParams_t;

typedef struct wifi_AccessPoint_Params_tag
{
	float   position[CARTESIANSIZE];
	char    *macAddress;
	float    distance;
	float    estReceivedPower;
	float    rssisampledata[NUMBER_SAMPLES];
	uint32_t noSampleData;
	uint32_t noProcessedSampleData;

	kalmanParams_t   wifiInitParams;
	pathLossParams_t pathLoss;

}mock_wifiParams_t;

typedef struct mock_insNode_tag
{
	uint32_t deviceNo;
	char   devName[DEV_NAME];
	char   macAddress[DEV_NAME];
	float nodeCartPosition[CARTESIANSIZE];
	mock_wifiParams_t wifiAccessPointNode[MAXIMUM_NUMBER_NODES];
	uint32_t wifiNo;
	void (* trilaterationProcess)(struct mock_insNode_tag * insNodeBlock);
	void (* rssi2PowerProcess)(struct mock_insNode_tag * insNodeBlock);
	void (* power2DistanceProcess)(struct mock_insNode_tag * insNodeBlock);
	void (* filterProcess)(mock_wifiParams_t * insNodeBlockWifi);
	void * next;
}mock_insNode_t;

mock_insNode_t *  createInsNodeListDevice(const char * deviceId);


#endif // WIFI_NODE_H

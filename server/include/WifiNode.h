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

}wifiParams_t;

typedef struct insNode_tag
{
	uint32_t deviceNo;
	char   devName[DEV_NAME];
	char   macAddress[DEV_NAME];
	float nodeCartPosition[CARTESIANSIZE];
	wifiParams_t wifiAccessPointNode[MAXIMUM_NUMBER_NODES];
	uint32_t wifiNo;
	void (* trilaterationProcess)(struct insNode_tag * insNodeBlock);
	void (* rssi2PowerProcess)(struct insNode_tag * insNodeBlock);
	void (* power2DistanceProcess)(struct insNode_tag * insNodeBlock);
	void (* filterProcess)(wifiParams_t * insNodeBlockWifi);
	void * next;
}insNode_t;

float * GetCartesianPosition(insNode_t * insNodeBlock);
insNode_t *  createInsNodeListDevice(const char * deviceId);
void computePLProcess(insNode_t * insNodeBlock);
void kalmanProcess(wifiParams_t * insNodeBlockWifi);
void rssi2Power(insNode_t * insNodeBlock);
void power2distance(insNode_t * insNodeBlock);
void orderRSSIAscend(insNode_t * insNodeBlock);
void trilateration_process(insNode_t * insNodeBlock);
insNode_t * InsNodeDefine(insNode_t * insNodeBlock, uint32_t deviceID, const char * deviceId);
uint32_t loadLCFGParams(wifiParams_t * wifiNodeBlock);
void initKalmanParams(wifiParams_t * wifiNodeBlock);
void findMacPath(char buff[128], const char * macaddress);
insNode_t * findWifiNode(insNode_t * insNoderoot, const char * deviceId);
insNode_t * destroyInsNode(insNode_t * insNoderoot);

#endif // WIFI_NODE_H

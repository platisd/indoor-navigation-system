/*************************************************************************************************************************
 * 			FILENAME :- WifiNode.h
 *
 * Description :- The module contains a group of functions to implement a trilateration algorithm with a simple 1D kalman filter
 * 					and a three circle problem solver.
 * 					It is expected that the positions of the wifinodes are already in the local config (.xml file). The main function
 * 					call here is the GetCartesianPosition() function.
 *
 * Author : Isaac Alex Sackey
 * Last edited: - 18th July 2018
 ************************************************************************************************************************/

#ifndef WIFI_NODE_H
#define WIFI_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

/************************************************************************************************************************
 *
 * 		CONSTANTS
 *
 ************************************************************************************************************************/
#define SAMPLERSSIDATA 4000
#define CARTESIANSIZE 3
#define CORDINATE_DIMENSION 3
#define NO_FLOORS 3
#define TRILATERAT_NUMBER_NODES 3
#define MAXIMUM_NUMBER_NODES 15
#define DEV_NAME 64
#define SAMPLING_FREQUENCY 10
#define SAMPLING_TIME 1
#define NUMBER_SAMPLES SAMPLERSSIDATA
#define INITIAL_ERROR_ESTIMATE 20
#define INITIAL_ERROR_MEASUREMENT 5
#define INITIAL_ESTIMATE -50
#define n_factor 12
#define POWER_do -20  //tobe caliberated
#define distance_o 1   //tobe determined by caliberation


/************************************************************************************************************************
 *
 * 		HELPFUL STRUCTURES
 *
 ************************************************************************************************************************/
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


/************************************************************************************************************************
 *
 * 		FUNCTIONS
 *
 ************************************************************************************************************************/


/************************************************************************************************************************
 *  Function          := GetCartesianPosition
 *  Description       :=
 *  					 This function takes a pointer to an insNode_t block and returns a 3 array floating point of that
 *  					 ins position. It is the main function of this module which is to be called by
 *  					 the localization module.
 *
 *  parameters input(s)  :=
 *  					    insNode_t *
 *  parameters output    :=
 *  					    float[3]
 ************************************************************************************************************************/
float * GetCartesianPosition(insNode_t * insNodeBlock);


/************************************************************************************************************************
 *  Function          := createInsNodeListDevice
 *  Description       :=
 *  					 This function takes a mac address as a device ID and adds an insinode block as part of the
 *  					 ins node list in memory.
 *
 *  parameters input(s)  :=
 *  					    device ID, which is in principle the mac address of the ins node.
 *  parameters output    :=
 *  					    pointer to the newly created insNode_t node block.
 ************************************************************************************************************************/
insNode_t *  createInsNodeListDevice(const char * deviceId);


/************************************************************************************************************************
 *  Function          := computePLProcess
 *  Description       :=
 *  					 This subroutine computes the distance of the ins node from all registered wifinodes it has logged
 *  					 in the database earlier . It applied the path loss equation P_l = (P_o) x 10^((d_o - P_r)/(10*n)).
 *
 *  parameters input(s)  :=
 *  					    insNode_t *.
 *  parameters output    :=
 *  					    void.
 ************************************************************************************************************************/
void computePLProcess(insNode_t * insNodeBlock);


/************************************************************************************************************************
 *  Function          := kalmanProcess
 *  Description       :=
 *  					 This process computes the kalman filtered estimated received signal strength value (rssi) by the
 *  					 provided series of rssi values by the pointer to insNodeBlock struct provided as argument.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void kalmanProcess(wifiParams_t * insNodeBlockWifi);



/************************************************************************************************************************
 *  Function          := rssi2power
 *  Description       :=
 *  					 This is a level shifter for wifinodes whose rssi values are not power levels.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void rssi2Power(insNode_t * insNodeBlock);


/************************************************************************************************************************
 *  Function          := power2distance
 *  Description       :=
 *  					 This function computes the distance of each wifi Node. It is a subroutine called from the computePLProcess()
 *  					 function. see :- void computePLProcess(insNode_t * insNodeBlock) above.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void power2distance(insNode_t * insNodeBlock);


/************************************************************************************************************************
 *  Function          := orderRSSIAscend
 *  Description       :=
 *  					 This process rearranges the wifiNode stacking in the insNodeBlock node block in ascending order of estimated
 *  					 power.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void orderRSSIAscend(insNode_t * insNodeBlock);


/************************************************************************************************************************
 *  Function          := trilateration_process
 *  Description       :=
 *  					 Solves the three circle problem by using the first three most signifcant estimated power values
 *  					 in the insNodeBlock structure.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void trilateration_process(insNode_t * insNodeBlock);


/************************************************************************************************************************
 *  Function          := InsNodeDefine
 *  Description       :=
 *  					 This function initializes each ins node block with the appriopriate device id, function callbacks,
 *  					 and initial kalman process parameters.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    pointer to the newly defined ins node block.
 ************************************************************************************************************************/
insNode_t * InsNodeDefine(insNode_t * insNodeBlock, uint32_t deviceID, const char * deviceId);


/************************************************************************************************************************
 *  Function          := loadLCFGParams
 *  Description       :=
 *  					 This function initializes the wifinode field in the insNodeBlock with the position
 *  					 of the wifi AP from localconfig.
 *
 *  parameters input(s)  :=
 *  					    pointer to wifiNodeBlock (wifiNodeBlock *).
 *  parameters output    :=
 *  					    returns a zero on success and a non zero otherwise.
 ************************************************************************************************************************/
uint32_t loadLCFGParams(wifiParams_t * wifiNodeBlock);


/************************************************************************************************************************
 *  Function          := initKalmanParams
 *  Description       :=
 *  					 This process intializes the wifiNode substructure of the insNodeBlock with the
 *  					  initial kalman filter values.
 *
 *  parameters input(s)  :=
 *  					    pointer to wifiNodeBlock (wifiNodeBlock *).
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void initKalmanParams(wifiParams_t * wifiNodeBlock);


/************************************************************************************************************************
 *  Function          := findMacPath
 *  Description       :=
 *  					 Loops through all localconfig params and returns the xml leave path to the provided mac address.
 *  					 the xml path is returned in the buff input argument.
 *
 *  parameters input(s)  :=
 *  					    macaddress :- macadress whose path in localconfig is to be found.
 *  					    buff       :- xml path of macaddress.
 *  parameters output    :=
 *  					    void returned.
 ************************************************************************************************************************/
void findMacPath(char buff[128], const char * macaddress);


/************************************************************************************************************************
 *  Function          := findWifiNode
 *  Description       :=
 *  					 This process locates an insNode given the macaddress from the insNode list.
 *
 *  parameters input(s)  :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 *  					    device ID :- macaddress of the insNode to be found.
 *  parameters output    :=
 *  					    pointer to the located insNodeBlock (insNodeBlock *).
 ************************************************************************************************************************/
insNode_t * findWifiNode(insNode_t * insNoderoot, const char * deviceId);


/************************************************************************************************************************
 *  Function          := destroyInsNode
 *  Description       :=
 *  					 Cleanup insNodeBlock after localization process.
 *
 *  parameters input(s)  :=
 *  					    pointer to cleaned insNodeBlock (insNodeBlock *).
 *  parameters output    :=
 *  					    pointer to insNodeBlock (insNodeBlock *).
 ************************************************************************************************************************/
insNode_t * destroyInsNode(insNode_t * insNoderoot);

#endif // WIFI_NODE_H

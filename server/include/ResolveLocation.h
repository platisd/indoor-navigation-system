#include <stdio.h>


#define CORDINATE_DIMENSION 3

#define TRILATERAT_NUMBER_NODES 3

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
	float co_ord[3];
}cord_t;

typedef struct co_ordinate_distance_tag
{
	cord_t position;
	float distance;
}cordDist_t;

typedef struct wifiNodeTable
{
	cordDist_t cordinatesDistance;
	float    power;
	char     MACAddress[30];
	float    rssi;
	float 	 rssiBuffer[NUMBER_SAMPLES];
}wifi_n;

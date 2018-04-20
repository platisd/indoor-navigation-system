#include <WifiNode.h>
#include <WifiAccessPointLocalConfig.h>

insNode_t insNoderootP;
insNode_t * insNoderoot = &insNoderootP;

float * GetCartesianPosition(insNode_t * insNodeBlock)
{
	float * cartPosition;
	struct timespec before,after;
	long delta;

	clock_gettime(CLOCK_MONOTONIC_RAW, &before);

	computePLProcess(insNodeBlock);

	insNodeBlock->trilaterationProcess(insNodeBlock);

	clock_gettime(CLOCK_MONOTONIC_RAW, &after);
	delta = 1000000000*(after.tv_sec - before.tv_sec) + after.tv_nsec - before.tv_nsec;
	printf("[%s] - RSSI processing and positioning took: %ld ns\n", __func__, delta);

       destroyInsNode(insNodeBlock);

	return insNodeBlock->nodeCartPosition;
}

insNode_t *  createInsNodeListDevice(const char * deviceId)
{
	uint32_t count = 0;

	insNode_t * looper = insNoderoot , * insNode = NULL;

	if (looper != 0)
	{
		while ((looper->next != NULL) && (strcmp(deviceId,looper->devName)))
		{
				looper = looper->next;
				count++;
		}

		if (strcmp(deviceId,looper->devName))
		{
			looper->next = insNode = (insNode_t *)calloc(1,sizeof(insNode_t));
			looper = (insNode_t *)looper->next;
			InsNodeDefine(looper,count,deviceId);
			printf("[%s] New WifiNode Device Block Created ID: %s!! \n",__func__,deviceId);
		}
		else
		{
			printf("[%s] The WifiNode Device Block: %s Already exists!! \n",__func__,deviceId);
			return NULL;
		}
	}
	else
	{
		printf("WifiNode Device creation failed!! \n");
	}
	return insNode;
}

void computePLProcess(insNode_t * insNodeBlock)
{
	uint32_t j = 0;
	uint32_t i = 0;

	for (j = 0; j < MAXIMUM_NUMBER_NODES; j++)
	{
		for (i = 0; i < insNodeBlock->wifiAccessPointNode[j].noSampleData; i++)
		{
			insNodeBlock->filterProcess(&insNodeBlock->wifiAccessPointNode[j]);
		}
	}

	insNodeBlock->rssi2PowerProcess(insNodeBlock);
	insNodeBlock->power2DistanceProcess(insNodeBlock);
}

void kalmanProcess(wifiParams_t * insNodeBlockWifi)
{
	insNodeBlockWifi->wifiInitParams.kalmanGain          = insNodeBlockWifi->wifiInitParams.initialErrorEstimate / (insNodeBlockWifi->wifiInitParams.initialErrorEstimate + (float)INITIAL_ERROR_MEASUREMENT);
	insNodeBlockWifi->estReceivedPower      +=  (insNodeBlockWifi->wifiInitParams.kalmanGain * (insNodeBlockWifi->rssisampledata[insNodeBlockWifi->noProcessedSampleData] - insNodeBlockWifi->estReceivedPower));
	insNodeBlockWifi->wifiInitParams.initialErrorEstimate = (1 - insNodeBlockWifi->wifiInitParams.kalmanGain) * insNodeBlockWifi->wifiInitParams.initialErrorEstimate;

	insNodeBlockWifi->noProcessedSampleData++;
}

void rssi2Power(insNode_t * insNodeBlock)
{
	float powerShifter = 0.0f; //dBm

	uint32_t j = 0;

	for (j = 0; j < MAXIMUM_NUMBER_NODES; j++)
	{
		insNodeBlock->wifiAccessPointNode[j].estReceivedPower += powerShifter;
	}
}

void power2distance(insNode_t * insNodeBlock)
{
	uint32_t j = 0;

	for (wifiParams_t * wifi = insNodeBlock->wifiAccessPointNode; wifi->macAddress ; wifi++ )
	{
		wifi->pathLoss.nFactor = (wifi->pathLoss.powerdo - wifi->pathLoss.powerd) / (float)(10 * (log10(wifi->pathLoss.dDistance / wifi->pathLoss.doDistance)));

		wifi->distance = (wifi->pathLoss.doDistance * powf(  10, ((float)(wifi->pathLoss.powerdo - wifi->estReceivedPower ))/((float) (10 * wifi->pathLoss.nFactor))  )  ); // 10, ((float)(insNodeBlock->wifiAccessPointNode[j].pathLoss.powerdo - insNodeBlock->wifiAccessPointNode[j].pathLoss.powerd ))/((float) (10 * insNodeBlock->wifiAccessPointNode[j].pathLoss.nFactor))

		//printf("[%s] wifiMacaddress: %s, wifidistance:: %f  Estimated power: %f, d0: %f , powerdo: %f, powerd: %f, nfactor: %f\n",__func__,insNodeBlock->wifiAccessPointNode[j].macAddress,insNodeBlock->wifiAccessPointNode[j].distance,insNodeBlock->wifiAccessPointNode[j].estReceivedPower,insNodeBlock->wifiAccessPointNode[j].pathLoss.doDistance,insNodeBlock->wifiAccessPointNode[j].pathLoss.powerdo,insNodeBlock->wifiAccessPointNode[j].pathLoss.powerd,insNodeBlock->wifiAccessPointNode[j].pathLoss.nFactor);
	}

}

void orderRSSIAscend(insNode_t * insNodeBlock)
{
	uint32_t idx = 0;
	uint32_t max;
	uint32_t i,j,k;
	wifiParams_t tempTotalwifi[MAXIMUM_NUMBER_NODES];
	wifiParams_t tempwifi;

	memcpy(tempTotalwifi,insNodeBlock->wifiAccessPointNode,sizeof(wifiParams_t)*MAXIMUM_NUMBER_NODES);

	for (i = 0; i < MAXIMUM_NUMBER_NODES; i++)
	{
		for (j = i + 1; j < MAXIMUM_NUMBER_NODES; j++)
		{
			if (((tempTotalwifi[i].distance > tempTotalwifi[j].distance) && (tempTotalwifi[j].distance > 0.1)) || (tempTotalwifi[i].distance < 0.1))
			{
				tempwifi = tempTotalwifi[i];

				tempTotalwifi[i] = tempTotalwifi[j];

				tempTotalwifi[j] = tempwifi;
			}
		}
	}

	memcpy(insNodeBlock->wifiAccessPointNode,tempTotalwifi,sizeof(wifiParams_t)*MAXIMUM_NUMBER_NODES);
}

void trilateration_process(insNode_t * insNodeBlock)
{
	orderRSSIAscend(insNodeBlock);

	if (insNodeBlock != NULL)
	{

		float x_1 = insNodeBlock->wifiAccessPointNode[0].position[0];
		float y_1 = insNodeBlock->wifiAccessPointNode[0].position[1];
		float z_1 = insNodeBlock->wifiAccessPointNode[0].position[2];

		float x_2 = insNodeBlock->wifiAccessPointNode[1].position[0];
		float y_2 = insNodeBlock->wifiAccessPointNode[1].position[1];
		float z_2 = insNodeBlock->wifiAccessPointNode[1].position[2];

		float x_3 = insNodeBlock->wifiAccessPointNode[2].position[0];
		float y_3 = insNodeBlock->wifiAccessPointNode[2].position[1];
		float z_3 = insNodeBlock->wifiAccessPointNode[2].position[2];

		float d_1 = insNodeBlock->wifiAccessPointNode[0].distance;
		float d_2 = insNodeBlock->wifiAccessPointNode[1].distance;
		float d_3 = insNodeBlock->wifiAccessPointNode[2].distance;

		//
		//printf("AP 1: x: %f, y: %f, z: %f, ... AP 2: x: %f, y: %f, z: %f, ... AP 3: x: %f, y: %f, z: %f \n",x_1,y_1,z_1,x_2,y_2,z_2,x_3,y_3,z_3);
		//printf("AP distance 1: %f ... AP distance 2: %f ... AP distance 3: %f \n",d_1,d_2,d_3);

		float A = (x_1 * x_1) + (y_1 * y_1) - (d_1 * d_1);
		float B = (x_2 * x_2) + (y_2 * y_2) - (d_2 * d_2);
		float C = (x_3 * x_3) + (y_3 * y_3) - (d_3 * d_3);

		float X_32 = (x_3 - x_2);
		float X_13 = (x_1 - x_3);
		float X_21 = (x_2 - x_1);
		float Y_32 = (y_3 - y_2);
		float Y_13 = (y_1 - y_3);
		float Y_21 = (y_2 - y_1);


		insNodeBlock->nodeCartPosition[0] = (A * Y_32 + B * Y_13 + C * Y_21) / (2 * (x_1 * Y_32 + x_2 * Y_13 + x_3 * Y_21));
		insNodeBlock->nodeCartPosition[1] = (A * X_32 + B * X_13 + C * X_21) / (2 * (y_1 * X_32 + y_2 * X_13 + y_3 * X_21));
		insNodeBlock->nodeCartPosition[2] = (round((z_1 +  z_2 + z_3) / (float)NO_FLOORS));

		//printf("Node position 1: %f. node  position2: %f, node position3: %f",insNodeBlock->nodeCartPosition[0],insNodeBlock->nodeCartPosition[1],insNodeBlock->nodeCartPosition[2]);
	}
	else
	{
		printf("Invalid NodeBlock!");
	}
}

insNode_t * InsNodeDefine(insNode_t * insNodeBlock, uint32_t deviceID, const char * deviceId)
{
	// create looper-root first
	uint32_t ret, i;

	memset(insNodeBlock, 0, sizeof(insNode_t));

	strcat(insNodeBlock->macAddress,deviceId);
	strcat(insNodeBlock->devName,deviceId);
	insNodeBlock->power2DistanceProcess = power2distance;
	insNodeBlock->filterProcess = kalmanProcess;
	insNodeBlock->rssi2PowerProcess = rssi2Power;
	insNodeBlock->trilaterationProcess = trilateration_process;
	insNodeBlock->next = NULL;
	insNodeBlock->deviceNo = deviceID;

	//init kalman Params;
	for (i = 0; i < MAXIMUM_NUMBER_NODES; i++)
	{
		initKalmanParams(&insNodeBlock->wifiAccessPointNode[i]);
	}

	return insNodeBlock;
}

void initKalmanParams(wifiParams_t * wifiNodeBlock)
{
	wifiNodeBlock->noSampleData = NUMBER_SAMPLES;

	wifiNodeBlock->estReceivedPower = wifiNodeBlock->wifiInitParams.initialpowerEstimate = INITIAL_ESTIMATE ;

	wifiNodeBlock->wifiInitParams.kalmanGain = 0;

	wifiNodeBlock->wifiInitParams.initialErrorEstimate = INITIAL_ERROR_ESTIMATE;

	wifiNodeBlock->pathLoss.doDistance = 1.0f;

	wifiNodeBlock->noProcessedSampleData = 0;

	//strcpy(wifiNodeBlock->macAddress,"ff:ff:ff:ff:ff:ff");
}


uint32_t loadLCFGParams(wifiParams_t * wifiNodeBlock)  // fill macadresses before calling this function
{
	int32_t ret = 0;
	uint32_t k;
	char buff[128],buffParams[128];
	char leaves[8][24] = {"_3DPosition/x","_3DPosition/y","_3DPosition/z","arbitraryDistance","powerTransmit","powerAtArbitraryDistance","macAddress"};

	findMacPath(buffParams,wifiNodeBlock->macAddress);

	if (buff != NULL) //change check
	{
		for (k = 0; k < 3 ; k++)
		{
			snprintf(buff,128,"%s%s",buffParams,leaves[k]);
			ret += lcfg_getFloatParameter(buff,&wifiNodeBlock->position[k]);
		}

		snprintf(buff,128,"%s%s",buffParams,leaves[k]); k++;

		if ((ret += lcfg_getFloatParameter(buff,&wifiNodeBlock->pathLoss.dDistance)) != -1)
		{
			snprintf(buff,128,"%s%s",buffParams,leaves[k]); k++;

			if ((ret += lcfg_getFloatParameter(buff,&wifiNodeBlock->pathLoss.powerdo )) != -1)
			{
				snprintf(buff,128,"%s%s",buffParams,leaves[k]); k++;

				if ((ret += lcfg_getFloatParameter(buff,&wifiNodeBlock->pathLoss.powerd)) != -1)
				{
					snprintf(buff,128,"%s%s",buffParams,leaves[k]); k++;

					wifiNodeBlock->macAddress = lcfg_getStringParameter(buff);

				}
			}
		}
	}


	float x_1 = wifiNodeBlock->position[0];
	float y_1 = wifiNodeBlock->position[1];
	float z_1 = wifiNodeBlock->position[2];

	return ret;
}

void findMacPath(char * buff, const char * macaddress)
{
	char buffMac[128];
	char * temp;
	uint32_t i;
	uint32_t j;

	for (i = 1; i <= NO_FLOORS; i++)
	{
		for (j = 1; j <= MAXIMUM_NUMBER_NODES; j++)
		{
			sprintf(buffMac, LCFG_LEAF_STR,i,j);
			strcat(buffMac,"macAddress");

			temp = lcfg_getStringParameter(buffMac);

			if (!strcmp(temp,macaddress))
			{
				lcfg_freeStringParameter(temp);
				sprintf(buff, LCFG_LEAF_STR,i,j);

				return;
			}
		}
	}
	printf("[%s] No path found in Local Config for wifiNode:: %s \n",__func__,macaddress);
	buff = NULL;
}

insNode_t * findWifiNode(insNode_t * insNoderoot, const char * deviceId)
{
	insNode_t * looper = (insNode_t *)insNoderoot;

	while (looper != NULL)
	{
		if (!strcmp(looper->devName,deviceId))
		{
			return looper;
			break;
		}
		else
		{
			looper = looper->next;
		}
	}

	//printf("Could not find device: %s!!\n", deviceId);
	return NULL;
}

insNode_t * destroyInsNode(insNode_t * insNode)
{
	int i;
	//memset(insNode->devName , '0', sizeof(char)*DEV_NAME);
	for ( i = 0; i < MAXIMUM_NUMBER_NODES; ++i)
	{
		insNode->wifiAccessPointNode[i].distance = 0.0f;
		insNode->wifiAccessPointNode[i].estReceivedPower = 0.0f;
		insNode->wifiAccessPointNode[i].macAddress = NULL;
		insNode->wifiAccessPointNode[i].noSampleData = 0;
		memset(insNode->wifiAccessPointNode[i].rssisampledata,0,sizeof(float)*NUMBER_SAMPLES);
		insNode->wifiAccessPointNode[i].noProcessedSampleData = 0.0f;
	}
	return insNode;
}

/*************************************************************************************************************************
 * 			FILENAME :- WifiAccessPointLocalConfig.h
 *
 * Description :- The module contains functions needed to initialize lcfg, collect and set leave and node parameter values from the local config xml file.
 *
 *
 * Author : Isaac Alex Sackey
 * Last edited: - 18th July 2018
 ************************************************************************************************************************/
#ifndef WIFI_NODE_ACCESS_POINT_LOCAL_CONFIG_H
#define WIFI_NODE_ACCESS_POINT_LOCAL_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xmlschemas.h>
#include <libxml/xpointer.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/************************************************************************************************************************
 *     GLOBAL VARIABLES
 ************************************************************************************************************************/
static pthread_mutex_t lock;
static xmlNode *firstRootChild = NULL;


/************************************************************************************************************************
 *     DEFINITION VARIABLES VARIABLES
 ************************************************************************************************************************/
#define LCFG_LEAF_STR "/WifiNodes/wifiFloor%d/wifiNodeBlock%d/"


/************************************************************************************************************************
 *  Function          := findNode
 *  Description       :=
 *  					 Find a node in the xml lib node list that corresponds to a particular path.
 *
 *  parameters input(s)  :=
 *  					    path whose node in the node list is not be found.
 *  parameters output    :=
 *  					    pointer to the corresponding xml Node in the nodelist.
 ************************************************************************************************************************/
static xmlNode * findNode(const char *path);

/************************************************************************************************************************
 *  Function          := lcfg_getFloatParameter | lcfg_getInt32Parameter
 *  Description       :=
 *  					 Fetch float/integer parameter from the provided xml path.
 *
 *  parameters input(s)  :=
 *  					    path whose node in the node list is not be found.
 *  					    pointer to value to the fetched.
 *  parameters output    :=
 *  					    0 on success non zero on failure.
 ************************************************************************************************************************/
int32_t lcfg_getFloatParameter(const char *path,float *value);
int32_t lcfg_getInt32Parameter(const char *path, int32_t *value);


/************************************************************************************************************************
 *  Function          := lcfg_getStringParameter
 *  Description       :=
 *  					 Fetch string parameter from the provided path. Do lcfg_freeStringParameter() on input pointer to parameter input after this
 *  					 function.
 *
 *  parameters input(s)  :=
 *  					    path to string.
 *  parameters output    :=
 *  					    void.
 ************************************************************************************************************************/
char * lcfg_getStringParameter(const char *path);


/************************************************************************************************************************
 *  Function          := lcfg_setStringParameter
 *  Description       :=
 *  					 Set string parameter provided in input parameter content in the xml nodelist in memory.
 *  					 Note this does not update the xml file itself.
 *
 *  parameters input(s)  :=
 *  					    path :- path to string to be updated.
 *  					    content :- string content to be updated.
 *  parameters output    :=
 *  					    0 upon success and non zero otherwise.
 ************************************************************************************************************************/
int32_t lcfg_setStringParameter(const char *path, const char *content);


/************************************************************************************************************************
 *  Function          := lcfg_freeStringParameter
 *  Description       :=
 *  					 Free memory allocated by lcfg_getStringParameter() function.
 *
 *  parameters input(s)  :=
 *  					    parameter :- path to string.
 *  parameters output    :=
 *  					    void.
 ************************************************************************************************************************/
void lcfg_freeStringParameter(char *parameter);



/************************************************************************************************************************
 *  Function          := lcfg_initialize
 *  Description       :=
 *  					 Load all xml data into a nodelist in RAM memory.
 *
 *  parameters input(s)  :=
 *  					    file containing xml data.
 *  parameters output    :=
 *  					    0 on success nonzero on failure.
 ************************************************************************************************************************/
int32_t lcfg_initialize(const char* lcfg_file);

#endif //WIFI_NODE_ACCESS_POINT_LOCAL_CONFIG_H

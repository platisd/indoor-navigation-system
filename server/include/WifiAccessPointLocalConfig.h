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

static pthread_mutex_t lock;
static xmlNode *firstRootChild = NULL;

#define LCFG_LEAF_STR "/WifiNodes/wifiFloor%d/wifiNodeBlock%d/"

static xmlNode * findNode(const char *path);
int32_t lcfg_getFloatParameter(const char *path,float *value);
int32_t lcfg_getInt32Parameter(const char *path, int32_t *value);
char * lcfg_getStringParameter(const char *path);
int32_t lcfg_setStringParameter(const char *path, const char *content);
void lcfg_freeStringParameter(char *parameter);
int32_t lcfg_initialize(const char* lcfg_file);

#endif //WIFI_NODE_ACCESS_POINT_LOCAL_CONFIG_H

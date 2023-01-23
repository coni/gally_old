#include <string.h>
#include <stdlib.h>

#include "cjson/cJSON.h"

char* mc_GetMainclass(cJSON* manifest)
{
	cJSON* jsonMainclass = cJSON_GetObjectItemCaseSensitive(manifest, "mainClass");
    size_t len_mainclass = strlen(jsonMainclass->valuestring)+1;
	char* mainclass = malloc(sizeof(char) * len_mainclass);
    strcpy(mainclass, jsonMainclass->valuestring);
	return mainclass;
}

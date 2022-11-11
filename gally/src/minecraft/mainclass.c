#include <string.h>
#include <stdlib.h>

#include "../cJSON/cJSON.h"

char* mc_GetMainclass(cJSON* manifest)
{
	char* mainclass = NULL;
	cJSON* jsonMainclass = cJSON_GetObjectItemCaseSensitive(manifest, "mainClass");
	mainclass = malloc(strlen(jsonMainclass->valuestring));
	strcpy(mainclass, jsonMainclass->valuestring);
	return mainclass;
}

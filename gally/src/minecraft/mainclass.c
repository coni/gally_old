#include <string.h>
#include <stdlib.h>

#include "../cJSON/cJSON.h"

char* mc_GetMainclass(cJSON* manifest)
{
	cJSON* jsonMainclass = cJSON_GetObjectItemCaseSensitive(manifest, "mainClass");
	char* mainclass = malloc(strlen(jsonMainclass->valuestring));
	if (mainclass == NULL)
		return NULL;
	strcpy(mainclass, jsonMainclass->valuestring);
	return mainclass;
}

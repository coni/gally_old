#include <string.h>
#include <stdlib.h>

#include "../cjson/cJSON.h"

char* mc_GetMainclass(cJSON* manifest)
{
	cJSON* jsonMainclass = cJSON_GetObjectItemCaseSensitive(manifest, "mainClass");
	char* mainclass = jsonMainclass->valuestring;
    free(jsonMainclass);
	return mainclass;
}

#ifdef _WIN32
#include <cjson/cJSON.h>
#include <utils.h>
#endif

#ifdef __unix__
#include "../cjson/cJSON.h"
#include "../utils.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cJSON *mc_GetMainManifest(char *path)
{
	size_t len_fullpath = (strlen(path) + 24) * sizeof(char*);
	char *fullpath = malloc(len_fullpath);

	if (fullpath != NULL)
	{
		snprintf(fullpath, len_fullpath, "%s/version_manifest_v2.json", path);
		http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", fullpath);
		return json_ParseFile(fullpath);
	}

	return NULL;
}

char *mc_GetInherit(cJSON *manifest)
{
	char * version = NULL;
	manifest = cJSON_GetObjectItemCaseSensitive(manifest, "inheritsFrom");
	if (manifest)
		version = manifest->valuestring;
	return version;
}

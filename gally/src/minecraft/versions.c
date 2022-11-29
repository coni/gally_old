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

cJSON* mc_GetMainManifest(char* path)
{
    cJSON* manifest = NULL;
	size_t len_fullpath = (strlen(path) + 25)*  sizeof(char*);
	char* fullpath = malloc(len_fullpath);

	if (fullpath != NULL)
	{
		snprintf(fullpath, len_fullpath, "%s/version_manifest_v2.json", path);
		http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", fullpath);
		manifest = json_ParseFile(fullpath);
	}

    free(fullpath);
	return manifest;
}

char* mc_GetInherit(cJSON* manifest)
{
	manifest = cJSON_GetObjectItemCaseSensitive(manifest, "inheritsFrom");
	if (manifest)
		return manifest->valuestring;
	return NULL;
}

cJSON* mc_GetManifest(cJSON* versionManifest, char* path, char* version)
{
    cJSON* manifest = NULL;
    cJSON* versions = NULL;
    cJSON* versionInfo  = NULL;
    cJSON* id = NULL;
    cJSON* url= NULL;

	size_t len_fullpath = (strlen(path) + strlen(version)*2 + 8);
    char* fullpath = malloc(len_fullpath*sizeof(char*));

    if (fullpath == NULL)
        return NULL;
    snprintf(fullpath, len_fullpath, "%s/%s/%s.json", path, version, version);

    versions = cJSON_GetObjectItemCaseSensitive(versionManifest, "versions");
    cJSON_ArrayForEach(versionInfo, versions)
    {
        id = cJSON_GetObjectItemCaseSensitive(versionInfo, "id");
        url = cJSON_GetObjectItemCaseSensitive(versionInfo, "url");
        if (strcmp(id->valuestring,version) == 0)
        {
            http_Download(url->valuestring, fullpath);
        }
    }

    manifest = json_ParseFile(fullpath);

    free(fullpath);

    return manifest;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "minecraft/versions.h"
#include "launcher.h"
#include "cjson/cJSON.h"

int mc_GetClientSize(cJSON* manifest)
{
    int total_size = 0;
	cJSON* tmp = cJSON_GetObjectItemCaseSensitive(manifest, "downloads");
	if (tmp)
	{
		tmp = cJSON_GetObjectItemCaseSensitive(tmp, "client");
        cJSON* size = cJSON_GetObjectItemCaseSensitive(tmp, "size");
        total_size += size->valueint;
    }
    return total_size;
}

int mc_GetClientSizeVersion(char* version, GamePath gamePath)
{
    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);
    int size = mc_GetClientSize(manifest);
    cJSON_Delete(mainManifest);
    cJSON_Delete(manifest);
    return size;
}

char* mc_DownloadClient(cJSON* manifest, GamePath gamePath, char* version)
{
    char* versionPath = gamePath.version;
    size_t len_path;
	char* path = NULL;
    char* url = NULL;
    cJSON* temp = NULL;
	cJSON* jsonVersion = cJSON_GetObjectItemCaseSensitive(manifest, "downloads");

	if (jsonVersion)
	{
		jsonVersion = cJSON_GetObjectItemCaseSensitive(jsonVersion, "client");
		temp = cJSON_GetObjectItemCaseSensitive(manifest, "id");

		temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "url");
		url = malloc((strlen(temp->valuestring) + 1) * sizeof(char));
		if (url == NULL)
			return NULL;
		strcpy(url, temp->valuestring);

		len_path = (strlen(versionPath) + (strlen(version) * 2) + 7);
		path = malloc(len_path * sizeof(char));
		if (path == NULL)
			return NULL;

		snprintf(path, len_path, "%s/%s/%s.jar", versionPath, version, version);
		http_Download(url, path);

        free(path);
        free(url);
	}


    if (path == NULL)
    {
        size_t len_path = strlen(gamePath.version) + (strlen(version) * 2) + 7;
        path = malloc(sizeof(char) * len_path);
        snprintf(path, len_path, "%s/%s/%s.jar", gamePath.version, version, version);
    }

	return path;
}

char* mc_GetType(cJSON* manifest)
{
	manifest = cJSON_GetObjectItemCaseSensitive(manifest, "type");
	if (manifest)
        return manifest->valuestring;
	return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"
#include "../cjson/cJSON.h"

char* mc_DownloadClient(cJSON* manifest, char* versionPath,  char* version)
{

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
		url = malloc((strlen(temp->valuestring)) * sizeof(char));
		if (url == NULL)
			return NULL;
		strcpy(url, temp->valuestring);

		len_path = (strlen(versionPath) + (strlen(version) * 2) + 7);
		path = malloc(len_path * sizeof(char));
		if (path == NULL)
			return NULL;

		snprintf(path, len_path, "%s/%s/%s.jar", versionPath, version, version);
		http_Download(url, path);
	}

    free(temp);
    free(jsonVersion);
    free(url);
	return path;
}

char* mc_GetType(cJSON* manifest)
{
	manifest = cJSON_GetObjectItemCaseSensitive(manifest, "type");
	if (manifest)
        return manifest->valuestring;
	return NULL;
}

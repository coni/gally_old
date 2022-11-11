#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"
#include "../cjson/cJSON.h"

char* mc_DownloadClient(cJSON* manifest, char* versionPath,  char* version)
{

	char* path = NULL;
	cJSON* jsonVersion = cJSON_GetObjectItemCaseSensitive(manifest, "downloads");

	if (jsonVersion)
	{
		jsonVersion = cJSON_GetObjectItemCaseSensitive(jsonVersion, "client");
		cJSON* temp = cJSON_GetObjectItemCaseSensitive(manifest, "id");

		temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "url");
		char* url = malloc((strlen(temp->valuestring)) * sizeof(char));
		if (url == NULL)
			return NULL;
		strcpy(url, temp->valuestring);

		/*
		temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "sha1");
		char* sha1 = malloc((strlen(temp->valuestring) + 1)*  sizeof(char));
		strncpy(sha1, (strlen(temp->valuestring) + 1), temp->valuestring);
		*/

		size_t len_path = (strlen(versionPath) + (strlen(version) * 2) + 7);
		path = malloc(len_path * sizeof(char));
		if (path == NULL)
			return NULL;

		snprintf(path, len_path, "%s/%s/%s.jar", versionPath, version, version);
		http_Download(url, path);
	}
	return path;
}

char* mc_GetType(cJSON* manifest)
{
	char* type = NULL;
	cJSON* typeJson = cJSON_GetObjectItemCaseSensitive(manifest, "type");
	if (typeJson)
		type = typeJson->valuestring;
	return type;
}

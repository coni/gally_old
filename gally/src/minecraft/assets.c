#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "minecraft/versions.h"
#include "launcher.h"
#include "utils.h"
#include "cjson/cJSON.h"

char* mc_GetAssetIndex(cJSON *manifest)
{
	char* assetIndex = NULL;
	cJSON *index = cJSON_GetObjectItemCaseSensitive(manifest, "assetIndex");
	if (index)
	{
		index = cJSON_GetObjectItemCaseSensitive(index, "id");
		if (index)
		{
			assetIndex = malloc((strlen(index->valuestring)+1) * sizeof(char));
			strcpy(assetIndex, index->valuestring);
		}
	}
	return assetIndex;
}

cJSON* mc_GetAssetsManifest(cJSON *manifest, GamePath gamePath)
{
    char* path = gamePath.assets;
    cJSON *i = cJSON_GetObjectItemCaseSensitive(manifest, "assetIndex");
    cJSON *assetsManifest = NULL;
    size_t len_indexesPath = (strlen(path) + 9);
	char * indexesPath = malloc(len_indexesPath * sizeof(char));
    snprintf(indexesPath, len_indexesPath, "%s/indexes", path);

	char * fullpath = NULL;
    size_t len_fullpath = 0;
	if (i)
	{
        cJSON *url = cJSON_GetObjectItemCaseSensitive(i, "url");
        cJSON *index = cJSON_GetObjectItemCaseSensitive(i, "id");
        len_fullpath = (strlen(indexesPath) + strlen(index->valuestring) + 7);
        fullpath = malloc(len_fullpath * sizeof(char));
        snprintf(fullpath, len_fullpath, "%s/%s.json", indexesPath, index->valuestring);
        http_Download(url->valuestring, fullpath);
		assetsManifest = json_ParseFile(fullpath);
	}

    return assetsManifest;
}

int mc_DownloadAssets(cJSON *assetsManifest, GamePath gamePath)
{
    char* assetsPath = gamePath.assets;
    size_t len_ressourceUrl = 40;
	char ressourceUrl[] = "https://resources.download.minecraft.net";

    size_t len_fullpath = 0;
    size_t len_url = 0;
    size_t len_path = (strlen(assetsPath) + 9);
	char * path = malloc(len_path * sizeof(char));
    snprintf(path, len_path, "%s/objects/", assetsPath);

	cJSON* i = NULL;
	cJSON* tmp = cJSON_GetObjectItemCaseSensitive(assetsManifest, "objects");
	if (tmp)
	{
		cJSON_ArrayForEach(i, tmp)
		{
			cJSON *hash = cJSON_GetObjectItemCaseSensitive(i, "hash");
			char folder[3];
			strcpy(folder, "");
			folder[0] = hash->valuestring[0];
			folder[1] = hash->valuestring[1];

            len_url = len_ressourceUrl + strlen(hash->valuestring) + 5;
			char *url = malloc(len_url * sizeof(char));
            snprintf(url, len_url, "%s/%s/%s", ressourceUrl, folder, hash->valuestring);

            len_fullpath = len_path + strlen(hash->valuestring) + 5;
			char* fullpath = malloc(len_fullpath * sizeof(char));
            snprintf(fullpath, len_fullpath, "%s/%s/%s", path, folder, hash->valuestring);

			http_Download(url, fullpath);
            free(url);
            free(fullpath);
		}
	}

    cJSON_Delete(assetsManifest);
	return 0;
}

int mc_GetAssetsSize(cJSON *manifest)
{
	cJSON* i = NULL;
	cJSON* tmp = cJSON_GetObjectItemCaseSensitive(manifest, "objects");
    int total_size = 0;
	if (tmp)
	{
		cJSON_ArrayForEach(i, tmp)
		{
			cJSON *size = cJSON_GetObjectItemCaseSensitive(i, "size");
            total_size += size->valueint;
		}
	}

	return total_size;
}

int mc_GetAssetsSizeVersion(char* version, GamePath gamePath)
{
    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);
    cJSON* assetsManifest = mc_GetAssetsManifest(manifest, gamePath);
    return mc_GetAssetsSize(assetsManifest);
}

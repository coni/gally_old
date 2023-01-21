#ifdef _WIN32
#include <cjson/cJSON.h>
#include <utils.h>
#endif

#ifdef __unix__
#include "cjson/cJSON.h"
#include "utils.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h> 
 #ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#endif

#include "launcher.h"

cJSON* mc_GetMainManifest(GamePath gamePath)
{
    char* path = gamePath.root;
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

cJSON* mc_RefreshMainManifest(GamePath gamePath)
{
    char* path = gamePath.root;
	size_t len_fullpath = (strlen(path) + 25)*  sizeof(char*);
	char* fullpath = malloc(len_fullpath);

	if (fullpath == NULL)
        system_Error(1, "Could not allocate memory");

    snprintf(fullpath, len_fullpath, "%s/version_manifest_v2.json", path);
    if (system_FileExist(fullpath))
        remove(fullpath);
    free(fullpath);

    return mc_GetMainManifest(gamePath);
}

char* mc_GetInherit(cJSON* manifest)
{
    char* version = NULL;
	manifest = cJSON_GetObjectItemCaseSensitive(manifest, "inheritsFrom");
	if (manifest)
    {
        version = malloc(sizeof(char) * (strlen(manifest->valuestring)+1));
        strcpy(version, manifest->valuestring);
		return version;
    }
	return NULL;
}

cJSON* mc_GetManifest(cJSON* versionManifest, GamePath gamePath, char* version)
{
    char* path = gamePath.version;
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
    if (versions == NULL)
        return NULL;
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

int mc_DoesVersionExist(char* version, GamePath gamePath)
{
    int exist = 0;
    cJSON* id = NULL;
    cJSON* versionInfo = NULL;
    size_t len_filepath = (strlen(gamePath.version) + (strlen(version) * 2) + 8);
    char* filepath = malloc(sizeof(char) * len_filepath);
    snprintf(filepath, len_filepath, "%s/%s/%s.json", gamePath.version, version, version);

    if (system_FileExist(filepath) == 0)
    {
        free(filepath);
        return 1;
    }
    free(filepath);
    
    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    for (int i = 0; i < 2; i++)
    {
        cJSON* versions = cJSON_GetObjectItemCaseSensitive(mainManifest, "versions");
        if (versions)
        {
            cJSON_ArrayForEach(versionInfo, versions)
            {
                id = cJSON_GetObjectItemCaseSensitive(versionInfo, "id");
                if (strcmp(id->valuestring,version) == 0)
                {
                    exist = 1;
                    break;
                }
            }
        }
        if (exist == 0 && i == 0)
        {
            cJSON_free(mainManifest);
            mainManifest = mc_RefreshMainManifest(gamePath);
        }
    }

    cJSON_free(mainManifest);
    return exist;
}

void mc_ListInstalledVersion(GamePath gamePath)
{
    char* path = gamePath.version;
	DIR *d;
    struct dirent *dir;
    d = opendir(path);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
       {
            size_t len_fullpath = strlen(path) + strlen(dir->d_name) + 2;
            char* fullpath = malloc(sizeof(char) * (len_fullpath));
            snprintf(fullpath, len_fullpath, "%s/%s", path, dir->d_name);
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && system_IsFile(fullpath) == 0)
            {
                size_t len_filename = len_fullpath + strlen(dir->d_name) + 7;
                char* filename = malloc(sizeof(char) * len_filename);
                snprintf(filename, len_filename, "%s/%s.json", fullpath, dir->d_name);
                if (system_FileExist(filename) == 0)
                    printf("%s  ", dir->d_name); 
            }
            free(fullpath);
        }
        printf("\n");
        closedir(d);
    }
}

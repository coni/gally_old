#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "minecraft/versions.h"
#include "utils.h"
#include "launcher.h"
#include "cjson/cJSON.h"

int compareLwjglVersion(char* new, char* old)
{
	for (int i = 0; i < (int)strlen(new); i++)
	{
		if (new[i] > old[i])
			return 1;
		else if (new[i] < old[i])
			return -1;
	}
	return 0;
}

char* str_split(char* text, char chr, int index)
{
    int i = 0;
    size_t len_element = 0;
    char* element = NULL;

    while (index>0 && text[i] != '\0')
    {
        while (text[i] != chr && text[i] != '\0')
            i++;
        index--;
        if (text[i] != '\0')
            i++;
    }
    
    if (text[i] == '\0')
        return NULL;

    while (text[i+len_element] != chr && text[i+len_element] != '\0')
        len_element++;


    element = malloc(sizeof(char) * (len_element+1));
    for (size_t j = 0; j < len_element; j++)
        element[j] = text[i+j];
    element[len_element] = '\0';
    return element;
}

char* mc_GetLwjglVersion(cJSON* manifest)
{
	char* name = NULL;
	char* version = NULL;

	char* lwjglVersion = malloc(sizeof(char) * 6);
    strcpy(lwjglVersion, "0.0.0");

    cJSON* libName = NULL;
    cJSON* lib = NULL;

    cJSON* libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");
	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
            
            libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
            if (libName)
            {
                name = str_split(libName->valuestring, ':', 1);
                if (strcmp(name, "lwjgl") == 0)
                {
                    version = str_split(libName->valuestring, ':', 2);
                    if (compareLwjglVersion(version, lwjglVersion))
                    {
                        free(lwjglVersion);
                        lwjglVersion = version;
                    } 
                    else
                        free(version);
                }
                free(name);
            }
        }
    }
    
    if (lwjglVersion != NULL)
    {
        lwjglVersion[5] = '\0';

        if (strcmp(lwjglVersion, "2.9.4") == 0)
            lwjglVersion[4] = '2';
    }

	return lwjglVersion;
}


int mc_GetLibrariesSize(cJSON* manifest)
{
    cJSON* libDlInfo = NULL;
	cJSON* libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");
    cJSON* tmp = NULL;
    cJSON* tmp_i = NULL;
    cJSON* lib = NULL;
    int total_size = 0;
    int isCorrectOs = 0;
	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
            isCorrectOs = 1;
	        tmp = cJSON_GetObjectItemCaseSensitive(lib, "rules");
            if (tmp)
            {
                isCorrectOs = 0;
                cJSON_ArrayForEach(tmp_i, tmp)
                {
                    tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "os");
                    if (tmp)
                    {
                        tmp = cJSON_GetObjectItemCaseSensitive(tmp, "name");
                        if (tmp)
                        {
                            if (strcmp(OSNAME, tmp->valuestring) == 0)
                            {
                                tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "action");
                                if (tmp)
                                {
                                    if (strcmp(tmp->valuestring, "disallow") == 0)
                                        isCorrectOs = 0;
                                    else if (strcmp(tmp->valuestring, "allow") == 0)
                                        isCorrectOs = 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "action");
                        if (tmp)
                        {
                            if (strcmp(tmp->valuestring, "disallow") == 0)
                                isCorrectOs = 0;
                            else if (strcmp(tmp->valuestring, "allow") == 0)
                                isCorrectOs = 1;
                        }
                    }
                }
            }
            if (isCorrectOs == 0)
                continue;

			cJSON* libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "downloads");
			if (libDlInfo)
			{	
				libDlInfo = cJSON_GetObjectItemCaseSensitive(libDlInfo, "artifact");
				if (libDlInfo)
				{
					cJSON* size = cJSON_GetObjectItemCaseSensitive(libDlInfo, "size");
                    total_size += size->valueint;
				}
			}
        }
    }

    return total_size;
}

int mc_GetLibrariesSizeVersion(char* version, GamePath gamePath)
{
    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);
    int size = mc_GetLibrariesSize(manifest);

    cJSON_Delete(mainManifest);
    cJSON_Delete(manifest);
    return size;
}

char** mc_DownloadLibraries(cJSON *manifest, GamePath gamePath)
{
    char* path = gamePath.libraries;
    int isCorrectOs = 0;

    size_t len_path = strlen(path);
	char* fullpath = NULL;
	char* org = NULL;
	char* name = NULL;
	char* version = NULL;
    char* native = NULL;

    size_t len_native = 0;
    size_t len_name = 0;
    size_t len_version = 0;
    size_t len_org = 0;


    size_t len_classpath = 0;
	char** classpath = malloc(sizeof(char*));

	cJSON* lib = NULL;
	cJSON* libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");
    cJSON* tmp = NULL;
    cJSON* tmp_i = NULL;
    cJSON* libDlInfo = NULL;

    char* libNameFormatted = NULL;
    char* libUrl = NULL;
    cJSON* libName = NULL;
    cJSON* urlInfo = NULL;

	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
            isCorrectOs = 1;
	        tmp = cJSON_GetObjectItemCaseSensitive(lib, "rules");
            if (tmp)
            {
                isCorrectOs = 0;
                cJSON_ArrayForEach(tmp_i, tmp)
                {
                    tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "os");
                    if (tmp)
                    {
                        tmp = cJSON_GetObjectItemCaseSensitive(tmp, "name");
                        if (tmp)
                        {
                            if (strcmp(OSNAME, tmp->valuestring) == 0)
                            {
                                tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "action");
                                if (tmp)
                                {
                                    if (strcmp(tmp->valuestring, "disallow") == 0)
                                        isCorrectOs = 0;
                                    else if (strcmp(tmp->valuestring, "allow") == 0)
                                        isCorrectOs = 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "action");
                        if (tmp)
                        {
                            if (strcmp(tmp->valuestring, "disallow") == 0)
                                isCorrectOs = 0;
                            else if (strcmp(tmp->valuestring, "allow") == 0)
                                isCorrectOs = 1;
                        }
                    }
                }
            }
            if (isCorrectOs == 0)
                continue;

			libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
            size_t len_libName = strlen(libName->valuestring);
			libNameFormatted = NULL;

			fullpath = NULL;

            org = str_split(libName->valuestring, ':', 0);
            len_org = strlen(org);

            name = str_split(libName->valuestring, ':', 1);
            len_name = strlen(name);

            version = str_split(libName->valuestring, ':', 2);
            len_version = strlen(version);

            native = str_split(libName->valuestring, ':', 3);
            if (native != NULL)
                len_native = strlen(native);
            else
                len_native = 0;
            
			size_t len_libNameFormatted = len_libName + len_name + len_version + len_native + 7;
			libNameFormatted = realloc(libNameFormatted, sizeof(char) * len_libNameFormatted);

            for (int i=0; org[i] != '\0'; i++)
            {
                if (org[i] == '.')
                    org[i] = '/';
            }

			snprintf(libNameFormatted, len_libNameFormatted, "%s/%s/%s/%s-%s", org, name, version, name, version);
            if (len_native != 0)
            {
                strncat(libNameFormatted, "-", len_libNameFormatted);
                strncat(libNameFormatted, native, len_libNameFormatted);
                free(native);
            }

            strncat(libNameFormatted, ".jar", len_libNameFormatted);

			size_t len_fullpath = (len_path + len_libNameFormatted + 2);
			fullpath = realloc(fullpath, sizeof(char) * len_fullpath);
			snprintf(fullpath, len_fullpath, "%s/%s", path, libNameFormatted);
			
            classpath[len_classpath] = malloc(sizeof(char) * (strlen(fullpath) + 1));
            strcpy(classpath[len_classpath], fullpath);
            len_classpath++;
            classpath = realloc(classpath, sizeof(char *) * (len_classpath  + 1));

			// Download Librarie
			libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "downloads");
			if (libDlInfo)
			{	
				libDlInfo = cJSON_GetObjectItemCaseSensitive(libDlInfo, "artifact");
				if (libDlInfo)
				{
					urlInfo = cJSON_GetObjectItemCaseSensitive(libDlInfo, "url");
					http_Download(urlInfo->valuestring, fullpath);
				}
			} else
			{
				libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "url");
				if (libDlInfo)
				{
					size_t len_libUrl = strlen(libDlInfo->valuestring) + strlen(libNameFormatted) + 1;
					libUrl = realloc(libUrl, sizeof(char) * len_libUrl);
					snprintf(libUrl, len_libUrl, "%s%s", libDlInfo->valuestring, libNameFormatted);
					http_Download(libUrl, fullpath);	
                    free(libUrl);
				}
			}

            free(fullpath);
            free(libNameFormatted);
            free(org);
            free(name);
            free(version);
		}
	}
    classpath[len_classpath] = NULL;

	return classpath;
}

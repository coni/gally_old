#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../utils.h"
#include "../cjson/cJSON.h"

int compareLwjglVersion(char* new, char* old)
{
	for (int i = 0; i < (int)strlen(new); i++)
	{
		if (new[i] > old[i])
			return 0;
		else if (new[i] < old[i])
			return 1;
	}
	return 1;
}

char* mc_GetLwjglVersion(cJSON* manifest)
{
	cJSON* libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");
	cJSON* lib = NULL;
	char* lwjglVersion = malloc(sizeof(char *) * 6);
	strcpy(lwjglVersion, "0.0.0");
	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			cJSON *libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
			int isLwjgl = 0;
			char* version = NULL;
			char* splittedLibName = libName->valuestring;
			char* splittedLibNameElt = strtok(splittedLibName, ":");
			char* org = NULL;
			

			org = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(org, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");

			char *name = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(name, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");
			
			version = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(version, splittedLibNameElt);
			
			char* libNameFormatted = malloc(sizeof (char *) * ((strlen(name) + strlen(version))* 2 + 8));
			strcpy(libNameFormatted, "");


			char* splitOrg = strtok(org, ".");
			while (splitOrg)
			{
				strcat(libNameFormatted, splitOrg);
				strcat(libNameFormatted, "/");
				if (strcmp(splitOrg, "lwjgl") == 0)
					isLwjgl = 1;
				splitOrg = strtok(NULL, ".");
			}

			if (isLwjgl)
			{
				version[5] = '\0';
				if (strcmp(lwjglVersion, version) != 0)
				{
					if (compareLwjglVersion(version, lwjglVersion) == 0)
					{
						strcpy(lwjglVersion, version);
					}
					else
						continue;
				}
			}
		}
	}
	if (strcmp(lwjglVersion, "2.9.4") == 0)
		lwjglVersion = "2.9.2";
	else if (strcmp(lwjglVersion, "0.0.0") == 0)
		lwjglVersion = NULL;
	return lwjglVersion;
}

char* mc_DownloadLibraries(cJSON *manifest, char *path)
{
	char* fullpath = NULL;
	char* org = NULL;
	char* name = NULL;
	char* version = NULL;
	int isLwjgl = 0;

	char* classpath = malloc(sizeof(char *));
	strcpy(classpath, "");

	cJSON* lib = NULL;
	cJSON* libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");

	char* lwjglClasspath = malloc(sizeof(char));
	strcpy(lwjglClasspath, "");

	char* lwjglVersion = malloc(sizeof(char) * 6);
	strcpy(lwjglVersion, "0.0.0");

	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			cJSON *libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
			/* cJSON *libUrl = cJSON_GetObjectItemCaseSensitive(lib, "url"); */
			char *splittedLibName = libName->valuestring;
			char *splittedLibNameElt = strtok(splittedLibName, ":");
			char *libNameFormatted = NULL;

			fullpath = NULL;
			org = NULL;
			name = NULL;
			version = NULL;
			isLwjgl = 0;

			size_t len_org = strlen(splittedLibNameElt) + 1;
			org = malloc(sizeof(char) * len_org);
			strncpy(org, splittedLibNameElt, len_org);
			splittedLibNameElt =  strtok(NULL,":");

			size_t len_name = strlen(splittedLibNameElt) + 1;
			name = malloc(sizeof(char) * len_name);
			strncpy(name, splittedLibNameElt, len_name);
			splittedLibNameElt =  strtok(NULL,":");

			size_t len_version = strlen(splittedLibNameElt) + 1;
			version = malloc(sizeof(char) * len_version);
			strncpy(version, splittedLibNameElt, len_version);


			size_t len_libNameFormatted = strlen(org) + (strlen(name) + strlen(version)) * 2 + 10;
			libNameFormatted = malloc(sizeof(char) * len_libNameFormatted);
			strncpy(libNameFormatted, "", len_libNameFormatted);

			char *splitOrg = strtok(org, ".");
			while (splitOrg)
			{
				strncat(libNameFormatted, splitOrg, len_libNameFormatted);
				strncat(libNameFormatted, "/", len_libNameFormatted);
				if (strcmp(splitOrg, "lwjgl") == 0)
					isLwjgl = 1;
				splitOrg = strtok(NULL, ".");
			}

			snprintf(libNameFormatted + len_org, len_libNameFormatted - len_org, "%s/%s/%s-%s.jar", name, version, name, version);

			size_t len_fullpath = (strlen(path) + strlen(libNameFormatted) + 2);
			fullpath = malloc(sizeof(char) * len_fullpath);
			snprintf(fullpath, len_fullpath, "%s/%s", path, libNameFormatted);
			
			if (isLwjgl)
			{
				if (strlen(version) >= 5)
					version[5] = '\0';

				if (strcmp(lwjglVersion, version) != 0)
				{
					if (compareLwjglVersion(version, lwjglVersion) == 0)
					{
						lwjglClasspath = realloc(lwjglClasspath, sizeof(char) * (strlen(fullpath) + 1));
						strcpy(lwjglClasspath, fullpath);
						strcpy(lwjglVersion, version);
					}
					else
						continue;
				}
				lwjglClasspath = realloc(lwjglClasspath, sizeof(char *) * (strlen(lwjglClasspath) + strlen(fullpath) + 1));
				strcat(lwjglClasspath, ":");
				strcat(lwjglClasspath, fullpath);
			}
			else
			{
				classpath = realloc(classpath, sizeof(char *) * (strlen(classpath) + strlen(fullpath) + 1));
				strcat(classpath, fullpath);
				strcat(classpath, ":");
			}
				// Download Librarie
			cJSON *libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "downloads");
			if (libDlInfo)
			{	
				libDlInfo = cJSON_GetObjectItemCaseSensitive(libDlInfo, "artifact");
				if (libDlInfo)
				{
					cJSON *libUrl = cJSON_GetObjectItemCaseSensitive(libDlInfo, "url");
					http_Download(libUrl->valuestring, fullpath);
				}
			} else
			{
				libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "url");
				if (libDlInfo)
				{
					size_t len_libUrl = strlen(libDlInfo->valuestring) + strlen(libNameFormatted) + 1;
					char* libUrl = malloc(sizeof(char) * len_libUrl);
					snprintf(libUrl, len_libUrl, "%s%s", libDlInfo->valuestring, libNameFormatted);
					http_Download(libUrl, fullpath);	
				}
			}

			//free(fullpath);
		}
	}

	if (strcmp(lwjglClasspath, "") != 0)
	{
		classpath = realloc(classpath, strlen(classpath) + strlen(lwjglClasspath));
		strcat(classpath, lwjglClasspath);
	}
	return classpath;
}

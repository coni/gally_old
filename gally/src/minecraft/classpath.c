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
    cJSON *libName = NULL;
    cJSON* lib = NULL;
    int isLwjgl;
    char* version = NULL;
    char* splittedLibName = NULL;
    char* splittedLibNameElt = NULL;
    char* org = NULL;
    char* name = NULL;
    char* libNameFormatted = NULL;
    char* splitOrg = NULL;
    char* lwjglVersion = "0.0.0";

	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
            splittedLibName = libName->valuestring;
            splittedLibNameElt = strtok(splittedLibName, ":");
			isLwjgl = 0;
			version = NULL;
			org = NULL;
			

			org = realloc(org, sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(org, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");

			name = realloc(name, sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(name, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");
			
			version = realloc(version, sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(version, splittedLibNameElt);
			
			libNameFormatted = realloc(libNameFormatted, sizeof (char *) * ((strlen(name) + strlen(version))* 2 + 8));
			strcpy(libNameFormatted, "");


			splitOrg = strtok(org, ".");
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
						strcpy(lwjglVersion, version);
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
    
    free(libraries);
    free(libName);
    free(lib);
    free(version);
    free(splittedLibName);
    free(splittedLibNameElt);
    free(org);
    free(name);
    free(libNameFormatted);
    free(splitOrg);

	return lwjglVersion;
}

char* mc_DownloadLibraries(cJSON *manifest, char *path)
{
    int isCorrectOs = 0;

	char* fullpath = NULL;
	char* org = NULL;
	char* name = NULL;
	char* version = NULL;
    char* native = NULL;

    size_t len_native = 0;
    size_t len_name = 0;
    size_t len_version = 0;
    size_t len_org = 0;

	int isLwjgl = 0;

	char* classpath = malloc(sizeof(char *));
	strcpy(classpath, "");

	cJSON* lib = NULL;
	cJSON* libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");
    cJSON* tmp = NULL;
    cJSON* tmp_i = NULL;
    cJSON* libDlInfo = NULL;

	char* lwjglClasspath = "";
	char* lwjglVersion = "0.0.0";

    char* splittedLibName = NULL;
    char* splittedLibNameElt = NULL;
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
                cJSON_ArrayForEach(tmp_i, tmp)
                {
                    tmp = cJSON_GetObjectItemCaseSensitive(tmp_i, "os");
                    if (tmp)
                    {
                        tmp = cJSON_GetObjectItemCaseSensitive(tmp, "name");
                        if (tmp)
                        {
                            if (strcmp(OSNAME, tmp->valuestring) != 0)
                            {
                                isCorrectOs = 0;
                            }
                        }
                    }
                }
            }
            if (isCorrectOs == 0)
                continue;

			libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
            size_t len_libName = strlen(libName->valuestring);
			splittedLibName = libName->valuestring;
			splittedLibNameElt = strtok(splittedLibName, ":");
			libNameFormatted = NULL;

			fullpath = NULL;
			org = NULL;
			name = NULL;
			version = NULL;
			isLwjgl = 0;

			len_org = strlen(splittedLibNameElt) + 1;
			org = realloc(org, sizeof(char) * len_org);
			strncpy(org, splittedLibNameElt, len_org);
			splittedLibNameElt =  strtok(NULL,":");

			len_name = strlen(splittedLibNameElt) + 1;
			name = realloc(name, sizeof(char) * len_name);
			strncpy(name, splittedLibNameElt, len_name);
			splittedLibNameElt =  strtok(NULL,":");

			len_version = strlen(splittedLibNameElt) + 1;
			version = realloc(version, sizeof(char) * len_version);
			strncpy(version, splittedLibNameElt, len_version);
			splittedLibNameElt =  strtok(NULL,":");

            if (splittedLibNameElt != NULL)
            {
                len_native = strlen(splittedLibNameElt) + 2;
                native = realloc(native, sizeof(char) * len_native);
                strncpy(native, splittedLibNameElt, len_native);
            }
            else
            {
                len_native = 0;
                native = NULL;
            }
            
			size_t len_libNameFormatted = len_libName + len_name + len_version + len_native + 5;
			libNameFormatted = realloc(libNameFormatted, sizeof(char) * len_libNameFormatted);

            for (int i=0; org[i] != '\0'; i++)
            {
                if (org[i] == '.')
                    org[i] = '/';
            }

			snprintf(libNameFormatted, len_libNameFormatted, "%s/%s/%s/%s-%s", org, name, version, name, version);
            if (native != NULL)
            {
                strncat(libNameFormatted, "-", len_libNameFormatted);
                strncat(libNameFormatted, native, len_libNameFormatted);
            }

            strncat(libNameFormatted, ".jar", len_libNameFormatted);

			size_t len_fullpath = (strlen(path) + strlen(libNameFormatted) + 2);
			fullpath = realloc(fullpath, sizeof(char) * len_fullpath);
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
				lwjglClasspath = realloc(lwjglClasspath, sizeof(char) * (strlen(lwjglClasspath) + strlen(fullpath) + 1));
				strcat(lwjglClasspath, CLASSSEPARATOR);
				strcat(lwjglClasspath, fullpath);
			}
			else
			{
				classpath = realloc(classpath, sizeof(char *) * (strlen(classpath) + strlen(fullpath) + 1));
				strcat(classpath, fullpath);
				strcat(classpath, CLASSSEPARATOR);
			}
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
    free(org);
    free(name);
    free(version);
    free(native);
    free(lib);
    /* free(libraries); */
    free(tmp);
    free(tmp_i);
    free(libDlInfo);
    /* free(lwjglClasspath); */
    /* free(lwjglVersion); */
    free(splittedLibName);
    /* free(splittedLibNameElt); */
    free(libNameFormatted);
    free(libUrl);
    free(libName);
    free(urlInfo);

	return classpath;
}

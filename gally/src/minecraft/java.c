#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cjson/cJSON.h"
#include "utils.h"
#include "launcher.h"

cJSON* mc_GetJreMainManifest(char* path)
{
    size_t len_filename = (strlen(path) + 10);
    char* filename = malloc(len_filename * sizeof(char));
    cJSON * manifest = NULL;

    snprintf(filename, len_filename, "%s/all.json", path);
    http_Download("https://launchermeta.mojang.com/v1/products/java-runtime/2ec0cc96c44e5a76b9c8b7c39df7210883d12871/all.json", filename);

    manifest = json_ParseFile(filename);

    free(filename);
    return manifest;
}

cJSON* mc_GetJreManifest(cJSON* manifest, char* component, GamePath gamePath)
{
    char* path = gamePath.runtime;
    size_t len_fullpath;
    size_t len_os;
    char* os = NULL;
    char* fullpath = NULL;
    cJSON* jreManifest = NULL;
    cJSON* i = NULL;

    if (strcmp(OSNAME, "windows") == 0 || (strcmp(OSNAME, "linux") == 0 && strcmp(ARCHNAME, "x86") == 0))
    {
        len_os = strlen(OSNAME) + strlen(ARCHNAME) + 2;
        os = malloc(len_os * sizeof(char));
        snprintf(os, len_os, "%s-%s", OSNAME, ARCHNAME);
    }
    else
    {
        len_os = strlen(OSNAME) + 1;
        os = malloc(len_os * sizeof(char));
        strncpy(os, OSNAME, len_os);
    }

    manifest = cJSON_GetObjectItemCaseSensitive(manifest, os);
    len_fullpath = (strlen(path) + strlen(component) + 7);
    fullpath = malloc(len_fullpath * sizeof(char));
    snprintf(fullpath, len_fullpath, "%s/%s.json", path, component);

    if (manifest)
    {
        manifest = cJSON_GetObjectItemCaseSensitive(manifest, component);
        if (manifest)
        {
            cJSON_ArrayForEach(i, manifest)
            {
                manifest = cJSON_GetObjectItemCaseSensitive(i, "manifest");
                if (manifest)
                {
                    cJSON * url = cJSON_GetObjectItemCaseSensitive(manifest, "url");
                    http_Download(url->valuestring, fullpath);
                    jreManifest = json_ParseFile(fullpath);
                }
            }
        }
    }
    free(os);
    free(fullpath);
    free(i);

    return jreManifest;
}

char* mc_GetJreComponent(cJSON* manifest)
{
    cJSON * i = cJSON_GetObjectItemCaseSensitive(manifest, "javaVersion");
    if (i)
    {
        i = cJSON_GetObjectItemCaseSensitive(i, "component");
        if (i)
            return i->valuestring;
    }
    return NULL;
}

char* mc_DownloadJre(cJSON* manifest, GamePath gamePath)
{
    char* path = gamePath.runtime;
    size_t len_javaPath;
    size_t len_os;
    size_t len_fullpath;
    char* os = NULL;
    char* tmpPath = NULL;
    char* component = NULL;
    char* javaPath = NULL;
    char* filename = NULL;
    char* fullpath = NULL;
    cJSON* jreBaseManifest = NULL;
    cJSON* jreManifest = NULL;
    cJSON* files = NULL;
    cJSON* element = NULL;
    
    if (strcmp(OSNAME, "windows") == 0 || (strcmp(OSNAME, "linux") == 0 && strcmp(ARCHNAME, "i386") == 0))
    {
        len_os = strlen(OSNAME) + strlen(ARCHNAME) + 2;
        os = malloc(len_os * sizeof(char));
        snprintf(os, len_os, "%s-%s", OSNAME, ARCHNAME);
    }
    else
    {
        len_os = strlen(OSNAME) + 1;
        os = malloc(len_os * sizeof(char));
        strncpy(os, OSNAME, len_os);
    }

    tmpPath = malloc((strlen(path) + 6) * sizeof(char));
    strcpy(tmpPath, path);
    strcat(tmpPath, "temp/");

    component = mc_GetJreComponent(manifest);
    if (component == NULL)
        return NULL;

    /* if (component == NULL) */
    /*     component = "jre-legacy"; */

    jreBaseManifest = mc_GetJreMainManifest(path);
    jreManifest = mc_GetJreManifest(jreBaseManifest, component, gamePath);


    len_javaPath = strlen(path) + (strlen(component)*2) + strlen(os) + 5;
    javaPath = malloc(len_javaPath * sizeof(char));
    snprintf(javaPath, len_javaPath, "%s/%s/%s/%s", path, component, os, component); 

    if (jreManifest)
    {
        files = cJSON_GetObjectItemCaseSensitive(jreManifest, "files");
        if (files)
        {
            cJSON_ArrayForEach(element, files)
            {
                cJSON* type = cJSON_GetObjectItemCaseSensitive(element, "type");
                cJSON* downloads = cJSON_GetObjectItemCaseSensitive(element, "downloads");

                downloads = cJSON_GetObjectItemCaseSensitive(downloads, "raw");
                cJSON * url = cJSON_GetObjectItemCaseSensitive(downloads,"url");
                filename = element->string;
                len_fullpath = (len_javaPath + strlen(filename) + 2);
                fullpath = realloc(fullpath, sizeof(char) * len_fullpath);
                snprintf(fullpath, len_fullpath, "%s/%s", javaPath, filename);

                if (type)
                {
                    if (strcmp(type->valuestring, "file") == 0)
                    {
                        http_Download(url->valuestring, fullpath);
                        #ifdef __unix__
                        cJSON* executable = cJSON_GetObjectItemCaseSensitive(element, "executable");
                        if (executable)
                        {
                            if (executable->valueint == 1)
                                system_MakeExec(fullpath);
                        }
                        #endif
                    }
                    else if (strcmp(type->valuestring, "directory") == 0)
                        system_Mkdir(fullpath);
                }
            }
        }
    }

    cJSON_Delete(jreBaseManifest);
    cJSON_Delete(jreManifest);
    free(os);
    free(tmpPath);

    return javaPath;
}

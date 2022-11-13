#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../cjson/cJSON.h"
#include "../utils.h"


cJSON* mc_GetJreMainManifest(char* path)
{
    size_t len_filename = (strlen(path) + 10);
  char* filename = malloc(len_filename * sizeof(char));
  cJSON * manifest = NULL;
  snprintf(filename, len_filename, "%s/all.json", path);
    http_Download("https://launchermeta.mojang.com/v1/products/java-runtime/2ec0cc96c44e5a76b9c8b7c39df7210883d12871/all.json", filename);

  manifest = json_ParseFile(filename);
  return manifest;
}

cJSON* mc_GetJreManifest(cJSON* manifest, char* component, char* path)
{
    size_t len_os = 0;
    char* os = NULL;
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

  cJSON * jreManifest = NULL;
  cJSON * i = NULL;
  manifest = cJSON_GetObjectItemCaseSensitive(manifest, os);
  size_t len_fullpath = (strlen(path) + strlen(component) + 7);
  char* fullpath = malloc(len_fullpath * sizeof(char));
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
  return jreManifest;
}

char* mc_GetJreComponent(cJSON* manifest)
{
  char * component = NULL;
  cJSON * i = cJSON_GetObjectItemCaseSensitive(manifest, "javaVersion");
  if (i)
  {
    i = cJSON_GetObjectItemCaseSensitive(i, "component");
    if (i)
    {
      component = i->valuestring;
    }
  }
  return component;
}

char* mc_DownloadJre(cJSON* manifest, char* path)
{
    size_t len_os = 0;
    char* os = NULL;
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

  char * tempPath = malloc((strlen(path) + 6) * sizeof(char));
  strcpy(tempPath, path);
  strcat(tempPath, "temp/");

  char * component = mc_GetJreComponent(manifest);
  if (component == NULL)
    return NULL;
  cJSON * jreBaseManifest = mc_GetJreMainManifest(path);
  cJSON * jreManifest = mc_GetJreManifest(jreBaseManifest, component, path);
  
  size_t len_javaPath = strlen(path) + (strlen(component)*2) + strlen(os) + 5;
  char * javaPath = malloc(len_javaPath * sizeof(char));
  snprintf(javaPath, len_javaPath, "%s/%s/%s/%s", path, component, os, component); 

  cJSON * element = NULL;

  if (jreManifest)
  {
    jreManifest = cJSON_GetObjectItemCaseSensitive(jreManifest, "files");
    if (jreManifest)
    {
      cJSON_ArrayForEach(element, jreManifest)
      {
        cJSON* type = cJSON_GetObjectItemCaseSensitive(element, "type");
        cJSON* downloads = cJSON_GetObjectItemCaseSensitive(element, "downloads");

        downloads = cJSON_GetObjectItemCaseSensitive(downloads, "raw");
        cJSON * url = cJSON_GetObjectItemCaseSensitive(downloads,"url");
        char* filename = element->string;
        size_t len_fullpath = (len_javaPath + strlen(filename) + 2);
        char* fullpath = malloc(sizeof(char) * len_fullpath);
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

  return javaPath;
}

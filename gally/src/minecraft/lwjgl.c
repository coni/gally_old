#include "../cjson/cJSON.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils.h"

cJSON * mc_GetBaseLwjglManifest(char* path)
{
        
    size_t len_filename = strlen(path) + 11;
      char* filename = malloc(sizeof(char) * len_filename);
      snprintf(filename, len_filename, "%s/lwjgl.json", path);
      http_Download("https://gally-launcher.com/files/lwjgl.json", filename);
      return json_ParseFile(filename);
}

char* mc_DownloadLwjgl(char* lwjglVersion, char* path)
{
	char * lwjglPath = malloc((strlen(path) + strlen(lwjglVersion) + 2)*sizeof(char*));
	strcpy(lwjglPath, path);
	strcat(lwjglPath, lwjglVersion);
	strcat(lwjglPath, "/");

  cJSON * lwjglManifest = mc_GetBaseLwjglManifest(path);
  cJSON * i = NULL;

  lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, "lwjgl");
  if (lwjglManifest)
  {
    lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, OSNAME);
    if (lwjglVersion)
    {
      lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, ARCHNAME);
      if (lwjglManifest)
      {
        lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, lwjglVersion);
        cJSON_ArrayForEach(i, lwjglManifest)
        {
          cJSON * url = cJSON_GetObjectItemCaseSensitive(i, "url");
          cJSON * filename = cJSON_GetObjectItemCaseSensitive(i, "filename");
          char * fullpath = malloc((strlen(lwjglPath) + strlen(filename->valuestring) + 1) * sizeof(char *));
          strcpy(fullpath, lwjglPath);
          strcat(fullpath, filename->valuestring);
          http_Download(url->valuestring, fullpath);
          free(fullpath);
        }
      }
    }
  }

  cJSON_Delete(lwjglManifest);
  return lwjglPath;
}

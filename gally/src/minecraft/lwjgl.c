#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../cjson/cJSON.h"
#include "../utils.h"

cJSON * mc_GetBaseLwjglManifest(char* path)
{
        
    size_t len_filename = strlen(path) + 12;
      char* filename = malloc(sizeof(char) * len_filename);
      snprintf(filename, len_filename, "%s/lwjgl.json", path);
      http_Download("https://gally-launcher.com/files/lwjgl.json", filename);
      free(filename);
      return json_ParseFile(filename);
}

char* mc_DownloadLwjgl(char* lwjglVersion, char* path)
{
	char * lwjglPath = NULL;


  cJSON * lwjglManifest = mc_GetBaseLwjglManifest(path);
  cJSON * i = NULL;

  cJSON* tmp = cJSON_GetObjectItemCaseSensitive(lwjglManifest, "lwjgl");
  if (tmp)
  {
    tmp = cJSON_GetObjectItemCaseSensitive(tmp, OSNAME);
    if (tmp)
    {
      tmp = cJSON_GetObjectItemCaseSensitive(tmp, ARCHNAME);
      if (tmp)
      {
        tmp = cJSON_GetObjectItemCaseSensitive(tmp, lwjglVersion);
        if (tmp)
        {
            lwjglPath = malloc((strlen(path) + strlen(lwjglVersion) + 3)*sizeof(char));
            strcpy(lwjglPath, path);
            strcat(lwjglPath, "/");
            strcat(lwjglPath, lwjglVersion);
            strcat(lwjglPath, "/");

            cJSON_ArrayForEach(i, tmp)
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
  }

  cJSON_Delete(lwjglManifest);
  return lwjglPath;
}

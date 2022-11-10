#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <cjson/cJSON.h>
#endif

#ifdef __unix__
#include <unistd.h>
#include "cjson/cJSON.h"
#endif

#include "utils.h"
#include <curl/curl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// VERY TRASH, NEED FIX
cJSON *json_ParseFile(char *filename)
{
	int c;
	int index = 0;
	int size = 2;
	char* content = malloc(size);
	FILE* fp;

	fp = fopen(filename, "r");
	while ((c = getc(fp)) != EOF)
	{
		if (index >= size - 1)
			content = realloc(content, size++);
		content[index] = c;
		index++;
	}
	fclose(fp);

	return cJSON_Parse(content);
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
	return written;
}

int http_Download(char *url, char *filename)
{
	CURL **session = curl_easy_init();

	// Source : https://curl.se/libcurl/c/url2file.html
	if (system_FileExist(filename) == 0)
		return 0;

	FILE* pagefile;
	int http_code = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl_easy_setopt(session, CURLOPT_URL, url);
	curl_easy_setopt(session, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1);

	system_Mkdir(filename);
	pagefile = fopen(filename, "wb");
	if (pagefile) {
		curl_easy_setopt(session, CURLOPT_WRITEDATA, pagefile);
		curl_easy_perform(session);
		fclose(pagefile);
	}
	curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &http_code);

	curl_global_cleanup();
	curl_easy_cleanup(session);

	return http_code;
}


int system_Exec(char *command)
{
    return system(command);
}

int system_Mkdir(char *dir) 
{
    char tmp[1024];
    char *p = NULL;

    snprintf(tmp, sizeof(tmp), "%s", dir);

    *strrchr(tmp, '/') = 0;
    for (p = tmp + 1; *p; p++)
    {
        if (*p == '/') {
            *p = 0;
			if (_mkdir(tmp) == 1)
				return 1;
            *p = '/';
        }
    }
    return _mkdir(tmp);
}

int system_MakeExec(char *file)
{
	size_t len_command = strlen(file) + 8;
    char *command = malloc(len_command);

	if (command != NULL)
	{
		snprintf(command, len_command, "chmod+x %s", file);
		system_Exec(command);
		free(command);
		return 0;
	}

	free(command);
	return 1;
}

void system_Error(int code, char *string)
{
    fprintf(stderr, string);
    exit(code);
}

int system_FileExist(char *path)
{
	return access(path, F_OK);
}

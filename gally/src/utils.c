#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <tchar.h>
#endif

#ifdef __unix__
#include <unistd.h>
#endif

#include <curl/curl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson/cJSON.h"
#include "utils.h"

int DOWNLOAD_CUR = 0;
int DOWNLOAD_TOTAL = 0;
int DOWNLOAD_TEST = 0;

cJSON* json_ParseFile(char* filename)
{
    cJSON* parsed = NULL;
    int c;
    int i = 0;
    char* content = NULL;
    FILE* fp = fopen(filename, "r");

    while ((c = getc(fp)) != EOF)
        i++;

    fseek(fp, 0, 0);
    content = malloc((i + 1) * sizeof(char));

    i = 0;
    while ((c = getc(fp)) != EOF)
        content[i++] = c;
    content[i] = '\0';
    fclose(fp);
    parsed = cJSON_Parse(content);

    free(content);

    return parsed;
}

char *str_replace(char *orig, char *rep, char *with) 
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(sizeof(char) * (strlen(orig) + (len_with - len_rep) * count + 1));

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    DOWNLOAD_TEST += nmemb;

    if (DOWNLOAD_TOTAL > 0)
    {
        unsigned long a = (unsigned long) DOWNLOAD_TEST;
        unsigned long b = (unsigned long) DOWNLOAD_TOTAL;
        unsigned long c = a*100/b;

        printf(" loading.. %d%% ", c);
        fflush(stdout);
        printf("\r");
    }
	size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
	return written;
}

int http_Download(char* url, char* filename)
{
    int http_code = 0;
    curl_global_init(CURL_GLOBAL_ALL);
	CURL* session = curl_easy_init();

	// Source : https://curl.se/libcurl/c/url2file.html
	if (system_FileExist(filename) != 0)
    {

        FILE* pagefile;

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
    }

	curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &http_code);
	curl_easy_cleanup(session);
    curl_global_cleanup();
    
	return http_code;
}

int system_Exec(char* command)
{
    #ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    if (!CreateProcessA(NULL,   // No module name (use command line)
        command,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    #elif __unix__
    system(command);
    #endif
    return 0;
}

int system_Mkdir(char* dir) 
{
    char tmp[1024];
    char* p = NULL;

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

#ifdef __unix__
int system_MakeExec(char* file)
{
	size_t len_command = strlen(file) + 10;
    char* command = malloc(len_command);

	if (command != NULL)
	{
		snprintf(command, len_command, "chmod +x %s", file);
		system_Exec(command);
		free(command);
		return 0;
	}

	free(command);
	return 1;
}
#endif

void system_Error(int code, char* string)
{
    fprintf(stderr, string);
    exit(code);
}

int system_FileExist(char* path)
{
	return access(path, F_OK);
}

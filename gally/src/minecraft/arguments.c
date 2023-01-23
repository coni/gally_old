#include "cjson/cJSON.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minecraft/arguments.h"

JvmArgs mc_InitJvmArgs()
{
	JvmArgs args;
    args.client = NULL;
	args.classpath = NULL;
	args.launcher_name = "gally";
	args.natives_directory = "NULL";
	args.launcher_version = "1.0";
	return args;
}

GameArgs mc_InitGameArgs()
{
	GameArgs args;
	args.auth_player_name = "steve";
	args.game_directory = "\".\"";
	args.assets_root = "assets";
	args.assets_index_name = "NULL";
	args.auth_uuid = NULL;
	args.auth_access_token = "NULL";
	args.clientid = "NULL";
	args.auth_xuid = "NULL";
	args.user_type = "mojang";
	args.version = "NULL";
	args.version_type = "release";
	args.resolution_width = "NULL";
	args.resolution_height = "NULL";
	args.user_properties = "{}";
	return args;
}

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

char** mc_GetGameArgs(cJSON* manifest, GameArgs args)
{
    char** argv = NULL;
    cJSON* gameArray = cJSON_GetObjectItemCaseSensitive(manifest, "arguments");
    cJSON* i = NULL;
    int argc = 0;
    int count = 0;
    if (gameArray)
    {
        gameArray = cJSON_GetObjectItemCaseSensitive(gameArray, "game");
        if (cJSON_IsArray(gameArray))
        {
            cJSON_ArrayForEach(i, gameArray)
                argc = cJSON_IsString(i) ? argc + 1 : argc;
            argv = malloc(sizeof(char*) * (argc + 1));
            cJSON_ArrayForEach(i, gameArray)
            {
                if (cJSON_IsString(i))
                {
                    argv[count] = malloc(sizeof(char) * (strlen(i->valuestring)+1));
                    strcpy(argv[count++], i->valuestring);
                }
            }
            argv[count] = NULL;
        }
    }
    else if ((i = cJSON_GetObjectItemCaseSensitive(manifest, "minecraftArguments")))
    {
        for (int j = 0; i->valuestring[j] != '\0'; j++)
           argc = i->valuestring[j] == ' ' ? argc + 1 : argc; 
        argv = malloc(sizeof(char*) * (argc + 1));
        char* cur_i = i->valuestring;
        int size = 0;
        for (size_t j = 0; j <= strlen(i->valuestring); j++)
        {
            if (i->valuestring[j] == ' ' || i->valuestring[j] == '\0')
            {
                argv[count] = malloc(sizeof(char) * (size+1));
                int k;
                for (k = 0; k < size; k++)
                    argv[count][k] = *(cur_i + k);
                argv[count][k] = '\0';
                count++;
                cur_i = i->valuestring + j + 1;
                size = -1;
            }
            size++;
        }
        argv[count] = NULL;
    }

    for (int j = 0; argv[j] != NULL; j++)
    {
        if (strcmp("${auth_player_name}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.auth_player_name) + 1));
            strcpy(argv[j],args.auth_player_name);
        }
        else if (strcmp("${version_name}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.version) + 1));
            strcpy(argv[j],args.version);
        }
        else if (strcmp("${game_directory}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.game_directory) + 1));
            strcpy(argv[j],args.game_directory);
        }
        else if (strcmp("${assets_root}", argv[j]) == 0 || strcmp("${game_assets}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.assets_root) + 1));
            strcpy(argv[j],args.assets_root);
        }
        else if (strcmp("${assets_index_name}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.assets_index_name) + 1));
            strcpy(argv[j],args.assets_index_name);
        }
        else if (strcmp("${auth_uuid}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.auth_uuid) + 1));
            strcpy(argv[j],args.auth_uuid);
        }
        else if (strcmp("${auth_access_token}", argv[j]) == 0 || strcmp("${auth_session}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.auth_access_token) + 1));
            strcpy(argv[j],args.auth_access_token);
        }
        else if (strcmp("${clientid}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.clientid) + 1));
            strcpy(argv[j],args.clientid);
        }
        else if (strcmp("${auth_xuid}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.auth_xuid) + 1));
            strcpy(argv[j],args.auth_xuid);
        }
        else if (strcmp("${user_type}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.user_type) + 1));
            strcpy(argv[j],args.user_type);
        }
        else if (strcmp("${version_type}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.version_type) + 1));
            strcpy(argv[j],args.version_type);
        }
        else if (strcmp("${user_properties}", argv[j]) == 0)
        {
            argv[j] = realloc(argv[j], sizeof(char) * (strlen(args.user_properties) + 1));
            strcpy(argv[j], args.user_properties);
        }
    }
	return argv;
}

char** mc_GetJvmArgs(cJSON* manifest, JvmArgs args, GameArgs gameArgs, GamePath gamePath)
{
    char* classseparator = malloc(sizeof(char)*2);
    classseparator[0] = CLASSSEPARATOR;
    classseparator[1] = '\0';
    char** argv = NULL;
    int argc = 0;
    int count = 0;
    cJSON* jvmArray = cJSON_GetObjectItemCaseSensitive(manifest, "arguments");
    cJSON* i = NULL;
    if (jvmArray)
    {
        jvmArray = cJSON_GetObjectItemCaseSensitive(jvmArray, "jvm");
        if (cJSON_IsArray(jvmArray))
        {
            cJSON_ArrayForEach(i, jvmArray)
                argc = cJSON_IsString(i) ? argc + 1 : argc;
            argv = malloc(sizeof(char*) * (argc+1));
            cJSON_ArrayForEach(i, jvmArray)
            {
                if (cJSON_IsString(i))
                {
                    int argv_null = 1;
                    argv[count] = malloc(sizeof(char) * (strlen(i->valuestring) + 1));
                    strcpy(argv[count], i->valuestring);

                    if (strstr(i->valuestring, "${natives_directory}"))
                    {
                        char* tmp = str_replace(argv[count], "${natives_directory}", args.natives_directory);
                        free(argv[count]);
                        argv[count] = tmp;
                    }
                    
                    if (strstr(i->valuestring, "${launcher_name}"))
                    {
                       char* tmp = str_replace(argv[count], "${launcher_name}", args.launcher_name);
                       free(argv[count]);
                       argv[count] = tmp;
                    }

                    if (strstr(i->valuestring, "${launcher_version}"))
                    {
                       char* tmp = str_replace(argv[count], "${launcher_version}", args.launcher_version);
                       free(argv[count]);
                       argv[count] = tmp;
                    }

                    if (strstr(i->valuestring, "${version_name}"))
                    {
                       char* tmp = str_replace(argv[count], "${version_name}", gameArgs.version);
                       free(argv[count]);
                       argv[count] = tmp;
                    }

                    if (strstr(i->valuestring, "${classpath_separator}"))
                    {
                       char* tmp = str_replace(argv[count], "${classpath_separator}", classseparator);
                       free(argv[count]);
                       argv[count] = tmp;
                    }

                    if (strstr(i->valuestring, "${library_directory}"))
                    {
                       char* tmp = str_replace(argv[count], "${library_directory}", gamePath.libraries);
                       free(argv[count]);
                       argv[count] = tmp;
                    }

                    if (strstr(i->valuestring, "${classpath}"))
                    {
                        // MAY CONFLICT WITH FABRIC
                        size_t count_classpath = 1;
                        size_t size = 0;
                        char* cp = NULL;
                        int cur;

                        size += strlen(args.client);
                        for (int i = 0; args.classpath[i] != NULL; i++)
                        {
                            count_classpath++;
                            for (int j = 0; args.classpath[i][j] != '\0'; j++)
                                size++;
                        }
                        cp = malloc(sizeof(char) * (size + count_classpath));
                        cur = 0;

                        for (int j = 0; args.client[j] != '\0'; j++)
                            cp[cur++] = args.client[j];
                        for (int i = 0; args.classpath[i] != NULL; i++)
                        {
                            cp[cur++] = CLASSSEPARATOR;
                            for (int j = 0; args.classpath[i][j] != '\0'; j++)
                                cp[cur++] = args.classpath[i][j];
                        }
                        cp[cur] = '\0';
                        char* tmp = str_replace(argv[count], argv[count], cp);
                        free(argv[count]);
                        free(cp);
                        argv[count] = tmp;
                    }
                    
                    if (strcmp(argv[count], i->valuestring) == 0)
                    {
                        char* tmp  = str_replace(argv[count], " ", "\"");
                        free(argv[count]);
                        argv[count] = tmp;
                    }

                    count++;
                }
            }
            argv[count] = NULL;
        }
    }
    else
    {
        // MAY CONFLICT WITH FABRIC
        size_t count_classpath = 1;
        size_t size = 0;
        char* cp = NULL;
        int cur;

        size += strlen(args.client);
        for (int i = 0; args.classpath[i] != NULL; i++)
        {
            count_classpath++;
            for (int j = 0; args.classpath[i][j] != '\0'; j++)
                size++;
        }
        cp = malloc(sizeof(char) * (size + count_classpath));
        cur = 0;

        for (int j = 0; args.client[j] != '\0'; j++)
            cp[cur++] = args.client[j];
        for (int i = 0; args.classpath[i] != NULL; i++)
        {
            cp[cur++] = CLASSSEPARATOR;
            for (int j = 0; args.classpath[i][j] != '\0'; j++)
                cp[cur++] = args.classpath[i][j];
        }
        cp[cur] = '\0';

        int tmp_size = strlen(args.natives_directory) + 21;
        argv = malloc(sizeof(char*) * 4);
        argv[0] = malloc(sizeof(char) * tmp_size);
        snprintf(argv[0], tmp_size, "-Djava.library.path=%s", args.natives_directory);

        tmp_size = 4;
        argv[1] = malloc(sizeof(char) * tmp_size);
        strcpy(argv[1], "-cp");

        argv[2] = cp;
        argv[3] = NULL;
    }
    
    free(classseparator);
	return argv;
}

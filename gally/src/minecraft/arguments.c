#include "cjson/cJSON.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minecraft/arguments.h"

JvmArgs mc_InitJvmArgs()
{
	JvmArgs args;
	args.classpath = "NULL";
	args.launcher_name = "gally";
	args.natives_directory = "NULL";
	args.launcher_version = "0.1";
	return args;
}

GameArgs mc_InitGameArgs()
{
	GameArgs args;
	args.auth_player_name = "steve";
	args.game_directory = "\".\"";
	args.assets_root = "assets";
	args.assets_index_name = "NULL";
	args.auth_uuid = "NULL";
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

char* mc_GetGameArgs(cJSON* manifest, GameArgs args)
{
	char *gameArguments = NULL;

	cJSON* i = NULL;
	cJSON* gameArgJson = cJSON_GetObjectItemCaseSensitive(manifest, "arguments");
	size_t len_gameArguments = 1;
	
	if (gameArgJson)
	{
		gameArgJson = cJSON_GetObjectItemCaseSensitive(gameArgJson, "game");
		if (gameArgJson)
		{
			cJSON_ArrayForEach(i, gameArgJson)
			{
				if (i->valuestring)
				{
					if (strcmp("${auth_player_name}",i->valuestring) == 0)
						len_gameArguments += strlen(args.auth_player_name) + 1;
					else if (strcmp("${version_name}",i->valuestring) == 0)
						len_gameArguments += strlen(args.version) + 1;
					else if (strcmp("${game_directory}",i->valuestring) == 0)
						len_gameArguments += strlen(args.game_directory) + 1;
					else if (strcmp("${assets_root}",i->valuestring) == 0)
						len_gameArguments += strlen(args.assets_root) + 1;
					else if (strcmp("${assets_index_name}",i->valuestring) == 0)
						len_gameArguments += strlen(args.assets_index_name) + 1;
					else if (strcmp("${auth_uuid}",i->valuestring) == 0)
						len_gameArguments += strlen(args.auth_uuid) + 1;
					else if (strcmp("${auth_access_token}",i->valuestring) == 0)
						len_gameArguments += strlen(args.auth_access_token) + 1;
					else if (strcmp("${clientid}",i->valuestring) == 0)
							len_gameArguments += strlen(args.clientid) + 1;
					else if (strcmp("${auth_xuid}",i->valuestring) == 0)
						len_gameArguments += strlen(args.auth_xuid) + 1;
					else if (strcmp("${user_type}",i->valuestring) == 0)
						len_gameArguments += strlen(args.user_type) + 1;
					else if (strcmp("${version_type}",i->valuestring) == 0)
						len_gameArguments += strlen(args.version_type) + 1;
					else
						len_gameArguments += strlen(i->valuestring) + 1;
				}			
			}
			gameArguments = malloc(sizeof(char) * len_gameArguments);
			strcpy(gameArguments, "");
			cJSON_ArrayForEach(i, gameArgJson)
			{
				if (i->valuestring)
				{
					/* remove_spaces(i->valuestring); */
					if (strcmp("${auth_player_name}", i->valuestring) == 0)
						strncat(gameArguments, args.auth_player_name, len_gameArguments);
					else if (strcmp("${version_name}", i->valuestring) == 0)
						strncat(gameArguments, args.version, len_gameArguments);
					else if (strcmp("${game_directory}", i->valuestring) == 0)
						strncat(gameArguments, args.game_directory, len_gameArguments);
					else if (strcmp("${assets_root}", i->valuestring) == 0)
						strncat(gameArguments, args.assets_root, len_gameArguments);
					else if (strcmp("${assets_index_name}", i->valuestring) == 0)
						strncat(gameArguments, args.assets_index_name, len_gameArguments);
					else if (strcmp("${auth_uuid}", i->valuestring) == 0)
						strncat(gameArguments, args.auth_uuid, len_gameArguments);
					else if (strcmp("${auth_access_token}", i->valuestring) == 0)
						strncat(gameArguments, args.auth_access_token, len_gameArguments);
					else if (strcmp("${clientid}", i->valuestring) == 0)
						strncat(gameArguments, args.clientid, len_gameArguments);
					else if (strcmp("${auth_xuid}", i->valuestring) == 0)
						strncat(gameArguments, args.auth_xuid, len_gameArguments);
					else if (strcmp("${user_type}", i->valuestring) == 0)
						strncat(gameArguments, args.user_type, len_gameArguments);
					else if (strcmp("${version_type}", i->valuestring) == 0)
						strncat(gameArguments, args.version_type, len_gameArguments);
					else
						strncat(gameArguments, i->valuestring, len_gameArguments);
					strncat(gameArguments, " ", len_gameArguments);
				}
			}
		}
	} 
	else
	{
		gameArgJson = cJSON_GetObjectItemCaseSensitive(manifest, "minecraftArguments");	
		if (cJSON_IsString(gameArgJson))
		{
			i = gameArgJson;
			//remove_spaces(i->valuestring);
			if (strstr(i->valuestring, "${auth_player_name}"))
				i->valuestring = str_replace(i->valuestring, "${auth_player_name}", args.auth_player_name);
			if (strstr(i->valuestring, "${user_properties}"))
				i->valuestring = str_replace(i->valuestring, "${user_properties}", args.user_properties);
			if (strstr(i->valuestring, "${version_name}"))
				i->valuestring = str_replace(i->valuestring, "${version_name}", args.version);
			if (strstr(i->valuestring, "${game_directory}"))
				i->valuestring = str_replace(i->valuestring, "${game_directory}", args.game_directory);
			if (strstr(i->valuestring, "${assets_root}"))
				i->valuestring = str_replace(i->valuestring, "${assets_root}", args.assets_root);
			if (strstr(i->valuestring, "${game_assets}"))
				i->valuestring = str_replace(i->valuestring, "${game_assets}", args.assets_root);
			if (strstr(i->valuestring, "${assets_index_name}"))
				i->valuestring = str_replace(i->valuestring, "${assets_index_name}", args.assets_index_name);
			if (strstr(i->valuestring, "${auth_uuid}"))
				i->valuestring =str_replace (i->valuestring, "${auth_uuid}", args.auth_uuid);
			if (strstr(i->valuestring, "${auth_access_token}"))
				i->valuestring = str_replace(i->valuestring, "${auth_access_token}", args.auth_access_token);
			if (strstr(i->valuestring, "${clientid}"))
				i->valuestring = str_replace(i->valuestring, "${clientid}", args.clientid);
			if (strstr(i->valuestring, "${auth_xuid}"))
				i->valuestring = str_replace(i->valuestring, "${auth_xuid}", args.auth_xuid);
			if (strstr(i->valuestring, "${user_type}"))
				i->valuestring = str_replace(i->valuestring, "${user_type}", args.user_type);
			if (strstr(i->valuestring, "${version_type}"))
				i->valuestring = str_replace(i->valuestring, "${version_type}", args.version_type);
			gameArguments = i->valuestring;
			//realloc(gameArguments, sizeof(char*)*(strlen(i->valuestring)+1));
			//strcpy(gameArguments, i->valuestring);
		}
	}
	return gameArguments;
}

char* mc_GetJvmArgs(cJSON* manifest, JvmArgs args)
{
	char* javaArguments = NULL;
	char* temp = NULL;
	size_t len_javaArguments = 1;
    char *argumentString = NULL;

	cJSON* i = NULL;
    cJSON* jvmArgJson = cJSON_GetObjectItemCaseSensitive(manifest, "arguments");	
	
	if (jvmArgJson)
	{
		jvmArgJson = cJSON_GetObjectItemCaseSensitive(jvmArgJson, "jvm");
		if (jvmArgJson)
		{
			cJSON_ArrayForEach(i, jvmArgJson)
			{
				if (i->valuestring)
				{
                    int isAlloc = 0;
                    size_t len_argumentString = (strlen(i->valuestring)+1);
                    argumentString = malloc(sizeof(char) * len_argumentString);
                    memcpy(argumentString, i->valuestring, len_argumentString);

					remove_spaces(argumentString);
					if (strstr(argumentString, "${natives_directory}"))
						temp = str_replace(argumentString, "${natives_directory}", args.natives_directory);
					else if (strstr(argumentString, "${launcher_name}"))
						temp = str_replace(argumentString, "${launcher_name}", args.launcher_name);
					else if (strstr(argumentString, "${launcher_version}"))
						temp = str_replace(argumentString, "${launcher_version}", args.launcher_version);
					else if (strstr(argumentString, "${classpath}"))
						temp = str_replace(argumentString, "${classpath}", args.classpath);
					else
                    {
						temp = argumentString;
                        isAlloc = 1;
                    }
					len_javaArguments += strlen(temp) + 1;

                    if (isAlloc == 0)
                        free(temp);
                    free(argumentString);
				}			
			}

			javaArguments = malloc(sizeof(char) * len_javaArguments);
			strcpy(javaArguments, "");

			cJSON_ArrayForEach(i, jvmArgJson)
			{
				if (i->valuestring)
				{
                    int isAlloc = 0;
                    size_t len_argumentString = (strlen(i->valuestring)+1);
                    argumentString = malloc(sizeof(char) * len_argumentString);
                    memcpy(argumentString, i->valuestring, len_argumentString);
					remove_spaces(argumentString);

					if (strstr(argumentString, "${natives_directory}"))
						temp = str_replace(argumentString, "${natives_directory}", args.natives_directory);
					else if (strstr(argumentString, "${launcher_name}"))
						temp = str_replace(argumentString, "${launcher_name}", args.launcher_name);
					else if (strstr(argumentString, "${launcher_version}"))
						temp = str_replace(argumentString, "${launcher_version}", args.launcher_version);
					else if (strstr(argumentString, "${classpath}"))
						temp = str_replace(argumentString, "${classpath}", args.classpath);
					else
                    {
						temp = argumentString;
                        isAlloc = 1;
                    }

					strcat(javaArguments, temp);
					strcat(javaArguments, " ");

                    if (isAlloc == 0)
                        free(temp);
                    free(argumentString);
				}
			}
		}
	}
	else
	{
        size_t len_javaArguments = (strlen(args.classpath) + strlen(args.natives_directory) + 26);
		javaArguments = malloc(len_javaArguments * sizeof(char));
        snprintf(javaArguments, len_javaArguments, "-Djava.library.path=%s -cp %s", args.natives_directory, args.classpath);
	}
    /* free(argumentString); */
	/* free(temp); */
	/* free(i); */
	/* free(jvmArgJson); */
    /* cJSON_Delete(jvmArgJson); */
	return javaArguments;
}

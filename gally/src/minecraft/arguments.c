#include "../cjson/cJSON.h"
#include "../utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arguments.h"

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
	char *gameArguments = malloc(sizeof(char*));
	strcpy(gameArguments,"");

	cJSON * i = NULL;
	cJSON * gameArgJson = cJSON_GetObjectItemCaseSensitive(manifest, "arguments");	
	
	if (gameArgJson)
	{
		gameArgJson = cJSON_GetObjectItemCaseSensitive(gameArgJson, "game");
		if (gameArgJson)
		{
			cJSON_ArrayForEach(i, gameArgJson)
			{
				if (i->valuestring)
				{
					remove_spaces(i->valuestring);
					if (strcmp("${auth_player_name}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.auth_player_name)));
						strcat(gameArguments, args.auth_player_name);
					}
					else if (strcmp("${version_name}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.version)));
						strcat(gameArguments, args.version);
					}
					else if (strcmp("${game_directory}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.game_directory)));
						strcat(gameArguments, args.game_directory);
					}
					else if (strcmp("${assets_root}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.assets_root)));
						strcat(gameArguments, args.assets_root);
					}
					else if (strcmp("${assets_index_name}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.assets_index_name)));
						strcat(gameArguments, args.assets_index_name);
					}
					else if (strcmp("${auth_uuid}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.auth_uuid)));
						strcat(gameArguments, args.auth_uuid);
					}
					else if (strcmp("${auth_access_token}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.auth_access_token)));
						strcat(gameArguments, args.auth_access_token);
					}
					else if (strcmp("${clientid}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.clientid)));
						strcat(gameArguments, args.clientid);
					}
					else if (strcmp("${auth_xuid}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.auth_xuid)));
						strcat(gameArguments, args.auth_xuid);
					}
					else if (strcmp("${user_type}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.user_type)));
						strcat(gameArguments, args.user_type);
					}
					else if (strcmp("${version_type}",i->valuestring) == 0)
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(args.version_type)));
						strcat(gameArguments, args.version_type);
					}
					else
					{
						gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+strlen(i->valuestring)+1));
						strcat(gameArguments, i->valuestring);
					}
					gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(gameArguments)+1));
					strcat(gameArguments," ");
				}			
			}
		}
	} else
	{
		gameArgJson = cJSON_GetObjectItemCaseSensitive(manifest, "minecraftArguments");	
		if (cJSON_IsString(gameArgJson))
		{
			i = gameArgJson;
			remove_spaces(i->valuestring);
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
			gameArguments = realloc(gameArguments, sizeof(char*)*(strlen(i->valuestring)+1));
			strcpy(gameArguments, i->valuestring);
		}
	}
	return gameArguments;
}

char* mc_GetJvmArgs(cJSON* manifest, JvmArgs args)
{
	char *javaArguments = malloc(sizeof(char*));
	strcpy(javaArguments,"");

	cJSON * i = NULL;
	cJSON * jvmArgJson = cJSON_GetObjectItemCaseSensitive(manifest, "arguments");	
	
	if (jvmArgJson)
	{
		jvmArgJson = cJSON_GetObjectItemCaseSensitive(jvmArgJson, "jvm");
		if (jvmArgJson)
		{
			cJSON_ArrayForEach(i, jvmArgJson)
			{
				char *temp = NULL;
				if (i->valuestring)
				{
					remove_spaces(i->valuestring);
					if (strstr(i->valuestring, "${natives_directory}"))
					{
						temp = str_replace(i->valuestring, "${natives_directory}", args.natives_directory);
						javaArguments = realloc(javaArguments, sizeof(char*)*(strlen(javaArguments)+strlen(temp)));
						strcat(javaArguments, temp);
					}
					else if (strstr(i->valuestring,"${launcher_name}"))
					{
						temp = str_replace(i->valuestring, "${launcher_name}", args.launcher_name);
						javaArguments = realloc(javaArguments, sizeof(char*)*(strlen(javaArguments)+strlen(temp)));
						strcat(javaArguments, temp);
					}
					else if (strstr(i->valuestring, "${launcher_version}"))
					{
						temp = str_replace(i->valuestring, "${launcher_version}", args.launcher_version);
						javaArguments = realloc(javaArguments, sizeof(char*)*(strlen(javaArguments)+strlen(temp)));
						strcat(javaArguments, temp);
					}
					else if (strstr(i->valuestring, "${classpath}"))
					{
						temp = str_replace(i->valuestring, "${classpath}", args.classpath);
						javaArguments = realloc(javaArguments, sizeof(char*)*(strlen(javaArguments)+strlen(temp)));
						strcat(javaArguments, temp);
					}
					else
					{
						javaArguments = realloc(javaArguments, sizeof(char*)*(strlen(javaArguments)+strlen(i->valuestring)+1));
						strcat(javaArguments, i->valuestring);
					}
					javaArguments = realloc(javaArguments, sizeof(char*)*(strlen(javaArguments)+1));
					strcat(javaArguments," ");
				}			
			}
		}
	}
	else
	{
		javaArguments = realloc(javaArguments, (strlen(args.classpath) + strlen(args.natives_directory) + 25) * sizeof(char*));
		strcat(javaArguments, "-Djava.library.path=");
		strcat(javaArguments, args.natives_directory);
		strcat(javaArguments, " -cp ");
		strcat(javaArguments, args.classpath);
	}
	return javaArguments;
}

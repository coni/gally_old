#include "getopt.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ArgOpt getopt_Parse(int argc, char* argv[])
{
    ArgOpt argopt = {NULL, NULL, 0, 0};
    Opt options[] = {
       /* value                 shortname   longname        type    required    description */
        { &argopt.version,      "v",        "version",      0,      1,          "Specify the version of the game" },
        { &argopt.username,     "u",        "username",     0,      0,          "Specify the username in-game" },
        { &argopt.skip_assets,  "sa",       "skip_assets",  1,      0,          "Don't download assets" },
        { &argopt.login_microsoft,  "lm",   "login-microsoft",  1,      0,          "Login into a Microsoft Account" },
    };

    int i = 1;
    Opt* ptr = NULL; // Point to the current element in the option list
    Opt* end_ptr = options + sizeof(options)/sizeof(options[0]);
    while (i < argc)
    {
        char* option_name = NULL;
        int flag_type = 0; // default = 0,  short = 1, long = 2
        char* value = NULL;
        int len_arg = 0;
        int skip = 0;
        int unknown_type = 1;
        int delimiter = -1;
        ptr = NULL; // Point to the current element in the option list

        for (; argv[i][len_arg] != '\0'; len_arg++);
        if (len_arg > 1)
        {
            if (argv[i][0] == '-')
                flag_type = 1;
            if (len_arg > 2)
            {
                if (argv[i][1] == '-')
                    flag_type = 2;
            }
        }

        if (flag_type > 0)
        {
            // using flag_type as the beginninig of the cursor
            for (delimiter = flag_type; argv[i][delimiter] != '=' && delimiter < len_arg; delimiter++);
            option_name = str_cpyrange(argv[i], flag_type, delimiter-flag_type);
            for (ptr = options; ptr < end_ptr && unknown_type == 1; ptr++)
            { 
                if (flag_type == 1 && strcmp(ptr->shortname, option_name) == 0 ||
                        flag_type == 2 && strcmp(ptr->longname, option_name) == 0)
                {
                    unknown_type = 0;
                    // COMPARER LES SHORTNAME ET LONGNAME
                    if (argv[i][delimiter] == '=') // Voluntary give every option with an equal a value
                    {
                        char* tmp_value = (argv[i]+delimiter+1);
                        value = tmp_value;
                    }
                    else if (ptr->type == 0 && value == NULL)
                    {
                        if (i + 1 < argc)
                            value = argv[i+1];
                        skip += 1;
                    }
                    ptr--;
                }
            }
        }
        
        if (flag_type > 0)
        {
            if (unknown_type)
                printf("UNKNOWN PARAMETER %s\n", option_name);
            else
            {
                if (ptr->type == 0)
                {
                    if (value == NULL)
                        printf("MISSING PARAMETER FOR %s\n", option_name);
                    else
                    {
                        char** tmp_value = ptr->value;
                        *tmp_value = value;
                    }
                }
                else
                {
                    if (value != NULL)
                        printf("VALUE WAS GIVEN FOR TYPE FLAG\n");
                    else
                    {
                        int* tmp_value = ptr->value;
                        *tmp_value = 1;
                    }
                }
            }
            free(option_name);
        }
        else
            printf("NO FLAG %s\n", argv[i]);


        i += 1 + skip;
    }
    for (ptr = options; ptr < end_ptr; ptr++)
    {
        char** tmp_val = ptr->value;
        if (ptr->required && *tmp_val == NULL)
            printf("MISSING REQUIRED PARAMETERS --%s [-%s]\n", ptr->longname, ptr->shortname);
    }
    return argopt;
}

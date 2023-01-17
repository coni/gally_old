#include "getopt.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ArgOpt getopt_Parse(int argc, char* argv[])
{
    ArgOpt argopt = {NULL, NULL, 0};
    Opt options[] = {
       /* value                 shortname   longname        shortname   type    required    description */
        { &argopt.version,      "v",        "version",        0,      1,          "Specify the version of the game" },
        { &argopt.username,     "u",        "username",        0,      0,          "Specify the username in-game" },
        { &argopt.skip_assets,  "sa",       "skip_assets",       1,      0,          "Don't download assets" },
    };

    int i = 1;
    while (i < argc)
    {
        char* option_name = NULL;
        int flag_type = 0; // default = 0,  short = 1, long = 2
        char* value = NULL;
        int len_arg = 0;
        int unknown_type = 1;
        int delimiter = -1;

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
            Opt* end_ptr = options + sizeof(options)/sizeof(options[0]);
            for (Opt* ptr = options; ptr < end_ptr; ptr++)
            { 
                if (flag_type == 1 && strcmp(ptr->shortname, option_name) == 0 ||
                        flag_type == 2 && strcmp(ptr->longname, option_name) == 0)
                {
                    unknown_type = 0;
                    // COMPARER LES SHORTNAME ET LONGNAME
                    if (ptr->type == 1 && argv[i][delimiter] == '=')
                        printf("WAS GIVEN A VALUE FOR A FLAG TYPE\n");
                    else
                    {
                        if (argv[i][delimiter] == '=')
                            value = str_cpyrange(argv[i], delimiter+1, len_arg-delimiter-1);
                        else
                        {
                            if (i + 1 < argc)
                                value = argv[i+++1];
                        }
                    }
                }
            }
        }
        

        if (flag_type > 0)
        {
            if (unknown_type)
                printf("UNKNOWN %s\n", option_name);
            else
            {
                printf("OPTION %s\n", option_name); 

                if (value != NULL)
                    printf("VALUE %s\n", value);
            }
        }
        else
            printf("NOFLAG %s\n", argv[i]);

        if (delimiter > -1 && argv[i][delimiter] == '=')
            free(value);
        i++;
    }
    return argopt;
}

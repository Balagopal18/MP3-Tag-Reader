/*
NAME: BALAGOPAL R NAYAK
START DATE: 17-11-2025
END DATE: 21-11-2025
DESCRIPTION: MP3 tag reader is a software which will read and display MP3 (ID3) tag information from MP3 files. The software will be desktop based and not web based. This solution will read the given MP3 file, extract various tag information and display them via command line. This project can be extended to implement a tag editor, where-in users can modify mp3 tag information.
PRE-REQUISITE:  Function Pointer
                String Operation
                File I/O Handling
*/


//this is a common file for both view and edit operation 
#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"
#include "common.h"


OperationType check_operation_type(char* argv[])            //function to determine which opertion the user has opted
{
    if (strcmp(argv[1],"-v") == 0)             //if user optes for view
        return e_view;
    else if (strcmp(argv[1], "-e") == 0)       //if user optes for edit
        return e_edit;
    else if (strcmp(argv[1],"--help") ==0)     //if user optes for help
        return e_help;
    else
        return e_unsupported;                  //if user passes any other flags
}

int main(int argc, char* argv[])       //main starts here 
{
    if(argc == 1)     //validating the CLA's
    {
        printf("To view please pass like: ./a.out -v mp3filename\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");
        return 1;
    }

    if(check_operation_type(argv) == e_help)
    {
        printf("Usage:\n");
        printf("-t  Modifies a Title tag\n");
        printf("-T  Modifies a Track tag\n");
        printf("-a  Modifies an Artist tag\n");
        printf("-A  Modifies an Album tag\n");
        printf("-y  Modifies a Year tag\n");
        printf("-c  Modifies a Comment tag\n");
        return 1;
    }

    else if(check_operation_type(argv) == e_view)
    {
        Info info;         //creating the structure variable for Info structure
        if(argc == 3)
        {
            if(read_and_validate_view_argc(argv,&info) == e_failure)           //validating the CLA's
            {
                return 1;
            }
            else
            {
                //printf("read and validate fn success\n");
                if(start_view(&info) == e_failure)              //if CLA arguments are validated , the start view operation
                {
                    return 0;
                }
            }
        }
        else
        {
            printf("To view please pass like: ./a.out -v mp3filename\n");      //error message
            return 1;
        }
    }

    else if(check_operation_type(argv) == e_edit)
    {
        EditInfo editinfo;           //creating the structure variable for EditInfo structure
        if(argc == 5)
        {
            if(read_and_validate_edit_arg(argv,&editinfo) == e_failure)          //validating the CLA's
            {
                return 1;
            }
            else
            {
                if(start_edit(&editinfo) == e_failure)             //if CLA arguments are validated , the start edit operation
                { 
                    return 0;
                }
            }
        }
        else{
            printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");       //error message
        }
    }
    
    else                //error messages
    {
        printf("To view please pass like: ./a.out -v mp3filename\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");
        return 1;
    }
}               //end of main
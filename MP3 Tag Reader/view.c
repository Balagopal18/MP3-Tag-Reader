
/* VIEW FUNCTIONS DEFINITION */

#include <stdio.h>
#include <string.h>
#include "view.h"
#include "common.h"
#include <stdlib.h>

Status read_and_validate_view_argc(char* argv[], Info *info)              //function to read and validate view arguments
{
    if(argv[2] == NULL)       //if user passed nothing
    {
        printf("To view please pass like: ./a.out -v mp3filename\n");
        return e_failure;
    }
    char* mp3_extn = strstr(argv[2] , ".mp3");                      //checking for file extension(.mp3) 
    if(mp3_extn == NULL || strcmp(mp3_extn , ".mp3") != 0)
    {
        printf("To view please pass like: ./a.out -v mp3filename\n");           //if not (.mp3) file then display error message
        return e_failure;
    }
    info->src_file_name = argv[2];                  //storing the source(.mp3) file to structure variable
    return e_success;
}

Status open_files_view(Info* info)                 //function to open files
{
    info->fptr_src_file = fopen(info->src_file_name , "r");        //open the source file in read mode
    if(info->fptr_src_file == NULL)
    {
        printf("ERROR: Opening of %s file failed\n",info->src_file_name);         //if file opening failed, display error message
        return e_failure;
    }
    return e_success;
}

Status validate_src_file(Info* info)                //function to validate the mp3 file version
{
    char arr[4];
    fread(arr,1,3,info->fptr_src_file);             //reading first 3 bytes from header(contains the version)
    //printf("Tag id  = %s\n",arr);
    if(strcmp(arr,"ID3") == 0)                      //if mp3 version is ID3 or not
    {
        return e_success;
    }
    return e_failure;
}

Status skip_header(Info* info)                      //function to skip the header 
{
    rewind(info->fptr_src_file);                    //bringing file pointer to first position
    fseek(info->fptr_src_file,10,SEEK_SET);         //skipping header
    //printf("position = %ld\n",ftell(info->fptr_src_file));
    if(ftell(info->fptr_src_file) == 10)
    {
        return e_success;
    }
    return e_failure;
}

Status read_and_validate_tag(Info* info)                 //function to validate tags
{
    char* tag = (char*)malloc(5*sizeof(char));           //creating array dynamically
    fread(tag,1,4,info->fptr_src_file);                  //reading 4bytes from source file
    tag[4] = '\0';
    //printf("tag = %s\n",tag);

    if (strcmp(tag,"TIT2") == 0)                  //if tag is TIT2
    {
        info->tag_name = "Title";                 //display as "title" in terminal 
        printf("%-20s",info->tag_name);

    }
    else if (strcmp(tag,"TPE1") ==0)              //if tag is TPE1
    {
        info->tag_name = "Artist name";           //display as "artist name" in terminal
        printf("%-20s",info->tag_name);   

    }
    else if (strcmp(tag,"TALB") ==0)              //if tag is TALB
    {
        info->tag_name = "Album";                 //display as "album" in terminal
        printf("%-20s",info->tag_name);

    }
    else if (strcmp(tag,"TYER") ==0)              //if tag is TYER
    {
        info->tag_name = "Year";                  //display as "year" in terminal
        printf("%-20s",info->tag_name);

    }
    else if (strcmp(tag,"TCON") ==0)              //if tag is TCON
    {
        info->tag_name = "Content Type";          //display as "content type" in terminal
        printf("%-20s",info->tag_name);

    }
    else if (strcmp(tag,"COMM") ==0)              //if tag is COMM
    {
        info->tag_name = "Composer";              //display as "composer" in terminal
        printf("%-20s",info->tag_name);

    }

    else
    {
        printf("tag not read\n");             
    }

    if(size_of_tag(info) == e_success)            //function call to read the size of tag's content
    {
        fseek(info->fptr_src_file,3,SEEK_CUR);          //skip next 3 bytes(2-flags + 1-'\0')
        char content[info->tag_size -1];            
        fread(content,1,info->tag_size -1,info->fptr_src_file);          //read size bytes from source file and store in content array
        content[info->tag_size-1] = '\0';         //store null at last
        printf(": %s\n",content);                 //display the content in array(that is read from source file)
        
    }
    free(tag);                        //free the dynamically created array
    return e_success;
}

Status size_of_tag(Info* info)                //function to get the size of tag's content
{
    int size[1];
    fread(size,1,4,info->fptr_src_file);      //read 4bytes after tag
    //printf("size = %d\n",*size);
    
    convert_big_to_little_endian(&size[0]);        //convert big endian to little endian - to get size
    info->tag_size = size[0];                 //store the size
    //printf("new size = %d\n",info->tag_size);

    return e_success;
}

void convert_big_to_little_endian(int* arr)             //function to convert big endian to little endian format
{
    char* p = (char*) arr;
    char temp;
    temp = p[0];
    p[0] = p[3];
    p[3] = temp;

    temp = p[1];
    p[1] = p[2];
    p[2] = temp;
}

Status start_view(Info* info)                         //function that calls all other sub-functions as a part - task-start view
{
    if(open_files_view(info) == e_success)
    {
        if(validate_src_file(info) == e_success)
        {
            printf("-------------------------------------------------------------------------\n");
            printf("MP3 Tag Reader and Editor for IDV3v2\n");
            printf("-------------------------------------------------------------------------\n");
            if(skip_header(info) == e_success)
            {
                int i=0;
                for( ; i<6 ; i++)                          //loop runs till 6(changable - depends on how much details user wants) 
                { 
                    if(read_and_validate_tag(info) == e_failure)
                        break;
                }
                if(i==6)
                {
                    printf("-------------------------------------------------------------------------\n");
                    return e_success;             //if sub-functions are executed file, then return 1 to ain function
                }
            }
        }
    }
    return e_failure;             //if any one sub-function fail then return 0 to main function
}
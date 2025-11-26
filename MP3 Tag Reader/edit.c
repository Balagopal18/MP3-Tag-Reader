
/* EDIT FUNCTIONS DEFINITION */

#include <stdio.h>
#include <string.h>
#include "edit.h"
#include "common.h"
#include <stdlib.h>

Status read_and_validate_edit_arg(char* argv[], EditInfo* editinfo)           //function to read and validate edit arguments
{
    if(argv[2] == NULL)               //if user doesn't enter any arguments
    {
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");       //display error message
        return e_failure;
    }
    if(strcmp(argv[2],"-t") == 0 || strcmp(argv[2],"-a") == 0 || strcmp(argv[2],"-A") == 0 || strcmp(argv[2],"-m") == 0 || strcmp(argv[2],"-y") == 0 || strcmp(argv[2],"-c") == 0 )
    {
        editinfo->flag = argv[2];               //store the flag in structure member
    }
    else
    {
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");       //display error message
        return e_failure;
    }

    if(argv[3] == NULL)                //if user doesn't enter any arguments
    {
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");      //display error message
        return e_failure;
    }
    editinfo->new_value = argv[3];     //store the new value in structure member

    if(argv[4] == NULL)                //if user doesn't enter any arguments
    {
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");      //display error message
        return e_failure;
    }
    char* mp3_extn = strstr(argv[4] , ".mp3");    //check if input file is (.mp3) file or not
    if(mp3_extn == NULL || strcmp(mp3_extn , ".mp3") != 0)
    {
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <new_value> mp3filename\n");      //display error message
        return e_failure;
    }
    editinfo->original_file_name = argv[4];       //store original file name in structure member
    return e_success;
}

Status start_edit(EditInfo* editinfo)             //function to start edit operation
{
    if(open_files_edit(editinfo) == e_success)
    {
        //printf("file opened success\n");
        printf("--------------------Select Edit Option---------------------\n");
        if(copy_header_edit(editinfo) == e_success)
        {
            if(compare_flag_and_tag(editinfo) == e_success)
            {
                //printf("hi\n");
                if(copy_temp_to_original_file(editinfo) == e_success)
                    return e_success;          //return success, if all sub function are success
            }
        }
    }
    return e_failure;         //return failure, if any one sub function fails
}

Status open_files_edit(EditInfo* editinfo)             //function to open files for edit operation
{
    editinfo->fptr_original_file = fopen(editinfo->original_file_name , "r+");            //open input audio file in r+ mode...so that we can read and write from same file 
    if(editinfo->fptr_original_file == NULL)           
    {
        printf("ERROR: Opening of %s file failed\n",editinfo->original_file_name);        //if file opening fails, display error message
        return e_failure;
    }

    editinfo->fptr_temp_file = fopen("temp.mp3" , "w+");                    //open input audio file in w+ mode...so that we can read and write from same file
    if(editinfo->fptr_temp_file == NULL)
    {
        printf("ERROR: Opening of %s file failed\n",editinfo->original_file_name);       //if file opening fails, display error message
        return e_failure;
    }
    return e_success;
}

Status copy_header_edit(EditInfo* editinfo)                  //function to copy the header from source to destination files
{
    char arr[10];
    fread(arr,1,10,editinfo->fptr_original_file);            //reading first 10bytes from source file
    fwrite(arr,1,10,editinfo->fptr_temp_file);               //writing the read content to destination file
    if(ftell(editinfo->fptr_original_file) == ftell(editinfo->fptr_temp_file))
    {
        //printf("header copy successful\n");
        return e_success;
    }
    return e_failure;
}

Status compare_flag_and_tag(EditInfo* editinfo)              //function to compare flag (entered by user) and tag in audio file
{
    char tag[5];
again:  fread(tag,1,4,editinfo->fptr_original_file);         //read tag from source file
    //printf("tag = %s\n",tag);

    if(strcmp(tag,"TIT2") == 0 && strcmp(editinfo->flag,"-t")==0)                      // if matching -> -t = TIT2
    {
        printf("---------------Select Title Change Option-----------\n");
        printf("%-10s","Title");
        if(do_edit(editinfo) == e_success)            //function call - to edit the title 
        {
            printf("---------------Title Changed Successfully---------------\n");
            return e_success;
        }
    }
  
    else if(strcmp(tag,"TALB") == 0 && strcmp(editinfo->flag,"-A")==0)                 // if matching -> -A = TALB
    {
        printf("---------------Select Album Change Option---------------\n");
        printf("%-10s","Album");
        if(do_edit(editinfo) == e_success)            //function call - to edit the album 
        {
            printf("---------------Album Changed Successfully---------------\n");
            return e_success;
        }
    }

    else if(strcmp(tag,"TYER") == 0 && strcmp(editinfo->flag,"-y")==0)                 // if matching -> -y = TYER
    {
        printf("---------------Select Year Change Option---------------\n");
        printf("%-10s","Year");
        if(do_edit(editinfo) == e_success)            //function call - to edit the year 
        {
            printf("---------------Year Changed Successfully---------------\n");
            return e_success;
        }
    }

    else if(strcmp(tag,"TCON") == 0 && strcmp(editinfo->flag,"-T")==0)                  // if matching -> -T = TCON
    {
        printf("---------------Select Content Type Change Option---------------\n");
        printf("%-10s","Content Type");
        if(do_edit(editinfo) == e_success)             //function call - to edit the content type
        {
            printf("---------------Content Type Changed Successfully---------------\n");
            return e_success;
        }
    }

    else if(strcmp(tag,"TPE1") == 0 && strcmp(editinfo->flag,"-a")==0)                   // if matching -> -a = TPE1
    {
        printf("---------------Select Artist Change Option---------------\n");
        printf("%-10s","Artist");
        if(do_edit(editinfo) == e_success)             //function call - to edit the artist
        {
            printf("---------------Artist Changed Successfully---------------\n");
            return e_success;
        }
    }

    if(strcmp(tag,"COMM") == 0 && strcmp(editinfo->flag,"-c")==0)                         // if matching -> -c = COMM
    {
        printf("---------------Select Composer Change Option---------------\n");
        printf("%-10s","Composer");
        if(do_edit(editinfo) == e_success)             //function call - to edit the composer
        {
            printf("---------------Composer Changed Successfully---------------\n");
            return e_success;
        }
    }
    else{
        if(skip_tag(editinfo) != e_success)                //if current tag not matching with user entered flag , then call skip function 
        {
            printf("tags are not matching\n");
            return e_failure;
        }
        goto again;                  //keep searching till the flag and tag match
    }
}

void convert_big_to_little_endian_for_edit(int* arr)             //function to convert endian-ness
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

Status do_edit(EditInfo* editinfo)                     //function to edit that particular tag
{
    printf(": %s\n",editinfo->new_value);
    char tag[5];
    fseek(editinfo->fptr_original_file,-4,SEEK_CUR);
    
    fread(tag,1,4,editinfo->fptr_original_file);          //copy tag name from source file
    fwrite(tag,1,4,editinfo->fptr_temp_file);             //write tag to destination file

    int size[1];
    fread(size,1,4,editinfo->fptr_original_file);         //copy the old size from source file

    convert_big_to_little_endian_for_edit(&size[0]);      //get little edian value for the old data
    
    editinfo->old_size = size[0];                         //store the old size

    editinfo->flag_size = strlen(editinfo->new_value)+1;             //getting new value size  

    convert_big_to_little_endian_for_edit(&editinfo->flag_size);     //convert new value string length (size) which is in little endian to big endian format

    int new_size[1];
    new_size[0] = editinfo->flag_size;
    fwrite(new_size,1,4,editinfo->fptr_temp_file);             //write the new size of new value in destination file 

    char flag[3];               
    fread(flag,1,3,editinfo->fptr_original_file);              //copying the flags from source to destination file
    fwrite(flag,1,3,editinfo->fptr_temp_file);

    //printf("%ld %ld\n",ftell(editinfo->fptr_original_file),ftell(editinfo->fptr_temp_file));

    fwrite(editinfo->new_value,1,strlen(editinfo->new_value),editinfo->fptr_temp_file);          //store the new content

    fseek(editinfo->fptr_original_file,editinfo->old_size-1,SEEK_CUR);      //skip content part in original 

    if(copy_remaining_data(editinfo) == e_success)            //function call to copy the remaining data till EOF 
    {
        //printf("copy remaining data success\n");
        return e_success;
    }
    printf("ERROR: Copying remaining data failed\n");
    return e_failure;
}

Status copy_remaining_data(EditInfo* editinfo)                //function to copy the remaining data till EOF 
{
    char ch;
    while(fread(&ch,1,1,editinfo->fptr_original_file) == 1)        //copy character by character from source to destination file
    {
        fwrite(&ch,1,1,editinfo->fptr_temp_file);
    }
    return e_success;
}

Status skip_tag(EditInfo* editinfo)                   //function to skip tag if its not matching with flag entered by user
{
    char tag[5];
    fseek(editinfo->fptr_original_file,-4,SEEK_CUR); 
    
    fread(tag,1,4,editinfo->fptr_original_file);         //copy tag name from source to destination file
    fwrite(tag,1,4,editinfo->fptr_temp_file);

    int size[1];
    fread(size,1,4,editinfo->fptr_original_file);
    fwrite(size,1,4,editinfo->fptr_temp_file);           //copy size from source to destination file
    convert_big_to_little_endian_for_edit(&size[0]);

    char flag[3];
    fread(flag,1,3,editinfo->fptr_original_file);       
    fwrite(flag,1,3,editinfo->fptr_temp_file);          //copy flags from source to destination file

    char content[size[0]];
    fread(content,1,size[0]-1,editinfo->fptr_original_file);
    fwrite(content,1,size[0]-1,editinfo->fptr_temp_file);            //copy content from source to destination file

    return e_success;
}

Status copy_temp_to_original_file(EditInfo* editinfo)              //function to copy the edited data back to source file
{
    rewind(editinfo->fptr_original_file);            //bring source file pointer to start
    rewind(editinfo->fptr_temp_file);                //bring destination file pointer to start
    char ch;
    while(fread(&ch,1,1,editinfo->fptr_temp_file) ==1)
    {
        fwrite(&ch,1,1,editinfo->fptr_original_file);         //copy character by character
    }
    return e_success;
}
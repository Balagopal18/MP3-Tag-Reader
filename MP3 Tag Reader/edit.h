/* HEADER FILE FOR EDIT OPERATION */
#ifndef HEADER_EDIT
#define HEADER_EDIT

#include "common.h"

typedef struct _EditInfo
{
    /* original file members */
    char* original_file_name;            //to store original file name 
    FILE* fptr_original_file;            //to store original file pointer

    /* temporary file members */
    char* temp_file_name;                //to store temporary file name
    FILE* fptr_temp_file;                //to store temporary file pointer

    char* flag;                   //to store the flag opted by user
    int old_size;                 //to store the old size thats in original file
    char* new_value;              //to store the new value entered by user
    int flag_size;                //to store the size of new value
    char* tag_content;            //to store the content in that tag

}EditInfo;

#endif

Status read_and_validate_edit_arg(char* argv[], EditInfo *editinfo);         //function to read and validate the edit arguments
Status start_edit(EditInfo *editinfo);                        //function to start edit operation
Status open_files_edit(EditInfo *editinfo);                   //function to open files
Status copy_header_edit(EditInfo* editinfo);                  //function to copy the header from original to temp file
Status compare_flag_and_tag(EditInfo* editinfo);              //function to compare user entered flag and tag in mp3
Status do_edit(EditInfo* editinfo);                           //function to edit tag content
void convert_big_to_little_endian_for_edit(int* arr);         //function to convert endian-ness
Status copy_remaining_data(EditInfo* editinfo);               //function to copy the remaining data after editing the content of that tag
Status skip_tag(EditInfo* editinfo);                          //function to skip tag till next tag
Status copy_temp_to_original_file(EditInfo* editinfo);        //function to copy temp file content back to original mp3 file
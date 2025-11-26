/* HEADER FILE FOR VIEW OPERATION */
#ifndef HEADER_H
#define HEADER_H

#include "common.h"

typedef struct _Info
{
    /* source file */
    char* src_file_name;   //to store the source file
    FILE* fptr_src_file;   //to store the source file pointer

    char* tag_name;        //to store the tag
    int tag_size;          //to store the content size
    char* tag_content;     //to store the content

}Info;

#endif

OperationType check_operation_type(char* argv[]);                   //function to check operationtype chosen by user
Status read_and_validate_view_argc(char* argv[], Info *info);       //function to read and validate CLA's arguments
Status open_files_view(Info* info);                 //function to open files
Status validate_src_file(Info* info);               //function to validate the version of mp3 file       
Status start_view(Info* info);                      //function to start the view operation function
Status skip_header(Info* info);                     //function to skip the header
Status read_and_validate_tag(Info* info);           //function to read and validate the tag 
Status size_of_tag(Info* info);                     //function to get the size tag's content
void convert_big_to_little_endian(int* arr);        //function to convert big to little endian


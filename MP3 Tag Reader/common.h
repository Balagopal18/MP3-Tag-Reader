/* Common file for both edit and view options */

#ifndef COMMON_H
#define COMMON_H

typedef enum            //to determine the whether a operation is success or failure
{
    e_failure ,
    e_success
}Status;

typedef enum           //to classifies the operation type
{
    e_help,
    e_view,
    e_edit,
    e_unsupported

}OperationType;

#endif
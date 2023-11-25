#ifndef ERROR_HAND_H
#define ERROR_HAND_H
#include <stdint.h>
#include "book.h"
#define EXIT true
#define NOEXIT false

/* __enum_ERR   */
enum ErrorCodes {
    ERR_MALLOC = 1,
    ERR_FOPEN,
    ERR_FWRITE,
    ERR_FNAME,
    ERR_GET,
    ERR_INPUT,
};
/* Contains Error Messages  */
extern const char *errorMessages[][2];

/* Print specific error and exit 
 
syn = (__enum_ERR, "__ptr_name")   */
void print_succ(uint32_t succCode, char *varName);

/* Print specific error and exit 
 
syn = (__enum_ERR, "__ptr_name")   */
void print_error(uint32_t errorCode, char *varName, bool IFEXIT);
#endif

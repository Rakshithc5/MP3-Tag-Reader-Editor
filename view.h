#ifndef VIEW_H
#define VIEW_H

#include "types.h"

/* MP3 file information */
typedef struct TagInfo
{
    FILE *fp_mp3;      // Pointer to MP3 file
} TagInfo;

/* Function declarations */
OperationType check_operation(char *argv[]);                               // Determine requested operation
Status read_and_validate_mp3_file(char *argv[], TagInfo *tag_info);        // Validate MP3 file and ID3 header
Status view_tag(char *argv[], TagInfo *tag_info);                          // Display MP3 tag information

#endif
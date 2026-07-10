#ifndef EDIT_H
#define EDIT_H

#include "types.h"

/* MP3 tag editing information */
typedef struct _TagData
{
    FILE *fp_mp3;                    // Source MP3 file pointer
    FILE *fp_temp;                   // Temporary file pointer

    char frame_Id[5];                // Frame ID (e.g., TIT2, TPE1)
    char frame_Id_value[100];        // New frame value

    uint frame_Id_size;              // Size of frame data

} TagData;

/* Function declarations */
Status read_and_validate_mp3_file_args(char *argv[], TagData *tag_data); // Validate command-line arguments and MP3 file

Status edit_tag(char *argv[], TagData *tag_data); // Edit the selected MP3 tag

Status copy_data(TagData *tag_data, char flag); // Copy data up to the target frame

Status copy_remaining_data(TagData *tag_data); // Copy remaining MP3 data

Status copy_back_to_src(TagData *tag_data); // Replace original file with updated file

#endif
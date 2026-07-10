/* 
===========================================================
                 MP3 TAG READER & EDITOR
===========================================================

Name        : Rakshith C
Date        : 07/06/2026
Batch No    : 26001B

Description :
-------------
This project implements an MP3 Tag Reader and Editor in C
using the ID3v2.3 metadata format. It allows users to
view and modify MP3 tag information through command-line
arguments. The program reads metadata frames such as
Title, Artist, Album, Year, Genre, and Comments directly
from MP3 files. It validates file format, updates selected
tag information, and preserves the remaining audio data
without modification.

Features:
---------
1. MP3 Tag Viewing
   - Reads and validates MP3 files
   - Verifies ID3v2.3 signature
   - Displays Title, Artist, Album, Year,
     Genre, and Comment information
   - Presents tag details in a readable format

2. MP3 Tag Editing
   - Updates selected metadata frames
   - Supports modification of:
       * Title (TIT2)
       * Artist (TPE1)
       * Album (TALB)
       * Year (TYER)
       * Genre (TCON)
       * Comment (COMM)
   - Preserves remaining MP3 content

3. Command-Line Based Execution
   - Uses arguments for viewing and editing
   - Example:
       ./a.out -v song.mp3
       ./a.out -e -t "New Title" song.mp3
       ./a.out -e -a "New Artist" song.mp3

4. File Handling
   - Opens and validates MP3 files
   - Creates temporary files during editing
   - Copies updated data safely
   - Replaces original file after successful update

5. Validation
   - Verifies command-line arguments
   - Checks for valid MP3 file signature
   - Validates supported frame modifiers
   - Ensures input data length is within limits

6. Error Handling
   - Handles invalid arguments
   - Handles unsupported modifiers
   - Handles file opening failures
   - Handles invalid MP3 files
   - Displays meaningful success and error messages

===========================================================
*/
#include <stdio.h>

#include "types.h"
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{
    TagInfo tag_info;                 // Structure for view operation
    TagData tag_data;                 // Structure for edit operation

    if (argc < 2)
    {
        printf("ERROR: Incorrect formate of command Line arguments\n");
        printf("INFO: ./a.out -v or -e followed by file name\n");

        return m_failure;             // Exit if insufficient arguments
    }

    OperationType operation = check_operation(argv); // Determine requested operation

    if (operation == m_view)
    {
        printf("\n================mp3 tag view operation================\n\n");

        if (read_and_validate_mp3_file(argv, &tag_info) == m_success) // Validate MP3 file
        {
            if (view_tag(argv, &tag_info) == m_success) // Display tag information
            {
                printf("INFO: mp3 tag view operation is successfull\n");
                printf("=====================================================\n");
            }
        }
    }
    else if (operation == m_edit)
    {
        printf("================mp3 tag edit operation================\n\n");

        if (read_and_validate_mp3_file_args(argv, &tag_data) == m_success) // Validate edit arguments
        {
            if (edit_tag(argv, &tag_data) == m_success) // Edit MP3 tag
            {
                printf("INFO: mp3 tag edit operation is successfull\n");
                printf("=====================================================\n");
            }
        }
    }
    else if (operation == m_unsupported)
    {
        printf("ERROR: Unsupported Operation.\n");
        printf("INFO: Please use -v for view and -e for edit operations.\n");
    }

    return 0;                         // Program completed
}
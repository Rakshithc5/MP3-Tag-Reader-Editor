#include <stdio.h>
#include <string.h>

#include "types.h"
#include "view.h"

OperationType check_operation(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)   // Check for view operation
    {
        return m_view;
    }
    else if (strcmp(argv[1], "-e") == 0)  // Check for edit operation
    {
        return m_edit;
    }

    return m_unsupported;  // Return unsupported for invalid option
}

Status read_and_validate_mp3_file(char *argv[], TagInfo *tag_info)
{
    if (argv[2] == NULL)   // Verify MP3 file argument is provided
    {
        printf("ERROR: Incorrect format of command line arguments\n");
        printf("INFO : ./a.out -v <file_name.mp3>\n");

        return m_failure;
    }

    tag_info->fp_mp3 = fopen(argv[2], "r");  // Open MP3 file

    if (tag_info->fp_mp3 == NULL)
    {
        printf("ERROR: Unable to open file %s\n", argv[2]);
        return m_failure;
    }

    char signature[3];

    fread(signature, 1, 3, tag_info->fp_mp3);  // Read ID3 signature

    if (strncmp(signature, "ID3", 3) == 0)   // Validate MP3 tag signature
    {
        printf("\n");
        printf("=====================================================\n");
        printf("               MP3 TAG VIEWER                        \n");
        printf("=====================================================\n");
        printf("Version : ID3 v2.3\n");
        printf("=====================================================\n");
    }
    else
    {
        printf("ERROR: Invalid MP3 file signature\n");

        fclose(tag_info->fp_mp3);
        return m_failure;
    }

    fclose(tag_info->fp_mp3);  // Close file after validation

    return m_success;
}

Status view_tag(char *argv[], TagInfo *tag_info)
{
    tag_info->fp_mp3 = fopen(argv[2], "r");  // Reopen MP3 file

    fseek(tag_info->fp_mp3, 10L, SEEK_SET);  // Skip ID3 header

    char *frame_tags[6] =
    {
        "TIT2",
        "TPE1",
        "TALB",
        "TYER",
        "TCON",
        "COMM"
    };

    char *tag_names[6] =
    {
        "Title",
        "Artist",
        "Album",
        "Year",
        "Genre",
        "Composer"
    };

    char frame_id[5] = {0};   // Store frame ID
    int frame_size;           // Store frame size

    for (int index = 0; index < 6; index++)
    {
        fread(frame_id, 4, 1, tag_info->fp_mp3);  // Read frame ID
        frame_id[4] = '\0';

        fread(&frame_size, 4, 1, tag_info->fp_mp3);  // Read frame size

        char *ptr = (char *)&frame_size;

        for (int i = 0; i < 2; i++)   // Convert size to host byte order
        {
            char temp = *(ptr + i);
            *(ptr + i) = *(ptr + 4 - i - 1);
            *(ptr + 4 - i - 1) = temp;
        }

        fseek(tag_info->fp_mp3, 3, SEEK_CUR);  // Skip frame flags

        char frame_content[frame_size - 1];

        fread(frame_content,
              sizeof(frame_content),
              1,
              tag_info->fp_mp3);  // Read frame content

        frame_content[frame_size - 1] = '\0';  // Null terminate string

        printf("+---------------------------------------------------+\n");
        printf("| Frame ID : %-38s |\n", frame_id);
        printf("| Tag Name : %-38s |\n", tag_names[index]);
        printf("| Value    : %-38s |\n", frame_content);
        printf("+---------------------------------------------------+\n\n");
    }

    printf("=====================================================\n");
    printf("            END OF MP3 TAG INFORMATION               \n");
    printf("=====================================================\n");

    fclose(tag_info->fp_mp3);  // Close MP3 file

    return m_success;
}
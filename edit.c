#include <stdio.h>
#include <string.h>
#include "types.h"
#include "edit.h"

#include <stdio.h>
#include <string.h>

#include "types.h"
#include "edit.h"

Status read_and_validate_mp3_file_args(char *argv[], TagData *tag_data)
{
    if (argv[2] == NULL)
    {
       printf("\n");
printf("=========================================\n");
printf("          MP3 TAG EDIT OPTIONS           \n");
printf("=========================================\n");
printf("Usage:\n");
printf("  ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n\n");

printf("Available Modifiers:\n");
printf("  -t   Title Tag\n");
printf("  -A   Artist Tag\n");
printf("  -a   Album Tag\n");
printf("  -y   Year Tag\n");
printf("  -g   Genre Tag\n");
printf("  -c   Composer Tag\n");

printf("=========================================\n");
        return m_failure;   // Return failure if modifier is missing
    }

    if (strncmp(argv[2], "-t", 2) == 0)
    {
        strncpy(tag_data->frame_Id, "TIT2", 5);   // Select Title frame
    }
    else if (strncmp(argv[2], "-A", 2) == 0)
    {
        strncpy(tag_data->frame_Id, "TPE1", 5);   // Select Artist frame
    }
    else if (strncmp(argv[2], "-a", 2) == 0)
    {
        strncpy(tag_data->frame_Id, "TALB", 5);   // Select Album frame
    }
    else if (strncmp(argv[2], "-y", 2) == 0)
    {
        strncpy(tag_data->frame_Id, "TYER", 5);   // Select Year frame
    }
    else if (strncmp(argv[2], "-g", 2) == 0)
    {
        strncpy(tag_data->frame_Id, "TCON", 5);   // Select Genre frame
    }
    else if (strncmp(argv[2], "-c", 2) == 0)
    {
        strncpy(tag_data->frame_Id, "COMM", 5);   // Select Comment frame
    }
    else
    {
        printf("\n");
printf("=========================================\n");
printf("        INVALID MODIFIER SELECTED        \n");
printf("=========================================\n");
printf("ERROR : Unsupported Modifier\n\n");

printf("Supported Modifiers:\n");
printf("  -t   Modify Title Tag\n");
printf("  -A   Modify Artist Tag\n");
printf("  -a   Modify Album Tag\n");
printf("  -y   Modify Year Tag\n");
printf("  -g   Modify Genre Tag\n");
printf("  -c   Modify Composer Tag\n");

printf("=========================================\n");

        return m_failure;   // Return failure for invalid modifier
    }

   printf("[INFO] Updating Frame : %s\n",
       tag_data->frame_Id);   // Display selected frame

    if (argv[3] == NULL)
    {
      printf("\n");
printf("=========================================\n");
printf("            UPDATE FAILED                \n");
printf("=========================================\n");
printf("ERROR : No value provided for Frame ID\n");
printf("Frame : %s\n\n", tag_data->frame_Id);

printf("Usage:\n");
printf("  ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
printf("=========================================\n");
        return m_failure;   // Return failure if new value is missing
    }

    tag_data->frame_Id_size = strlen(argv[3]) + 1;   // Calculate new value size

    if (tag_data->frame_Id_size < 100)
    {
        strncpy(tag_data->frame_Id_value,
                argv[3],
                tag_data->frame_Id_size);   // Copy new frame value

        printf("[INFO] New Value : %s\n",
       tag_data->frame_Id_value);   // Display new value
               
    }
    else
    {
       printf("\n");
printf("=========================================\n");
printf("            UPDATE FAILED                \n");
printf("=========================================\n");
printf("ERROR : Input value exceeds maximum limit\n");
printf("Allowed Length : Less than 100 characters\n");
printf("=========================================\n");
        return m_failure;   // Return failure if value exceeds limit
    }

    if (argv[4] == NULL)
    {
        printf("\n");
printf("=========================================\n");
printf("               USAGE GUIDE               \n");
printf("=========================================\n");
printf("Command:\n");
printf("  ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
printf("=========================================\n");
        return m_failure;   // Return failure if file name is missing
    }

    tag_data->fp_mp3 = fopen(argv[4], "r");   // Open MP3 file

    if (tag_data->fp_mp3 == NULL)
    {
       printf("\n");
printf("=========================================\n");
printf("            FILE OPEN FAILED             \n");
printf("=========================================\n");
printf("ERROR : Unable to open file\n");
printf("File  : %s\n\n", argv[4]);

printf("Usage:\n");
printf("  ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
printf("=========================================\n");
        return m_failure;   // Return failure if file cannot be opened
    }

    char signature[3];

    fread(signature, 1, 3, tag_data->fp_mp3);   // Read MP3 signature

    if (strncmp(signature, "ID3", 3) != 0)
    {
       printf("\n");
printf("=========================================\n");
printf("          INVALID MP3 FILE               \n");
printf("=========================================\n");
printf("ERROR : File signature mismatch\n");
printf("Reason: Selected file is not a valid MP3\n\n");

printf("Usage:\n");
printf("  ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
printf("=========================================\n");
        fclose(tag_data->fp_mp3);   // Close file before exiting
        return m_failure;
    }

    fclose(tag_data->fp_mp3);   // Close validated MP3 file

    return m_success;   // Validation successful
}
Status edit_tag (char* argv[], TagData* mp3tagData){
    mp3tagData->fp_mp3 = fopen (argv [4], "r");   // Open source MP3 file

    printf("Creating the temp file\n");
    mp3tagData->fp_temp = fopen ("temp.mp3", "w");   // Create temporary MP3 file

    fseek (mp3tagData->fp_mp3,0,SEEK_SET);    // Move source file pointer to beginning
    fseek (mp3tagData->fp_temp,0,SEEK_SET);   // Move temp file pointer to beginning

    char str [10];

    fread (str,1,10,mp3tagData->fp_mp3);      // Read ID3 header
    fwrite (str,1,10,mp3tagData->fp_temp);    // Copy ID3 header to temp file

    char* frame_tag [6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};

    for (int i=0;i<6;i++){
        fread (str,1,4,mp3tagData->fp_mp3);       // Read frame ID
        fwrite (str,1,4,mp3tagData->fp_temp);     // Copy frame ID

        if ((strncmp(mp3tagData->frame_Id,frame_tag [i], 4)) == 0){
            if (copy_data (mp3tagData, 1) == m_failure){   // Update selected frame
                printf("\n");
printf("=========================================\n");
printf("          DATA COPY FAILED               \n");
printf("=========================================\n");
printf("ERROR : Failed to copy updated tag data\n");
printf("Source -> Temporary MP3 transfer failed\n");
printf("=========================================\n");
                return m_failure;
            }
        }
        else{
            if (copy_data (mp3tagData, 0) == m_failure){   // Copy unchanged frame
                printf("\n");
printf("=========================================\n");
printf("          DATA COPY FAILED               \n");
printf("=========================================\n");
printf("ERROR : Failed to copy remaining data\n");
printf("Source -> Temporary MP3 transfer failed\n");
printf("=========================================\n");
                return m_failure;
            }
        }
    }

    if (copy_remaining_data (mp3tagData) == m_failure){   // Copy remaining MP3 content
        printf("\n");
printf("=========================================\n");
printf("      REMAINING DATA COPY FAILED         \n");
printf("=========================================\n");
printf("ERROR : Unable to transfer remaining data\n");
printf("From   : Source MP3 File\n");
printf("To     : Temporary MP3 File\n");
printf("=========================================\n");
        return m_failure;
    }

	fclose(mp3tagData->fp_mp3);    // Close source MP3 file
	fclose(mp3tagData->fp_temp);   // Close temporary MP3 file

	printf("\n");
	printf("=========================================\n");
	printf("      REMOVING ORIGINAL MP3 FILE         \n");
	printf("=========================================\n");
	printf("STATUS : Deleting source file...\n");
	printf("=========================================\n");

	/* Delete original file */
	if (remove(argv[4]) != 0)
	{
	    printf("\n");
printf("=========================================\n");
printf("         FILE DELETION FAILED            \n");
printf("=========================================\n");
printf("ERROR : Unable to remove original file\n");
printf("Reason: File may be in use or protected\n");
printf("=========================================\n");
	    return m_failure;
	}

	printf("\n");
	printf("=========================================\n");
	printf("         FINALIZING UPDATE               \n");
	printf("=========================================\n");
	printf("STATUS : Renaming temporary file...\n");
	printf("=========================================\n");

	/* Rename temp file to original file name */
	if (rename("temp.mp3", argv[4]) != 0)
	{
	    printf("ERROR: Unable to rename temp file.\n");
	    return m_failure;
	}

	printf("\n");
	printf("=========================================\n");
	printf("        MP3 TAG UPDATE SUCCESSFUL        \n");
	printf("=========================================\n");
	printf("STATUS : File updated successfully\n");
	printf("All changes have been saved.\n");
	printf("=========================================\n");

	return m_success;   // Tag update completed successfully
}

Status copy_data (TagData* mp3tagData, char flag){
    if(flag==1){   // Target frame found and needs modification
        uint old_size,new_size;
        fread(&old_size,4,1,mp3tagData->fp_mp3);   // Read existing frame size

        char* ptr = (char*) &old_size;
        for(int i=0;i<2;i++){     // Convert size from big-endian format
            char temp=*(ptr+i);
            *(ptr+i)=*(ptr+4-i-1);
            *(ptr+4-i-1)=temp;
        }

        new_size = mp3tagData->frame_Id_size;   // Store updated frame size

        ptr = (char*) &mp3tagData->frame_Id_size;
        for(int i=0;i<2;i++){     // Convert new size to big-endian format
            char temp=*(ptr+i);
            *(ptr+i)=*(ptr+4-i-1);
            *(ptr+4-i-1)=temp;
        }

        fwrite(&mp3tagData->frame_Id_size,4,1,mp3tagData->fp_temp);   // Write updated size

        char f[3];
        fread(f,1,3,mp3tagData->fp_mp3);   // Read frame flags
        fwrite(f,1,3,mp3tagData->fp_temp); // Copy frame flags

        fwrite(mp3tagData->frame_Id_value,1,(new_size-1),mp3tagData->fp_temp);   // Write updated frame value

        fseek(mp3tagData->fp_mp3,(long)(old_size-1),SEEK_CUR);   // Skip old frame content
    }
    else{
        uint size;
        fread(&size,4,1,mp3tagData->fp_mp3);   // Read frame size
        fwrite(&size,4,1,mp3tagData->fp_temp); // Copy frame size

        char* ptr = (char*) &size;
        for(int i=0;i<2;i++){    // Convert size to host byte order
            char temp=*(ptr+i);
            *(ptr+i)=*(ptr+4-i-1);
            *(ptr+4-i-1)=temp;
        }

        char f[3];
        fread(f,1,3,mp3tagData->fp_mp3);   // Read frame flags
        fwrite(f,1,3,mp3tagData->fp_temp); // Copy frame flags

        char str[size-1];
        fread(str,1,(size-1),mp3tagData->fp_mp3);   // Read frame data
        str[size-1] ='\0';   // Null terminate frame content

        fwrite(str,1,(size-1),mp3tagData->fp_temp);   // Copy frame data
    }

    return m_success;   // Data copied successfully
}

Status copy_remaining_data (TagData* mp3tagData){
    char ch;

    while(fread(&ch,1,1,mp3tagData->fp_mp3)){   // Read remaining bytes from source file
        fwrite(&ch,1,1,mp3tagData->fp_temp);    // Write remaining bytes to temp file
    }

     return m_success;   // Remaining data copied successfully
}

Status copy_back_to_src (TagData* mp3tagData){
	printf("Copy Data Back to Source:\n");   // Display copy operation status

	fseek(mp3tagData->fp_mp3,0,SEEK_SET);    // Move source file pointer to beginning
	fseek(mp3tagData->fp_temp,0,SEEK_SET);   // Move temp file pointer to beginning

	char ch;

    while (fread(&ch,1,1,mp3tagData->fp_temp)){   // Read data from temp file
        fwrite(&ch,1,1,mp3tagData->fp_mp3);       // Write data back to source file
    }

    return m_success;   // Source file updated successfully
}
#ifndef TYPES_H
#define TYPES_H

/* Unsigned integer alias */
typedef unsigned int uint;

/* Function return status */
typedef enum
{
    m_success,     // Operation completed successfully
    m_failure      // Operation failed
} Status;

/* Supported operations */
typedef enum
{
    m_view,         // View MP3 tag information
    m_edit,         // Edit MP3 tag information
    m_unsupported   // Invalid or unsupported operation
} OperationType;

#endif
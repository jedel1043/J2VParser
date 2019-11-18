/*!
 * @file common.h
 * @brief Definition of two global variables to save the position of InputStreamBuffer
 * @details Definition of the global variables #current_line_number and #input_position required to save the position of the
 * buffer in the file input.
 */

#ifndef IO_COMMON_H
#define IO_COMMON_H

extern int current_line_number; //!< Vertical position of InputStreamBuffer at the file input (line number).
extern int input_position; //!< Horizontal position of InputStreamBuffer at the file input (column number).

#endif

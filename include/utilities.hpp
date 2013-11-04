/**
 * @file utilities.hpp
 * @brief Helper functions definition.
 */

#ifndef DEF_UTILITIES_H
#define DEF_UTILITIES_H

#include <string>

/** 
 * @brief Appends the given @p integer to the given @p string. 
 */
void string_appendInt(std::string &str, int x);

/**
 * @brief Returns the extension of the given @p file. 
 * @returns The file extension of the given @p file. If the file does not have an extension, 
 *          the function returns an empty string. 
 */
std::string getFileExtension(std::string &filename); 

#endif  /* DEF_UTILITIES_H */


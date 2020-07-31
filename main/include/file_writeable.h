#pragma once

#include <iostream>
#include <string>


/*
 * the FileWriteable interface is a template for objects which are able to be
 * saved in binary format and later retrieved from the binary.
 *
 * The object must be reconstructible without knowing the length of the binary
 * encoding that is given, i.e. if the length must be known, then it must be
 * written as part of the produced binary
 */
class FileWriteable {
    friend std::ostream & operator<<(std::ostream & f, const FileWriteable & obj);
    friend std::istream & operator>>(std::istream & f, FileWriteable & obj);
private:

    /*
     * returns a binary representation of this object that can be inserted in a
     * file for storage and later retrieved for restoration to its original
     * form
     */
    virtual const std::string to_file_str() const = 0;

    /*
     * given a pointer to memory containing this file's saved contents,
     * reconstructs the original object, modifying the data string to point to
     * just after the end of this object's encoding
     */
    virtual void reconstruct(std::istream & data) = 0;

};


/*
 * overload of insertion operator of FileWriteable object to an output stream
 */
std::ostream & operator<<(std::ostream & f, const FileWriteable & obj);

/*
 * overload of extraction operator of FileWriteable objects from an input stream
 */
std::istream & operator>>(std::istream & f, FileWriteable & obj);



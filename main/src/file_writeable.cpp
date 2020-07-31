
#include <file_writeable.h>


std::ostream & operator<<(std::ostream & f, const FileWriteable & obj) {
    f << obj.to_file_str();
    return f;
}


std::istream & operator>>(std::istream & f, FileWriteable & obj) {
    obj.reconstruct(f);
    return f;
}


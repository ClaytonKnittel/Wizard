#pragma once

#include <fstream>
#include <sstream>
#include <string>


class bifstream : public std::ifstream {
public:

    template<typename T>
    void read(T & t) {
        this->std::ifstream::read(reinterpret_cast<char *>(&t), sizeof(T));
    }

    template<typename T>
    bifstream & operator>>(T & t) {
        this->read(t);
        return *this;
    }

    bifstream & operator>>(std::string & s) {
        std::getline(*this, s, '\0');
        return *this;
    }

};


class bofstream : public std::ofstream {
public:

    template<typename T>
    void write(const T & t) {
        this->std::ofstream::write(reinterpret_cast<const char *>(&t),
                sizeof(T));
    }

    template<typename T>
    bofstream & operator<<(T & t) {
        this->write(t);
        return *this;
    }

    bofstream & operator<<(std::string & s) {
        *static_cast<std::ofstream *>(this) << s << '\0';
        return *this;
    }

};

class bfstream : public std::fstream {
public:

    template<typename T>
    void read(T & t) {
        this->std::fstream::read(reinterpret_cast<char *>(&t), sizeof(T));
    }

    template<typename T>
    void write(const T & t) {
        this->std::fstream::write(reinterpret_cast<const char *>(&t),
                sizeof(T));
    }

    template<typename T>
    bfstream & operator>>(T & t) {
        this->read(t);
        return *this;
    }

    bfstream & operator>>(std::string & s) {
        std::getline(*this, s, '\0');
        return *this;
    }

    template<typename T>
    bfstream & operator<<(T & t) {
        this->write(t);
        return *this;
    }

    bfstream & operator<<(std::string & s) {
        *static_cast<std::fstream *>(this) << s << '\0';
        return *this;
    }

};


class bistringstream : public std::istringstream {
public:

    template<typename T>
    void read(T & t) {
        this->std::istringstream::read(reinterpret_cast<char *>(&t), sizeof(T));
    }

    template<typename T>
    bistringstream & operator>>(T & t) {
        this->read(t);
        return *this;
    }

    bistringstream & operator>>(std::string & s) {
        std::getline(*this, s, '\0');
        return *this;
    }

};


class bostringstream : public std::ostringstream {
public:

    template<typename T>
    void write(const T & t) {
        this->std::ostringstream::write(reinterpret_cast<const char *>(&t),
                sizeof(T));
    }

    template<typename T>
    bostringstream & operator<<(T & t) {
        this->write(t);
        return *this;
    }

    bostringstream & operator<<(std::string & s) {
        *static_cast<std::ostringstream *>(this) << s << '\0';
        return *this;
    }

};


class bstringstream : public std::stringstream {
public:

    template<typename T>
    void read(T & t) {
        this->std::stringstream::read(reinterpret_cast<char *>(&t), sizeof(T));
    }

    template<typename T>
    void write(const T & t) {
        this->std::stringstream::write(reinterpret_cast<const char *>(&t),
                sizeof(T));
    }

    template<typename T>
    bstringstream & operator>>(T & t) {
        this->read(t);
        return *this;
    }

    bstringstream & operator>>(std::string & s) {
        std::getline(*this, s, '\0');
        return *this;
    }

    template<typename T>
    bstringstream & operator<<(T & t) {
        this->write(t);
        return *this;
    }

    bstringstream & operator<<(std::string & s) {
        *static_cast<std::stringstream *>(this) << s << '\0';
        return *this;
    }

};



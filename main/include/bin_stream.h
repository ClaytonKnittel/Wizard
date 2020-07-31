#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>




template<typename I>
class bstream {
private:

    I * base;
    bool do_delete;

public:

    bstream() : do_delete(true) {
        base = new I();
    }

    bstream(I & base) : base(&base), do_delete(false) {}

    ~bstream() {
        if (do_delete && base != nullptr) {
            delete base;
            base = nullptr;
        }
    }

    void open(const std::string & loc, int flags) {
        base->open(loc, flags);
    }

    bool is_open() {
        return base->is_open();
    }

    void close() {
        base->close();
    }

    std::string str() {
        return base->str();
    }

    template<typename T>
    void read(T & t) {
        base->read(reinterpret_cast<char *>(&t), sizeof(T));
    }

    template<typename T>
    void write(const T & t) {
        base->write(reinterpret_cast<const char *>(&t),
                sizeof(T));
    }

    template<>
    void write(const std::string & s) {
        base->write(s.c_str(), s.size());
    }

    void write(const void * mem, size_t n_bytes) {
        base->write(mem, n_bytes);
    }

    template<typename T>
    bstream & operator>>(T & t) {
        this->read(t);
        return *this;
    }

    template<>
    bstream & operator>>(std::string & s) {
        uint32_t len;
        *this >> len;
        s.reserve(len);
        std::copy_n(std::istream_iterator<char>(base),
                len, std::back_inserter(s));
        return *this;
    }

    template<typename T>
    bstream & operator<<(const T & t) {
        this->write(t);
        return *this;
    }

    template<>
    bstream & operator<<(const std::string & s) {
        uint32_t len = s.size();
        *this << len;
        this->write(s);
        return *this;
    }

};

template<typename I>
class bistream : public bstream<I> {
public:
};


template<typename I>
class bostream : public bstream<I> {
public:
};


class bifstream : public bistream<std::ifstream> {
};


class bofstream : public bostream<std::ofstream> {
};


class bfstream : public bstream<std::fstream> {
};


class bistringstream : public bistream<std::istringstream> {
};


class bostringstream : public bostream<std::ostringstream> {
};


class bstringstream : public bstream<std::stringstream> {
};



#pragma once

#include <unordered_map>
#include <regex>
#include <string>

#include <texture_set.h>


class TextureCollection {
private:

    std::regex name_regex;

    /*
     * map of texture names to TextureSet objects, 
     */
    std::unordered_map<std::string, TextureSet> m;

public:

    TextureCollection();
    ~TextureCollection();

    void clear();

    int add_texture(const std::string & loc, int tile_w, int tile_h);

    const TextureSet * get_tex_set(const std::string & name) const;

    /*
     * alias of get_tex_set
     */
    const TextureSet * operator[](const std::string & name) const;

};




#include <texture_collection.h>


TextureCollection::TextureCollection() :
        name_regex("(?:[^\\/]+\\/)*([^\\.]+)\\..*") {}

TextureCollection::~TextureCollection() {}

void TextureCollection::clear() {
    m.clear();
}


int TextureCollection::add_texture(const std::string & loc, int tile_w,
            int tile_h) {
    std::string name;
    std::cmatch cm;

    std::regex_match(loc.c_str(), cm, name_regex);

    if (cm.size() < 2) {
        fprintf(stderr, "Texture file %s is not a proper file format\n", loc.c_str());
        return -1;
    }

    name = cm[1];

    m.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name)),
            std::forward_as_tuple(loc, tile_w, tile_h));

    return 0;
}

const TextureSet * TextureCollection::get_tex_set(const std::string & name) const {
    auto it = m.find(name);
    if (it == m.end()) {
        return nullptr;
    }
    return &it->second;
}

const TextureSet * TextureCollection::operator[](const std::string & name) const {
    return get_tex_set(name);
}


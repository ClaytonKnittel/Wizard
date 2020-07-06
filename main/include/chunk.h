#pragma once


#include <tile.h>

const unsigned int chunk_size = 8;

class Chunk : public Object {
private:

    Tile tiles[chunk_size * chunk_size];

public:

    Chunk();
    virtual ~Chunk();

    virtual void insert_all(Renderer &) const;
};



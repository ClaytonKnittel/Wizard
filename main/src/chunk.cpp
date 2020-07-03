
#include <chunk.h>


Chunk::Chunk() {

}

Chunk::~Chunk() {

}


void Chunk::insert_all(Renderer & r) {

    for (int idx = 0; idx < chunk_size * chunk_size; idx++) {
        tiles[idx].insert_all(r);
    }

}



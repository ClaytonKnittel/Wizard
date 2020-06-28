

#include <sstream>
#include <unordered_map>
#include <random>

#include <math/random.h>

#include <bin_stream.h>
#include <map.h>
#include <util.h>


#define N_TEXS 1

Board::Board() : Entity(0, 0, .08f), rbuf(8000) {

#define W 5
#define H 5

    texs = new texture[N_TEXS];
    tex_files = new std::string[N_TEXS];
    texture_init(&texs[0], "main/img/test.bmp");
    tex_files[0] = "test.bmp";
    //texture_init(&texs[0], "main/img/gsquare2.bmp");
    //texture_init(&texs[1], "main/img/square2.bmp");

    for (int i = 0; i < W * H; i++) {
        tiles.push_back(Tile(
                &texs[(i / W + i % W) % N_TEXS],
                i % W,
                i / W,
                -(i / W + i % W) / 2
                ));
    }

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    gl_use_program(&prog);

    // bind each texture in fragment shader to the proper texture unit
    for (int i = 0; i < 16; i++) {
        int loc = gl_uniform_location(&prog, "texs") + i;
        glUniform1i(loc, i);
    }
}


Board::~Board() {
    for (int i = 0; i < N_TEXS; i++) {
        texture_destroy(&texs[i]);
    }
    delete [] texs;
    delete [] tex_files;
    gl_unload_program(&prog);
}



int Board::save(const char * loc) {
    bofstream f;

    f.open(loc, std::ios::binary | std::fstream::trunc);

    if (!f.is_open()) {
        fprintf(stderr, "Unable to open file %s for writing\n", loc);
        return -1;
    }

    bstringstream texs;
    bstringstream dat;
    std::unordered_map<texture *, int> tex_idxs;

    for (const Tile & t : tiles) {
        int tex_idx;
        auto it = tex_idxs.find(t.tex);

        if (it == tex_idxs.end()) {
            tex_idx = tex_idxs.size();
            tex_idxs[t.tex] = tex_idx;

            size_t arr_idx = ((uint64_t) t.tex) - ((uint64_t) this->texs);
            texs << tex_idx << tex_files[arr_idx];
        }
        else {
            tex_idx = it->second;
        }

        dat << t.x << t.y << t.z << tex_idx;
    }

    uint32_t n_tiles = tiles.size();
    uint32_t n_texs = tex_idxs.size();
    f << n_tiles << n_texs;
    f << texs.str();
    f << dat.str();

    return 0;
}


int Board::load(const char * loc) {
    bifstream f;

    f.open(loc, std::ios::binary);

    uint32_t n_tiles;
    uint32_t n_texs;

    f >> n_tiles >> n_texs;

    printf("%u, %u\n", n_tiles, n_texs);

    return 0;
}


void Board::render(const Screen & screen) {
    gl_use_program(&prog);

    // upload camera and view transformation matrices
    screen.apply(&prog);

    // call superclass render callback
    upload_pos(&prog);

    // upload all tiles to renderer
    for (Tile t : tiles) {
        t.insert_all(rbuf);
    }

    // flush renderer to screen
    rbuf.flush();

}


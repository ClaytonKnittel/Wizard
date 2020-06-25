

#include <map.h>
#include <random>

#include <math/random.h>

#include <util.h>

#define NUM_VERTICES 6
    
/*static const float square_data[NUM_VERTICES * 4] = {
    0.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 1.f, 0.f,
    1.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
};*/


extern const float aspect_ratio;
extern const float inv_aspect_ratio;


#define N_TEXS 1

Board::Board() : Entity(-.8f, -.3f, 2.5f), rbuf(600) {

#define W 10
#define H 10

    for (int i = 0; i < W * H; i++) {
        tiles.push_back({
                .id = (i / W) /*rand()*/ % N_TEXS,
                .x  = i % W,
                .y  = i / W,
                .z  = 0
                });
    }

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    for (int i = 0; i < 16; i++) {
        int loc = gl_uniform_location(&prog, "texs") + i;
        glUniform1i(loc, i);
    }

    texs = new texture[N_TEXS];
    texture_init(&texs[0], "main/img/square.bmp");
}


Board::~Board() {
    for (int i = 0; i < N_TEXS; i++) {
        texture_destroy(&texs[i]);
    }
    delete [] texs;
    gl_unload_program(&prog);
}


#define PIX_WID 17

#define TILE_TOP_AR .5f

void Board::render() {
    gl_use_program(&prog);

    // call superclass render callback
    upload_pos(&prog);

    float tile_size = .1f;
    float ytile_size = tile_size * inv_aspect_ratio * TILE_TOP_AR;

    for (int i = 0; i < 16; i++) {
        int loc = gl_uniform_location(&prog, "texs") + i;
        glUniform1i(loc, i);
    }


    for (Tile t : tiles) {
        int id  = t.id;
        float x = t.x * tile_size;
        float _y = t.y;
        float y = _y * ytile_size;
        float yx = _y * tile_size;

        Vertex t1 = {
            .x = x + yx / 2.f + tile_size / 2.f,
            .y = y / 2.f,
            .z = y,
            .tx = 0.f,
            .ty = 0.f
        },
             t2 = {
            .x = x + yx / 2.f + tile_size,
            .y = y / 2.f + ytile_size / 2.f,
            .z = y,
            .tx = 1.f,
            .ty = 0.f
        },
             t3 = {
            .x = x + yx / 2.f + tile_size / 2.f,
            .y = y / 2.f + ytile_size,
            .z = y,
            .tx = 1.f,
            .ty = 1.f
        },
             t4 = {
            .x = x + yx / 2.f + tile_size / 2.f,
            .y = y / 2.f,
            .z = y,
            .tx = 0.f,
            .ty = 0.f
        },
             t5 = {
            .x = x + yx / 2.f,
            .y = y / 2.f + ytile_size / 2.f,
            .z = y,
            .tx = 0.f,
            .ty = 1.f
        },
             t6 = {
            .x = x + yx / 2.f + tile_size / 2.f,
            .y = y / 2.f + ytile_size,
            .z = y,
            .tx = 1.f,
            .ty = 1.f
        };

        Triangle ta = {
            .vertices = {
                t1,
                t2,
                t3
            },
            .tex = &texs[id]
        };
        Triangle tb = {
            .vertices = {
                t4,
                t5,
                t6
            },
            .tex = &texs[id]
        };

        rbuf << ta << tb;
    }

    rbuf.flush();

}


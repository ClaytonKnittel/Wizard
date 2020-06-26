

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


#define N_TEXS 2

Board::Board() : Entity(-.8f, -.3f, .08f), rbuf(600) {

#define W 10
#define H 10

    for (int i = 0; i < W * H; i++) {
        tiles.push_back({
                .id = (i / W + i % W) % N_TEXS,
                //.id = rand() % N_TEXS,
                .x  = i % W,
                .y  = i / W,
                .z  = (i / W)
                });
    }

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    gl_use_program(&prog);

    // bind each texture in fragment shader to the proper texture unit
    for (int i = 0; i < 16; i++) {
        int loc = gl_uniform_location(&prog, "texs") + i;
        glUniform1i(loc, i);
    }

    texs = new texture[N_TEXS];
    texture_init(&texs[0], "main/img/square.bmp");
    //texture_init(&texs[0], "main/img/gsquare2.bmp");
    texture_init(&texs[1], "main/img/square2.bmp");
}


Board::~Board() {
    for (int i = 0; i < N_TEXS; i++) {
        texture_destroy(&texs[i]);
    }
    delete [] texs;
    gl_unload_program(&prog);
}


void Board::render(const Screen & screen) {
    gl_use_program(&prog);

    // upload camera and view transformation matrices
    screen.apply(&prog);

    // call superclass render callback
    upload_pos(&prog);

    float tile_size = 1.f;
    float ytile_size = tile_size;


    for (Tile t : tiles) {
        int id  = t.id;
        float x = t.x * tile_size;
        float y = t.y * ytile_size;
        float z = t.z;

        Triangle t1 = {
            .vertices = {
                {
                    .x = x,
                    .y = y,
                    .z = z,
                    .tx = 0.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z,
                    .tx = 1.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y + ytile_size,
                    .z = z,
                    .tx = 1.f,
                    .ty = 1.f
                }
            },
            .tex = &texs[id]
        };
        Triangle t2 = {
            .vertices = {
                {
                    .x = x,
                    .y = y,
                    .z = z,
                    .tx = 0.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y + ytile_size,
                    .z = z,
                    .tx = 1.f,
                    .ty = 1.f
                },
                {
                    .x = x,
                    .y = y + ytile_size,
                    .z = z,
                    .tx = 0.f,
                    .ty = 1.f
                }
            },
            .tex = &texs[id]
        };
        Triangle t3 = {
            .vertices = {
                {
                    .x = x,
                    .y = y,
                    .z = z - 1.f,
                    .tx = 0.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z - 1.f,
                    .tx = 1.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z,
                    .tx = 1.f,
                    .ty = 1.f
                }
            },
            .tex = &texs[id]
        };
        Triangle t4 = {
            .vertices = {
                {
                    .x = x,
                    .y = y,
                    .z = z - 1.f,
                    .tx = 0.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z,
                    .tx = 1.f,
                    .ty = 1.f
                },
                {
                    .x = x,
                    .y = y,
                    .z = z,
                    .tx = 0.f,
                    .ty = 1.f
                }
            },
            .tex = &texs[id]
        };
        Triangle t5 = {
            .vertices = {
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z - 1.f,
                    .tx = 0.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y + tile_size,
                    .z = z - 1.f,
                    .tx = 1.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y + tile_size,
                    .z = z,
                    .tx = 1.f,
                    .ty = 1.f
                }
            },
            .tex = &texs[id]
        };
        Triangle t6 = {
            .vertices = {
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z - 1.f,
                    .tx = 0.f,
                    .ty = 0.f
                },
                {
                    .x = x + tile_size,
                    .y = y + tile_size,
                    .z = z,
                    .tx = 1.f,
                    .ty = 1.f
                },
                {
                    .x = x + tile_size,
                    .y = y,
                    .z = z,
                    .tx = 0.f,
                    .ty = 1.f
                }
            },
            .tex = &texs[id]
        };

        rbuf << t1 << t2 << t3 << t4 << t5 << t6;
    }

    rbuf.flush();

}


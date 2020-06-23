

#include <map.h>

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




Board::Board() : Entity(0.f, 0.f), rbuf(&prog, 200) {

    for (int i = 0; i < 100; i++) {
        if ((i + i / 10) % 2 == 0) {
            tiles.push_back({
                    .id = 0,
                    .x  = i % 10,
                    .y  = i / 10,
                    .z  = 0
                    });
        }
        else {
            tiles.push_back({
                    .id = 0,
                    .x  = -10 + i % 10,
                    .y  = 1 + i / 10,
                    .z  = 0
                    });
        }
    }

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    texs = new texture[2];
    texture_init(&texs[0], "main/img/cube_top.bmp");
    texture_init(&texs[1], "main/img/cube_left.bmp");
}


Board::~Board() {
    for (int i = 0; i < 2; i++) {
        texture_destroy(&texs[i]);
    }
    delete [] texs;
    gl_unload_program(&prog);
}


#define PIX_WID 17

#define SCALE_DOWN ((PIX_WID - 1.f) / PIX_WID)

void Board::render() {
    gl_use_program(&prog);

    // call superclass render callback
    upload_pos(&prog);

    float tile_size = .1f;


    for (Tile t : tiles) {
        int id  = t.id;
        float x = t.x * SCALE_DOWN * tile_size;
        float y = t.y * SCALE_DOWN * tile_size;

        Vertex t1 = {
            .x = x + y / 2.f,
            .y = y / 2.f,
            .z = 0.f,
            .texture_idx = id,
            .tx = 0.f,
            .ty = 0.f
        },
             t2 = {
            .x = x + y / 2.f + tile_size,
            .y = y / 2.f,
            .z = 0.f,
            .texture_idx = id,
            .tx = 1.f,
            .ty = 0.f
        },
             t3 = {
            .x = x + y / 2.f + tile_size,
            .y = y / 2.f + tile_size,
            .z = 0.f,
            .texture_idx = id,
            .tx = 1.f,
            .ty = 1.f
        },
             t4 = {
            .x = x + y / 2.f,
            .y = y / 2.f,
            .z = 0.f,
            .texture_idx = id,
            .tx = 0.f,
            .ty = 0.f
        },
             t5 = {
            .x = x + y / 2.f,
            .y = y / 2.f + tile_size,
            .z = 0.f,
            .texture_idx = id,
            .tx = 0.f,
            .ty = 1.f
        },
             t6 = {
            .x = x + y / 2.f + tile_size,
            .y = y / 2.f + tile_size,
            .z = 0.f,
            .texture_idx = id,
            .tx = 1.f,
            .ty = 1.f
        };

        rbuf << t1 << t2 << t3;
        rbuf << t4 << t5 << t6;

        //gl_draw(&prot);
    }

    texture_use(&texs[0]);
    glCheckError();
    rbuf.flush();

}


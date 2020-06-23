

#include <map.h>


#define NUM_VERTICES 6
    
static const float square_data[NUM_VERTICES * 4] = {
    0.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 1.f, 0.f,
    1.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
};


Board::Board() {

    for (int i = 0; i < 100; i++) {
        tiles.push_back({
                .id = 0,
                .x  = i % 10,
                .y  = i / 10
            });
    }

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    gl_load_dynamic_textured(&prot, square_data, NUM_VERTICES);
    
    texture_init(&tex, "main/img/cube_top.bmp");
}


Board::~Board() {
    texture_destroy(&tex);
    gl_unload_static_monochrome_drawable(&prot);
    gl_unload_program(&prog);
}


#define PIX_WID 17

#define SCALE_DOWN ((PIX_WID - 1.f) / PIX_WID)


void Board::render() {
    gl_use_program(&prog);
    GLuint trans = gl_uniform_location(&prog, "trans");


    for (Tile t : tiles) {
        float x = t.x / 10.f * SCALE_DOWN;
        float y = t.y / 10.f * SCALE_DOWN;

        float mat[] = {
            1.f / 10, 0.f,          0.f,
            0.f,         1.f / 10, 0.f,
            x + y / 2.f, y / 2.f,      1.f
        };

        glUniformMatrix3fv(trans, 1, 0, mat);

        texture_use(&tex);
        gl_draw(&prot);
    }

}


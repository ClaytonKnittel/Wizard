#pragma once

#include <gl/drawable.h>
#include <gl/gl.h>
#include <gl/shader.h>

#include <board.h>
#include <entity.h>
#include <screen.h>


enum STState {
    disabled,
    enabled,
    drawing_area,
    fixed_area,
    dragging
};


class SelectTool : public Entity {
private:

    constexpr static int fs_per_vertex = 3;
    constexpr static int n_vertices = 24;

    // portion of a tile to be covered by select perimeter (1 / 16)
    constexpr static float strip_portion = .125f;

    float _buf[fs_per_vertex * n_vertices];

    drawable d;
    program prog;

    /*
     * reference to the board on which this select tool is active
     */
    const Board & b;

    STState state;
    bool modified;

    int anchor_x, anchor_y;
    int prev_mx, prev_my;
    float blx, bly, w, h;

    /*
     * updates contents of _buf to reflect change in position
     */
    void update_buf();

    void begin_area(float mx, float my);

    void update_area(float mx, float my);

public:

    SelectTool(const Board & board);
    ~SelectTool();

    void enable();
    void disable();

    void click(float mx, float my);
    void release();

    /*
     * world-coordinates of mouse, can be called every frame to continually
     * update the select tool's position
     */
    void mouse_at(float mx, float my);

    virtual void render(const Screen & screen);

};



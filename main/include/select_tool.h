#pragma once

#include <vector>

#include <gl/drawable.h>
#include <gl/gl.h>
#include <gl/shader.h>

#include <board.h>
#include <entity.h>
#include <screen.h>
#include <tile.h>


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
    Board & b;
    std::vector<Tile> grabbed_tiles;

    STState state;
    bool modified;

    // when in drawing mode, is the anchor position (starting mx, my when mouse
    // was first clicked)
    // when in dragging mode, is the original blx, bly before the selection
    // moved
    int anchor_x, anchor_y;

    union {
        // drawing mode
        struct {
            // prior mouse position in grid coordinates, only need to update when
            // these have changed
            int prev_mx, prev_my;
        };
        // dragging mode
        struct {
            // prior offsets from (anchor_x, anchor_y) the region was before
            // this frame
            int prev_dx, prev_dy;
            // the place the region was originally clicked (world coordinates)
            float anchor_mx, anchor_my;
        };
    };

    // bottom left of region is (blx, bly), width and height are w and h
    // respectively, used for drawing
    float blx, bly, w, h;

    /*
     * updates contents of _buf to reflect change in position
     */
    void update_buf();


    // true if given mouse position is within the borders of the current
    // selected region
    bool is_inside(float mx, float my);


    void begin_area(float mx, float my);

    void update_area(float mx, float my);

    /*
     * remove the tiles within the tool from the board and place them in the
     * grabbed_tiles vector, so they may be moved around
     */
    void take_board_tiles();

    /*
     * move all tiles in grabbed_tiles by (dx, dy)
     */
    void move_tiles(int dx, int dy);

    /*
     * inverse of take_board_tiles, places each of the tiles back on the board
     */
    void place_grabbed_tiles();

    void begin_dragging(float mx, float my);

    /*
     * to be called each frame while the area is being dragged around
     */
    void drag_to(float mx, float my);

public:

    SelectTool(Board & board);
    ~SelectTool();

    bool is_enabled() const;
    bool is_visible() const;

    void enable();
    void disable();

    void click(float mx, float my);
    void release();

    /*
     * deletes all tiles in current selection
     */
    void delete_tiles();

    /*
     * world-coordinates of mouse, can be called every frame to continually
     * update the select tool's position
     */
    void mouse_at(float mx, float my);

    virtual void render(const Screen & screen);

};



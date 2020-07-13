
#include <util.h>

#include <select_tool.h>


void SelectTool::update_buf() {
    float x = this->blx;
    float y = this->bly;

    _buf[0] = x;
    _buf[1] = y;
    _buf[2] = 1.f;

    _buf[3] = x + w;
    _buf[4] = y;
    _buf[5] = 1.f;

    _buf[6] = x + w - strip_portion;
    _buf[7] = y + strip_portion;
    _buf[8] = 0.f;


    _buf[9] = x;
    _buf[10] = y;
    _buf[11] = 1.f;

    _buf[12] = x + w - strip_portion;
    _buf[13] = y + strip_portion;
    _buf[14] = 0.f;

    _buf[15] = x + strip_portion;
    _buf[16] = y + strip_portion;
    _buf[17] = 0.f;



    _buf[18] = x + w;
    _buf[19] = y;
    _buf[20] = 1.f;

    _buf[21] = x + w;
    _buf[22] = y + h;
    _buf[23] = 1.f;

    _buf[24] = x + w - strip_portion;
    _buf[25] = y + h - strip_portion;
    _buf[26] = 0.f;


    _buf[27] = x + w;
    _buf[28] = y;
    _buf[29] = 1.f;

    _buf[30] = x + w - strip_portion;
    _buf[31] = y + h - strip_portion;
    _buf[32] = 0.f;

    _buf[33] = x + w - strip_portion;
    _buf[34] = y + strip_portion;
    _buf[35] = 0.f;



    _buf[36] = x + w;
    _buf[37] = y + h;
    _buf[38] = 1.f;

    _buf[39] = x;
    _buf[40] = y + h;
    _buf[41] = 1.f;

    _buf[42] = x + strip_portion;
    _buf[43] = y + h - strip_portion;
    _buf[44] = 0.f;


    _buf[45] = x + w;
    _buf[46] = y + h;
    _buf[47] = 1.f;

    _buf[48] = x + strip_portion;
    _buf[49] = y + h - strip_portion;
    _buf[50] = 0.f;

    _buf[51] = x + w - strip_portion;
    _buf[52] = y + h - strip_portion;
    _buf[53] = 0.f;



    _buf[54] = x;
    _buf[55] = y + h;
    _buf[56] = 1.f;

    _buf[57] = x;
    _buf[58] = y;
    _buf[59] = 1.f;

    _buf[60] = x + strip_portion;
    _buf[61] = y + strip_portion;
    _buf[62] = 0.f;


    _buf[63] = x;
    _buf[64] = y + h;
    _buf[65] = 1.f;

    _buf[66] = x + strip_portion;
    _buf[67] = y + strip_portion;
    _buf[68] = 0.f;

    _buf[69] = x + strip_portion;
    _buf[70] = y + h - strip_portion;
    _buf[71] = 0.f;

    this->modified = false;
}



bool SelectTool::is_inside(float mx, float my) {
    int x, y;
    b.get_coords(mx, my, x, y);

    return (x >= this->blx && x < this->blx + this->w) &&
           (y >= this->bly && y < this->bly + this->h);
}


void SelectTool::begin_area(float mx, float my) {
    b.get_coords(mx, my, this->anchor_x, this->anchor_y);
    this->blx = this->anchor_x;
    this->bly = this->anchor_y;
    this->w = 1.f;
    this->h = 1.f;
    this->prev_mx = this->anchor_x;
    this->prev_my = this->anchor_y;
    this->state = drawing_area;

    this->modified = true;
}

void SelectTool::update_area(float mx, float my) {
    int x, y;

    b.get_coords(mx, my, x, y);

    if (x == prev_mx && y == prev_my) {
        return;
    }

    int minx = MIN(anchor_x, x);
    int maxx = MAX(anchor_x, x) + 1;
    int miny = MIN(anchor_y, y);
    int maxy = MAX(anchor_y, y) + 1;

    this->w = (maxx - minx);
    this->h = (maxy - miny);
    this->blx = minx;
    this->bly = miny;

    this->modified = true;
}


void SelectTool::take_board_tiles() {
    this->grabbed_tiles = b.tiles_in_range(blx, bly, blx + w - 1, bly + h - 1);

    // erase each of the tiles from the board
    for (const Tile & t : grabbed_tiles) {
        b.add_tile(t.get_x(), t.get_y(), nullptr, 0);
    }
}


void SelectTool::move_tiles(int dx, int dy) {
    for (Tile & t : grabbed_tiles) {
        t.set_x(t.get_x() + dx);
        t.set_y(t.get_y() + dy);
    }
}


void SelectTool::place_grabbed_tiles() {
    for (const Tile & t : this->grabbed_tiles) {
        b.add_tile(t);
    }

    grabbed_tiles.clear();
}


void SelectTool::begin_dragging(float mx, float my) {
    b.get_coords(mx, my, this->prev_mx, this->prev_my);
    this->state = dragging;

    this->modified = false;
}


void SelectTool::drag_to(float mx, float my) {
    int x, y;

    b.get_coords(mx, my, x, y);

    if (x == prev_mx && y == prev_my) {
        // no change
        return;
    }

    int dx = x - this->prev_mx;
    int dy = y - this->prev_my;

    this->blx += dx;
    this->bly += dy;

    this->move_tiles(dx, dy);

    this->prev_mx = x;
    this->prev_my = y;

    this->modified = true;
}


SelectTool::SelectTool(Board & board) : Entity(0, 0, 1.f), b(board),
            state(disabled), modified(true), clipboard_full(false),
            anchor_x(0), anchor_y(0),
            prev_mx(0), prev_my(y), w(0), h(0) {

    gl_load_dynamic_pos_float(&d, NULL, n_vertices);

    gl_load_program(&prog, "main/res/select_tool.vs",
                           "main/res/select_tool.fs");
}


SelectTool::~SelectTool() {
    gl_unload_program(&prog);
    gl_unload_static_monochrome_drawable(&d);
}

bool SelectTool::is_enabled() const {
    return this->state != disabled;
}


void SelectTool::enable() {
    this->state = enabled;
    this->modified = true;
}

bool SelectTool::is_visible() const {
    return this->state != disabled && this->state != enabled;
}


void SelectTool::disable() {
    this->state = disabled;
}


void SelectTool::click(float mx, float my) {
    switch (this->state) {
        case enabled:
            this->begin_area(mx, my);
            break;
        case fixed_area:
            // TODO inside vs outside
            if (this->is_inside(mx, my)) {
                this->begin_dragging(mx, my);
            }
            else {
                this->place_grabbed_tiles();
                this->state = enabled;
            }
            break;
        case disabled:
        case drawing_area:
        case dragging:
            break;
    }
}

void SelectTool::release() {
    switch (this->state) {
        case drawing_area:
            take_board_tiles();
            state = fixed_area;
            break;
        case dragging:
            state = fixed_area;
            break;
        case disabled:
        case enabled:
        case fixed_area:
            break;
    }
}


void SelectTool::delete_tiles() {
    if (this->is_enabled()) {
        grabbed_tiles.clear();
    }
}


void SelectTool::copy() {
    if (this->is_enabled() && this->grabbed_tiles.size() > 0) {
        // only copy if we are copying at least something
        this->clipboard = this->grabbed_tiles;
        this->clipboard_full = true;
    }
}


void SelectTool::paste() {
    if (!this->clipboard_full) {
        return;
    }
    if (!this->is_enabled()) {
        // cannot paste when not enabled
        return;
    }

    if (this->state != enabled) {
        // if in any state besides enabled, then there will potentially be
        // tiles in grabbed_tiles
        place_grabbed_tiles();
    }

    WIZARD_ASSERT(grabbed_tiles.size() == 0);
    WIZARD_ASSERT(clipboard.size() > 0);

    grabbed_tiles.reserve(clipboard.size());

    auto it = clipboard.begin();

    const Tile & first = *it;
    int minx = first.get_x(), maxx = first.get_x();
    int miny = first.get_y(), maxy = first.get_y();

    grabbed_tiles.push_back(first);
    it++;

    for (; it != clipboard.end(); it++) {
        grabbed_tiles.push_back(*it);

        int x = it->get_x();
        int y = it->get_y();

        minx = MIN(minx, x);
        miny = MIN(miny, y);
        maxx = MAX(maxx, x);
        maxy = MAX(maxy, y);
    }

    this->blx = minx;
    this->bly = miny;
    this->w = maxx - minx + 1;
    this->h = maxy - miny + 1;
    this->modified = true;
}


void SelectTool::mouse_at(float mx, float my) {
    switch (this->state) {
        case drawing_area:
            this->update_area(mx, my);
            break;
        case dragging:
            // TODO drag around
            this->drag_to(mx, my);
            break;
        case disabled:
        case enabled:
        case fixed_area:
            break;
    }
}


void SelectTool::render(const Screen & screen) {
    if (!this->is_visible()) {
        return;
    }

    if (this->grabbed_tiles.size() > 0) {
        b.draw_tiles(screen, grabbed_tiles);
    }

    this->scale = b.get_scale();

    if (modified) {
        update_buf();
        gl_update_dynamic_pos_float(&d, this->_buf, n_vertices);
    }

    gl_use_program(&prog);

    // upload camera and view transformation matrices
    screen.apply(&prog);

    // call superclass render callback
    upload_pos(&prog);

    gl_draw_sub(&d, n_vertices);
}


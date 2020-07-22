
#include <tile_tree.h>

#include <chunk.h>



TileTree::NodeBase::NodeBase(int x, int y, uint8_t level) :
    parent(nullptr), x(x), y(y), node_level(level) {

}

TileTree::NodeBase::~NodeBase() {

}


int TileTree::NodeBase::get_log_w() {
    return log_children_dim * (node_level + 1);
}

int TileTree::NodeBase::get_log_h() {
    return log_children_dim * (node_level + 1);
}

int TileTree::NodeBase::get_w() {
    return 1 << get_log_w();
}

int TileTree::NodeBase::get_h() {
    return 1 << get_log_h();
}


template<typename Child_t>
TileTree::Node<Child_t>::Node(int x, int y, uint8_t level) :
    NodeBase(x, y, level), children{}, occ_b(0) {}

template<typename Child_t>
TileTree::Node<Child_t>::~Node() {
    for (int i = 0; i < NodeBase::branch_factor; i++) {
        if (children[i] != nullptr) {
            delete children[i];
        }
    }
}


template<typename Child_t>
TileTree::NodeBase * TileTree::Node<Child_t>::make_parent(int x_idx, int y_idx) {
    NodeBase * parent = new Node<NodeBase *>(x, y, this->node_level + 1);
    parent->
}

template<>
TileTree::NodeBase * TileTree::Node<Tile>::make_parent(int x_idx, int y_idx) {
    NodeBase * parent = new Node<NodeBase *>(x, y, this->node_level + 1);
}



void TileTree::prepare_insert(int x, int y) {
    NodeBase * root = this->root;
    int logw, logh, dx, dy;

    while (1) {

        logw = root->get_log_w();
        logh = root->get_log_h();

        // divide by width and height, rounding towards minus infinity
        dx = (x - root->x) >> logw;
        dy = (y - root->y) >> logh;

        if (dx == 0 && dy == 0) {
            // within the boundaries of the current root
            break;
        }

        NodeBase * parent;
        // if the new tile is within half the width of a parent node of the
        // current node, create a region containing both the current node and
        // the new tile
        if ((dx < NodeBase::children_dim && dx >= -NodeBase::children_dim &&
                    dy < NodeBase::children_dim && dy >= -NodeBase::children_dim) &&
                (dx < 0 || dy < 0)) {
            
            // the new region is created either touching the x-axis with
            // center at same x-coordinate as this->x if dy >= 0,
            // touching the y-axis with center at same y-coordinate as this->y
            // if dx >= 0, or center at (this->x, this->y) if dx < 0 and dy < 0

            // 4 if dx < 0, otherwise 0
            int half_width  = (dx < 0) * (NodeBase::children_dim / 2);
            // 4 if dy < 0, otherwise 0
            int half_height = (dy < 0) * (NodeBase::children_dim / 2);

            parent = root->make_parent(half_width, half_height);
        }
        else {
            // otherwise, simply make a parent node with the current node as
            // the bottom-left child
            parent = root->make_parent(0, 0);
        }

        root = parent;

    }

    this->root = root;
}



TileTree::TileTree() : root(nullptr) {
}


TileTree::~TileTree() {
    if (root != nullptr) {
        delete root;
    }
}



void TileTree::insert_tile(const Tile & t) {
    
}



TileTree::iterator TileTree::find_all(int llx, int lly, int urx, int ury) {

    return TileTree::iterator();
    
}



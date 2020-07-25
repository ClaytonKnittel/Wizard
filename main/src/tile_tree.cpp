
#include <stdexcept>
#include <utility>

#include <util.h>

#include <chunk.h>
#include <tile_tree.h>



TileTree::NodeBase::NodeBase(int x, int y, uint8_t level) :
    parent(nullptr), x(x), y(y), node_level(level) {

}

TileTree::NodeBase::~NodeBase() {

}



int TileTree::NodeBase::get_log_w(uint8_t node_level) {
    return log_children_dim * (node_level + 1);
}

int TileTree::NodeBase::get_log_h(uint8_t node_level) {
    return log_children_dim * (node_level + 1);
}

int TileTree::NodeBase::get_w(uint8_t node_level) {
    return 1 << get_log_w(node_level);
}

int TileTree::NodeBase::get_h(uint8_t node_level) {
    return 1 << get_log_h(node_level);
}


int TileTree::NodeBase::get_log_w() const {
    return get_log_w(this->node_level);
}

int TileTree::NodeBase::get_log_h() const {
    return get_log_h(this->node_level);
}

int TileTree::NodeBase::get_w() const {
    return get_w(this->node_level);
}

int TileTree::NodeBase::get_h() const {
    return get_h(this->node_level);
}


uint32_t TileTree::NodeBase::get_idx(int x_idx, int y_idx) {
    return y_idx * children_dim + x_idx;
}


uint32_t TileTree::NodeBase::get_tile_idx(int tile_x, int tile_y) {
    int x_idx, y_idx;
    tile_to_grid_coords(tile_x, tile_y, x_idx, y_idx);
    return get_idx(x_idx, y_idx);
}


void TileTree::NodeBase::tile_to_grid_coords(int tile_x, int tile_y,
        int & x_idx, int & y_idx) {
    int dx = tile_x - this->x;
    int dy = tile_y - this->y;

    x_idx = dx >> (log_children_dim * this->node_level);
    y_idx = dy >> (log_children_dim * this->node_level);
}


bool TileTree::NodeBase::contains(int tile_x, int tile_y) const {
    uint32_t dx = static_cast<uint32_t>(tile_x - this->x);
    uint32_t dy = static_cast<uint32_t>(tile_y - this->y);

    return !(dx >> (log_children_dim * (this->node_level + 1))) &&
           !(dy >> (log_children_dim * (this->node_level + 1)));
}


TileTree::NodeBase * TileTree::NodeBase::find_child_containing(int tile_x, int tile_y) {
    int x_idx, y_idx;
    assert(this->contains(tile_x, tile_y));

    this->tile_to_grid_coords(tile_x, tile_y, x_idx, y_idx);
    return get_child(x_idx, y_idx);
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

template<>
TileTree::Node<Tile>::~Node() {
}

template<>
TileTree::NodeBase * & TileTree::Node<TileTree::NodeBase *>::try_make_child(
        int x_idx, int y_idx) {
    NodeBase * & child = this->children[get_idx(x_idx, y_idx)];

    if (child != nullptr) {
        return child;
    }

    int node_x = this->x + (x_idx * get_w(this->node_level - 1));
    int node_y = this->y + (y_idx * get_w(this->node_level - 1));

    if (this->node_level == 1) {
        child = new Node<Tile>(node_x, node_y, 0);
    }
    else {
        child = new Node<NodeBase *>(node_x, node_y, this->node_level - 1);
    }

    return child;
}

template<>
Tile & TileTree::Node<Tile>::try_make_child(int x_idx, int y_idx) {
    Tile & t = this->children[get_idx(x_idx, y_idx)];
    t.x = this->x + x_idx;
    t.y = this->y + y_idx;
    return t;
}


template<typename Child_t>
TileTree::NodeBase * TileTree::Node<Child_t>::make_parent(int x_idx, int y_idx) {
    int x = this->x - x_idx * this->get_w();
    int y = this->y - y_idx * this->get_h();
    Node<NodeBase *> * parent = new Node<NodeBase *>(x, y, this->node_level + 1);
    parent->set_child(x_idx, y_idx, this);
    return parent;
}


template<>
void TileTree::Node<TileTree::NodeBase *>::insert_tile(const Tile & t) {
    int x_idx, y_idx;
    tile_to_grid_coords(t.x, t.y, x_idx, y_idx);
    NodeBase * & child = try_make_child(x_idx, y_idx);

    child->insert_tile(t);
}

template<>
void TileTree::Node<Tile>::insert_tile(const Tile & t) {
    int x_idx, y_idx;
    tile_to_grid_coords(t.x, t.y, x_idx, y_idx);
    Tile & tile = try_make_child(x_idx, y_idx);

    tile = t;
}


template<typename Child_t>
void TileTree::Node<Child_t>::set_child(int x_idx, int y_idx, Child_t && child) {
    children[get_idx(x_idx, y_idx)] = std::forward<Child_t>(child);
}


template<typename Child_t>
Child_t * TileTree::Node<Child_t>::get_child_idx(int idx) {
    return &children[idx];
}


template<typename Child_t>
TileTree::NodeBase * TileTree::Node<Child_t>::get_child(int x_idx, int y_idx) {
    return children[get_idx(x_idx, y_idx)];
}

// leaf nodes do not have children
template<>
TileTree::NodeBase * TileTree::Node<Tile>::get_child(int x_idx, int y_idx) {
    return nullptr;
}

template<typename Child_t>
bool TileTree::Node<Child_t>::is_leaf() const {
    return false;
}


template<>
bool TileTree::Node<Tile>::is_leaf() const {
    return true;
}


TileTree::NodeBase * TileTree::find_parent_of(int tile_x, int tile_y) {
    NodeBase * node = this->root;

    if (node == nullptr) {
        return nullptr;
    }

    if (!node->contains(tile_x, tile_y)) {
        return nullptr;
    }

    while (!node->is_leaf()) {
        NodeBase * child = node->find_child_containing(tile_x, tile_y);
        if (child == nullptr) {
            break;
        }
        node = child;
    }

    return node;
}


void TileTree::prepare_insert(int x, int y) {
    NodeBase * root = this->root;
    int logw, logh, dx, dy;

    if (root == NULL) {
        // make an 8-tile-aligned node containing this tile
        int node_x = ALIGN_DOWN(x, NodeBase::children_dim);
        int node_y = ALIGN_DOWN(y, NodeBase::children_dim);
        root = new Node<Tile>(node_x, node_y, 0);
    }

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
                    dy < NodeBase::children_dim && dy >= -NodeBase::children_dim)) {

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


void TileTree::do_insert(const Tile & t) {
    root->insert_tile(t);
}


void TileTree::print_node(const NodeBase & node) const {
    int level = root->node_level - node.node_level;
    printf("%*sNode (%p) (%d, %d), %dx%d\n", level, "", &node, node.x, node.y,
            node.get_w(), node.get_h());

    const Node<Tile> * leaf = dynamic_cast<const Node<Tile> *>(&node);
    if (leaf != nullptr) {
        for (int i = 0; i < NodeBase::branch_factor; i++) {
            if (leaf->children[i].texset != nullptr) {
                printf("%*sLeaf (%d, %d)\n", level + 1, "", leaf->children[i].x, leaf->children[i].y);
            }
        }
    }
    else {
        const Node<NodeBase *> * n = dynamic_cast<const Node<NodeBase *> *>(&node);
        assert(n != nullptr);
        for (int i = 0; i < NodeBase::branch_factor; i++) {
            if (n->children[i] != nullptr) {
                print_node(*n->children[i]);
            }
        }
    }
}



TileTree::TileTree() : root(nullptr) {
}


TileTree::~TileTree() {
    if (root != nullptr) {
        delete root;
    }
}


void TileTree::clear() {
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}


void TileTree::insert_tile(const Tile & t) {
    prepare_insert(t.x, t.y);
    do_insert(t);
}



TileTree::iterator::iterator(TileTree & owner, int llx, int lly, int urx, int ury) :
        owner(owner), llx(llx), lly(lly), urx(urx), ury(ury) {

    cur_region = owner.find_parent_of(llx, lly);
}


const Tile & TileTree::iterator::operator*() const {
    Node<Tile> * n;

    if (cur_region == nullptr) {
        throw new std::runtime_error("Dereferenced TileTree iterator beyond the "
                "valid range of iteration");
    }

    n = dynamic_cast<Node<Tile> *>(cur_region);
    assert(n != nullptr);

    return *n->get_child_idx(cur_idx);
}


TileTree::iterator & TileTree::iterator::operator++() {
    return *this;
}



TileTree::iterator TileTree::begin() {
    if (this->root == nullptr) {
        return iterator(*this, 0, 0, 0, 0);
    }
    else {
        int x, y, w, h;
        x = root->x;
        y = root->y;
        w = root->get_w();
        h = root->get_h();
        return iterator(*this, x, y, x + w, y + h);
    }
}


TileTree::iterator TileTree::end() {
    if (this->root == nullptr) {
        return iterator(*this, 0, 0, 0, 0);
    }
    else {
        int x, y, w, h;
        x = root->x;
        y = root->y;
        w = root->get_w();
        h = root->get_h();
        return iterator(*this, x + w, y + h, x + w, y + h);
    }
}



TileTree::iterator TileTree::find_all(int llx, int lly, int urx, int ury) {

    return iterator(*this, llx, lly, urx, ury);
}




void TileTree::print_tree() const {
    if (root != nullptr) {
        print_node(*root);
    }
}


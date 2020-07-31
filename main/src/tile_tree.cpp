
#include <stdexcept>
#include <utility>

#include <util.h>

#include <chunk.h>
#include <tile_tree.h>



TileTree::NodeBase::NodeBase(int x, int y, uint8_t level) :
    parent(nullptr), x(x), y(y), node_level(level), occ_b(0) {

    assert(level < 20);
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


uint8_t TileTree::NodeBase::get_node_level() const {
    return this->node_level;
}


void TileTree::NodeBase::mark_occupied(int x_idx, int y_idx) {
    int idx = x_idx + y_idx * children_dim;
    this->occ_b |= (1LU << idx);
}


uint32_t TileTree::NodeBase::get_idx(int x_idx, int y_idx) {
    return y_idx * children_dim + x_idx;
}


uint32_t TileTree::NodeBase::get_tile_idx(int tile_x, int tile_y) const {
    int x_idx, y_idx;
    tile_to_grid_coords(tile_x, tile_y, x_idx, y_idx);
    return get_idx(x_idx, y_idx);
}


void TileTree::NodeBase::tile_to_grid_coords(int tile_x, int tile_y,
        int & x_idx, int & y_idx) const {
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


node_bmask_t TileTree::NodeBase::gen_clipped_bmask(int llx, int lly,
        int urx, int ury) const {

    int dllx, dlly, durx, dury;

    tile_to_grid_coords(llx, lly, dllx, dlly);

    // round up the upper right coordinates to the tiles just beyond what
    // contains (urx, ury)
    tile_to_grid_coords(urx - 1, ury - 1, durx, dury);
    urx++;
    ury++;

    // if < 0, set to 0, otherwise keep the same
    dllx &= ~(dllx >> 31);
    dlly &= ~(dlly >> 31);
    durx &= ~(durx >> 31);
    dury &= ~(dury >> 31);

    // if > 8, set to 8, otherwise keep the same
    dllx = (dllx > children_dim) ? children_dim : dllx;
    dlly = (dlly > children_dim) ? children_dim : dlly;
    durx = (durx > children_dim) ? children_dim : durx;
    dury = (dury > children_dim) ? children_dim : dury;

    node_bmask_t x_bmask = (~((0x1lu << dllx) - 1)) & ((0x1lu << durx) - 1);
    node_bmask_t y_bmask = (~((0x1lu << dlly) - 1)) & ((0x1lu << dury) - 1);

    // splay y_bmask (max 0xff bits) evenly across the 64-bit number
    y_bmask = (y_bmask | (y_bmask << 28)) & 0x0000000f0000000flu;
    y_bmask = (y_bmask | (y_bmask << 14)) & 0x0003000300030003lu;
    y_bmask = (y_bmask | (y_bmask <<  7)) & 0x0101010101010101lu;

    // combine x and y bitmasks, for each set bit in the y bitvector, place a
    // copy of the x bitvector in the corresponding byte
    node_bmask_t clip = x_bmask * y_bmask;

    return clip & this->occ_b;
}


template<typename Child_t>
TileTree::Node<Child_t>::Node(int x, int y, uint8_t level) :
    NodeBase(x, y, level), children{} {}


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

    mark_occupied(x_idx, y_idx);

    return child;
}

template<>
Tile & TileTree::Node<Tile>::try_make_child(int x_idx, int y_idx) {
    Tile & t = this->children[get_idx(x_idx, y_idx)];
    t.x = this->x + x_idx;
    t.y = this->y + y_idx;
    mark_occupied(x_idx, y_idx);
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
    mark_occupied(x_idx, y_idx);
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

        printf("%d %d (%d)\n", dx, dy, NodeBase::children_dim);

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


uint8_t TileTree::get_tree_depth() const {
    return root->get_node_level() + 1;
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
        owner(owner), llx(llx), lly(lly), urx(urx), ury(ury),
        tree_depth(owner.get_tree_depth()) {

    region_stack = new stack_node[this->tree_depth];

    if (!add_to_stack(*owner.root)) {
        make_end();
    }
}

TileTree::iterator::iterator(TileTree & owner) : owner(owner),
        region_stack(nullptr) {
}


bool TileTree::iterator::is_end() const {
    return region_stack == nullptr;
}


void TileTree::iterator::make_end() {
    if (region_stack != nullptr) {
        delete [] region_stack;
        region_stack = nullptr;
    }
}


uint8_t TileTree::iterator::_get_next_idx(stack_node & s_node) {
    node_bmask_t search_mask;
    uint8_t next_idx;

    // find index of next unvisited child
    next_idx = tzcnt64(s_node.bmask);

    // mask off that child, which we are about to visit
    search_mask = (1LU << next_idx);
    s_node.bmask &= ~search_mask;

    return next_idx;
}


TileTree::NodeBase * TileTree::iterator::get_next_child(
        TileTree::iterator::stack_node & s_node) {

    Node<NodeBase *> * node;
    uint8_t next_idx;

    // node must not be a leaf;
    node = dynamic_cast<Node<NodeBase *> *>(s_node.node);
    assert(node != nullptr);

    if (s_node.bmask == 0) {
        // no children left
        return nullptr;
    }

    next_idx = _get_next_idx(s_node);

    return *node->get_child_idx(next_idx);
}


int TileTree::iterator::to_next_tile(
        TileTree::iterator::stack_node & s_node) {

    Node<Tile> * leaf;
    uint8_t next_idx;

    // node must be a leaf;
    leaf = dynamic_cast<Node<Tile> *>(s_node.node);
    assert(leaf != nullptr);

    if (s_node.bmask == 0) {
        // no children left
        return 0;
    }

    next_idx = _get_next_idx(s_node);

    // cache next_idx for lookup when dereference operator is called
    cur_idx = next_idx;

    return 1;
}



int TileTree::iterator::add_to_stack(NodeBase & node) {

    node_bmask_t bmask = node.gen_clipped_bmask(llx, lly, urx, ury);
    if (bmask == 0) {
        // no children within the bounds of iteration, so skip
        return 0;
    }

    // add the node to the stack
    uint8_t stack_idx = node.get_node_level();
    stack_node & s_node = this->region_stack[stack_idx];

    s_node.node  = &node;
    s_node.bmask = bmask;

    // if leaf nodes have any children in bounds, we must iterate through them
    // regardless, so we can return success immediately
    if (node.is_leaf()) {
        return 1;
    }

    // for non-leaf nodes, we have to check to see if their children contain
    // any nodes in bounds
    return find_next(s_node);
}


int TileTree::iterator::find_next(stack_node & s_node) {
    NodeBase * next_child;

    // this method is only for non-leaf nodes
    assert(!s_node.node->is_leaf());

    // check to see if any more children of this node have tiles to iterate
    // through
    while ((next_child = get_next_child(s_node)) != nullptr) {
        if (add_to_stack(*next_child)) {
            return 1;
        }
    }

    return 0;
}


TileTree::iterator::~iterator() {
    if (region_stack != nullptr) {
        delete [] region_stack;
    }
}


const Tile & TileTree::iterator::operator*() const {
    Node<Tile> * n;

    if (is_end()) {
        throw new std::runtime_error("Dereferenced TileTree iterator beyond the "
                "valid range of iteration");
    }

    n = dynamic_cast<Node<Tile> *>(region_stack[0].node);
    assert(n != nullptr);

    return *n->get_child_idx(cur_idx);
}


TileTree::iterator & TileTree::iterator::operator++() {

    if (to_next_tile(region_stack[0])) {
        return *this;
    }

    for (uint8_t idx = 1; idx < this->tree_depth; idx++) {
        if (find_next(region_stack[idx])) {
            return *this;
        }
    }
    make_end();
    return *this;
}


bool TileTree::iterator::operator==(const iterator & other) const {
    if (&this->owner != &other.owner) {
        // iterators are not the same if they belong to different trees
        return false;
    }

    if (this->is_end() && other.is_end()) {
        // if both are end iterators, then they are equal
        return true;
    }

    if (this->tree_depth != other.tree_depth) {
        // at least one of these iterators must be invalid
        throw new std::runtime_error("Comparison of at least one invalid "
                "TileTree iterator results in undefined/potentially dangerous "
                "behavior");
    }

    int same_stack = __builtin_memcmp(this->region_stack, other.region_stack,
            this->tree_depth * sizeof(stack_node));

    return same_stack && llx == other.llx && lly == other.lly &&
        urx == other.urx && ury == other.ury;
}


bool TileTree::iterator::operator!=(const iterator & other) const {
    return !(*this == other);
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
    return iterator(*this);
}



TileTree::iterator TileTree::find_all(int llx, int lly, int urx, int ury) {

    return iterator(*this, llx, lly, urx, ury);
}




void TileTree::print_tree() const {
    if (root != nullptr) {
        print_node(*root);
    }
}



#pragma once

#include <stdint.h>

#include <tile.h>


typedef uint64_t node_bmask_t;


class TileTree {
private:

    class NodeBase {
    public:

        static constexpr uint32_t log_children_dim = 3;

        static constexpr uint32_t children_dim = (1U << log_children_dim);
        // each node has at most 64 children (8x8 grid)
        static constexpr uint32_t branch_factor = children_dim * children_dim;

        // add virtual function to do traversing

        NodeBase * parent;

        // (x, y) coordinate of lower left corner of the node, will be the same
        // as the (x, y) coordinate of the lower-leftmost chunk
        int x, y;
        // 0 at leaves, each child is one less than their parent's level
        // 2 ^^ (log_children_dim * (node_level + 1)) yields the width and
        // height of this Node, in tiles
        uint8_t node_level;

        // bitmask of occupied chunks
        node_bmask_t occ_b;

        NodeBase(int x, int y, uint8_t level);
        virtual ~NodeBase();

        static int get_log_w(uint8_t node_level);
        static int get_log_h(uint8_t node_level);
        static int get_w(uint8_t node_level);
        static int get_h(uint8_t node_level);

        int get_log_w() const;
        int get_log_h() const;
        int get_w() const;
        int get_h() const;

        uint8_t get_node_level() const;

        /*
         * gives the mapping from 2-dimensional (x_idx, y_idx) coordinates in
         * children grid to the index of that child in the children array
         */
        static uint32_t get_idx(int x_idx, int y_idx);

        /*
         * gives the mapping from tile coordinates in the world to the index
         * of the child that would contain those coordinates in the children
         * array
         */
        uint32_t get_tile_idx(int tile_x, int tile_y) const;

        /*
         * returns the grid coordinates of the child containing the tile at
         * world coordinate pair (tile_x, tile_y)
         */
        void tile_to_grid_coords(int tile_x, int tile_y, int & x_idx,
                int & y_idx) const;


        /*
         * returns true if the given board coordinates lie within this child
         * (i.e. could be a descendent of this node)
         */
        bool contains(int tile_x, int tile_y) const;

        NodeBase * find_child_containing(int tile_x, int tile_y);

        /*
         * given the bounds of a rectangular region, returns a bitmask
         * corresponding to the set of children of this node which lie at
         * least partially in the region
         *
         * the upper bounds of the rectangular region are non-inclusive
         */
        node_bmask_t gen_clipped_bmask(int llx, int lly, int urx, int ury) const;

        /*
         * creates a parent node for the node, with the correct template of
         * Node
         *
         * the supplied x_idx and y_idx values are the coordinates in the
         * children grid at which the current Node should reside in the new
         * parent
         */
        virtual NodeBase * make_parent(int x_idx, int y_idx) = 0;

        virtual void insert_tile(const Tile & t) = 0;

        /*
         * given coordinates (child_x, child_y) in child coordinates
         * (i.e. (0, 0) is the lower leftmost child, (7, 7) is the upper
         * rightmost), returns the child at that spot, or nullptr if there
         * is none
         */
        virtual NodeBase * get_child(int x_idx, int y_idx) = 0;

        /*
         * true if this node is a leaf node, false otherwise
         */
        virtual bool is_leaf() const = 0;

    };

    template<typename Child_t>
    class __attribute__((aligned(128))) Node : public NodeBase {
    public:

        Child_t children[branch_factor];

        Node(int x, int y, uint8_t level);
        virtual ~Node();

        /*
         * makes a child node at the given children-grid coordinates, returning
         * a reference to the newly created child afetr inserting it into the
         * children array. If there was already a child there, it simply returns
         * a reference to it
         */
        Child_t & try_make_child(int x_idx, int y_idx);

        virtual NodeBase * make_parent(int x_idx, int y_idx);
        
        virtual void insert_tile(const Tile & t);

        /*
         * sets the child of the node at the given index (x_idx, y_idx) in the
         * 8x8 children grid to the given child
         */
        virtual void set_child(int x_idx, int y_idx, Child_t && child);

        /*
         * get a tile at a specific index
         */
        Child_t * get_child_idx(int idx);

        virtual NodeBase * get_child(int x_idx, int y_idx);
        
        virtual bool is_leaf() const;

    };


    /*
     * given coordinates (tile_x, tile_y), gives the lowest-level node that
     * would contain that tile if it were to exist (which it may or may not),
     * or nullptr if there is no such node
     */
    NodeBase * find_parent_of(int tile_x, int tile_y);


    /*
     * prepare insertion of a tile at coordinates (x, y) by building up the
     * tree until a node is created which contains these coordinates
     */
    void prepare_insert(int x, int y);

    /*
     * insert the given tile, assuming that the tile already lies within the
     * region covered by the root node
     */
    void do_insert(const Tile & t);

    /*
     * returns the depth of the tree (1 if only a leaf, 2 if one parent with
     * leaf children, etc.)
     */
    uint8_t get_tree_depth() const;


    void print_node(const NodeBase & node) const;


    NodeBase * root;

public:

    TileTree();
    ~TileTree();

    /*
     * erases all contents of the tree
     */
    void clear();

    void insert_tile(const Tile & t);

    TileTree & operator<<(const Tile & t);

    /*
     * removes a tile from the given coordinates (x, y)
     */
    void remove_tile(int x, int y);


    class iterator {
        friend class TileTree;
    private:

        struct stack_node {
            NodeBase * node;
            node_bmask_t bmask;
        };

        TileTree & owner;

        /*
         * a stack of the nodes we are searching through, with the root at the
         * bottom and leaves at the top
         *
         * set to nullptr when the iteration is complete
         */
        stack_node * region_stack;

        int llx, lly, urx, ury;

        /*
         * cached index of the current tile the iterator is on in the top
         * node's children array in region_stack
         */
        int cur_idx;

        /*
         * cached depth of owner, for easy lookup
         */
        int tree_depth;

        /*
         * constructs an iterator over the given rectangular region (urx and ury
         * give exclusive upper bounds)
         */
        iterator(TileTree & owner, int llx, int lly, int urx, int ury);
        /*
         * constructs an end-iterator
         */
        iterator(TileTree & owner);

        /*
         * returns true if this iterator has nothing left to iterate through
         */
        bool is_end() const;

        /*
         * marks an iterator as the end, to be called when ++ is called after
         * all tiles have been iterated over
         */
        void make_end();


        /*
         * returns the index of the next child to iterate through, updating the
         * bmask of node accordingly
         */
        uint8_t _get_next_idx(stack_node & node);

        /*
         * finds the next child of node and returns it, if there is one,
         * otherwise returning nullptr
         *
         * node must not correspond to a leaf node
         */
        NodeBase * get_next_child(stack_node & node);

        /*
         * iterates to the next tile in node which has not yet been iterated
         * through and returns 1 if there is one, otherwise returning 0
         *
         * node must correspond to a leaf node
         */
        int to_next_tile(stack_node & node);


        /*
         * creates a node_stack node for the given node and recursively calls
         * find_next if node is not a leaf
         */
        int add_to_stack(NodeBase & node);

        /*
         * recursively finds the next leaf node to be iterated over, returning
         * 1 if one could be found, 0 otherwise (calls _find_next)
         *
         * populates the node stack if a next leaf is found
         */
        int find_next(stack_node & node);

    public:

        ~iterator();

        const Tile & operator*() const;

        iterator & operator++();

        bool operator==(const iterator & other) const;

    };


    iterator begin();

    iterator end();

    /*
     * returns an iterator through all of the tiles within the supplied
     * rectangle (in board coordinates)
     */
    iterator find_all(int llx, int lly, int urx, int ury);

    /*
     * for debugging, prints the tree visually
     */
    void print_tree() const;

};



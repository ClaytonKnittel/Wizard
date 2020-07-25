#pragma once

#include <stdint.h>

#include <tile.h>

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

    };

    template<typename Child_t>
    class __attribute__((aligned(128))) Node : public NodeBase {
    public:

        Child_t children[branch_factor];
        // bitmask of occupied chunks
        uint64_t occ_b;

        Node(int x, int y, uint8_t level);
        virtual ~Node();

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
        uint32_t get_tile_idx(int tile_x, int tile_y);

        /*
         * returns the grid coordinates of the child containing the tile at
         * world coordinate pair (tile_x, tile_y)
         */
        void tile_to_grid_coords(int tile_x, int tile_y, int & x_idx,
                int & y_idx);

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

    };


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


    struct iterator {
    };

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



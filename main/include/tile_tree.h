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

        int get_log_w();
        int get_log_h();
        int get_w();
        int get_h();

        /*
         * creates a parent node for the node, with the correct template of
         * Node
         *
         * the supplied x_idx and y_idx values are the coordinates in the
         * children grid at which the current Node should reside in the new
         * parent
         */
        virtual NodeBase * make_parent(int x_idx, int y_idx) = 0;
    };

    template<typename Child_t>
    class __attribute__((aligned(128))) Node : public NodeBase {
    public:

        Child_t children[branch_factor];
        // bitmask of occupied chunks
        uint64_t occ_b;

        Node(int x, int y, uint8_t level);
        virtual ~Node();

        virtual NodeBase * make_parent(int x_idx, int y_idx);

    };


    /*
     * prepare insertion of a tile at coordinates (x, y) by building up the
     * tree until a node is created which contains these coordinates
     */
    void prepare_insert(int x, int y);


    NodeBase * root;

public:

    TileTree();
    ~TileTree();

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

};



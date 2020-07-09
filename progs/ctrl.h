#pragma once

class Node {
private:

    bool terminal;

    //void (*callback)(void);
    std::function<void(void)> * callback;

    std::unordered_map<char, Node *> children;

public:

    Node(bool terminal, std::function<void(void)> *callback=nullptr) :
            terminal(terminal), callback(callback) {}

    ~Node() {
        for (auto it = children.begin(); it != children.end(); it++) {
            delete it->second;
        }
        if (terminal) {
            assert(callback != nullptr);
            delete callback;
        }
    }

    bool is_terminal() {
        return terminal;
    }

    Node & add_child(int key) {
        Node * succ;
        auto it = children.find(key);
        if (it == children.end()) {
            succ = new Node(false);
            children.insert(std::pair<char, Node *>(key, succ));
        }
        else {
            succ = it->second;
        }
        return *succ;
    }

    Node * get_child(int key) {
        auto it = children.find(key);
        if (it == children.end()) {
            return nullptr;
        }
        return it->second;
    }

    void make_terminal(std::function<void(void)> &&callback) {
        terminal = true;
        this->callback = new std::function<void(void)>(
                std::forward<std::function<void(void)>>(callback));
    }

    void execute() {
        (*this->callback)();
    }

};


class Controller {
private:

    Node root;
    Node * state;

public:

    Controller() : root(false), state(&root) {
    }

    ~Controller() {}

    void press(int key) {
        Node * next = state->get_child(key);
        if (next == nullptr) {
            state = &root;
        }
        else if (next->is_terminal()) {
            next->execute();
            state = &root;
        }
        else {
            state = next;
        }
    }

    Node & get_root() {
        return root;
    }

};


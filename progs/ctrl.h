#pragma once

class Node {
private:

    bool terminal;

    //void (*callback)(void);
    std::function<void(void)> * callback;
    // to be called when this node is switched out of context
    std::function<void(void)> * exit_callback;

    std::unordered_map<char, Node *> children;

public:

    Node(bool terminal) :
            terminal(terminal), callback(nullptr), exit_callback(nullptr) {}

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

    bool has_exit_callback() const {
        return this->exit_callback != nullptr;
    }

    void make_terminal(std::function<void(void)> &&callback) {
        terminal = true;
        this->callback = new std::function<void(void)>(
                std::forward<std::function<void(void)>>(callback));
    }

    void add_exit_callback(std::function<void(void)> &&exit_callback) {
        this->exit_callback = new std::function<void(void)>(
                std::forward<std::function<void(void)>>(exit_callback));
    }

    void execute() {
        (*this->callback)();
    }

    void execute_exit() {
        (*this->exit_callback)();
    }

};


class Controller {
private:

    Node root;
    Node * state;
    // last state to have been executed
    Node * in_scope;

public:

    Controller() : root(false), state(&root), in_scope(nullptr) {
    }

    ~Controller() {}

    void press(int key) {
        Node * next = state->get_child(key);
        if (next == nullptr) {
            state = &root;
        }
        else if (next->is_terminal()) {
            if (in_scope != nullptr) {
                if (in_scope->has_exit_callback()) {
                    in_scope->execute_exit();
                }
            }
            next->execute();
            in_scope = next;
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


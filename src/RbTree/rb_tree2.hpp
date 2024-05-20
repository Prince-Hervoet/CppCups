#include <iostream>
#include <utility>  // For std::pair

enum class Color { Red, Black };

template <typename Key, typename Value>
class RedBlackTree {
 private:
  struct Node {
    std::pair<const Key, Value> data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(const std::pair<const Key, Value>& data)
        : data(data),
          color(Color::Red),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}
  };

  Node* root;

  Node* grandparent(Node* n) const {
    if (n && n->parent) {
      return n->parent->parent;
    }
    return nullptr;
  }

  Node* uncle(Node* n) const {
    Node* g = grandparent(n);
    if (g == nullptr) {
      return nullptr;
    }
    if (n->parent == g->left) {
      return g->right;
    } else {
      return g->left;
    }
  }

  Node* sibling(Node* n) const {
    if (n->parent == nullptr) {
      return nullptr;
    }
    if (n == n->parent->left) {
      return n->parent->right;
    } else {
      return n->parent->left;
    }
  }

  void rotateLeft(Node* n) {
    Node* new_parent = n->right;
    if (n->parent == nullptr) {
      root = new_parent;
    } else if (n == n->parent->left) {
      n->parent->left = new_parent;
    } else {
      n->parent->right = new_parent;
    }
    new_parent->parent = n->parent;
    n->parent = new_parent;
    n->right = new_parent->left;
    if (new_parent->left != nullptr) {
      new_parent->left->parent = n;
    }
    new_parent->left = n;
  }

  void rotateRight(Node* n) {
    Node* new_parent = n->left;
    if (n->parent == nullptr) {
      root = new_parent;
    } else if (n == n->parent->left) {
      n->parent->left = new_parent;
    } else {
      n->parent->right = new_parent;
    }
    new_parent->parent = n->parent;
    n->parent = new_parent;
    n->left = new_parent->right;
    if (new_parent->right != nullptr) {
      new_parent->right->parent = n;
    }
    new_parent->right = n;
  }

  void fixInsertion(Node* n) {
    while (n != root && n->parent->color == Color::Red) {
      Node* g = grandparent(n);
      Node* u = uncle(n);

      if (u != nullptr && u->color == Color::Red) {
        n->parent->color = Color::Black;
        u->color = Color::Black;
        g->color = Color::Red;
        n = g;
      } else {
        if (n->parent == g->left) {
          if (n == n->parent->right) {
            n = n->parent;
            rotateLeft(n);
          }
          n->parent->color = Color::Black;
          g->color = Color::Red;
          rotateRight(g);
        } else {
          if (n == n->parent->left) {
            n = n->parent;
            rotateRight(n);
          }
          n->parent->color = Color::Black;
          g->color = Color::Red;
          rotateLeft(g);
        }
      }
    }
    root->color = Color::Black;
  }

  Node* minimum(Node* n) const {
    while (n->left != nullptr) {
      n = n->left;
    }
    return n;
  }

  void transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

  void fixDeletion(Node* n) {
    while (n != root && (n == nullptr || n->color == Color::Black)) {
      if (n == n->parent->left) {
        Node* s = n->parent->right;
        if (s->color == Color::Red) {
          s->color = Color::Black;
          n->parent->color = Color::Red;
          rotateLeft(n->parent);
          s = n->parent->right;
        }
        if ((s->left == nullptr || s->left->color == Color::Black) &&
            (s->right == nullptr || s->right->color == Color::Black)) {
          s->color = Color::Red;
          n = n->parent;
        } else {
          if (s->right == nullptr || s->right->color == Color::Black) {
            s->left->color = Color::Black;
            s->color = Color::Red;
            rotateRight(s);
            s = n->parent->right;
          }
          s->color = n->parent->color;
          n->parent->color = Color::Black;
          s->right->color = Color::Black;
          rotateLeft(n->parent);
          n = root;
        }
      } else {
        Node* s = n->parent->left;
        if (s->color == Color::Red) {
          s->color = Color::Black;
          n->parent->color = Color::Red;
          rotateRight(n->parent);
          s = n->parent->left;
        }
        if ((s->right == nullptr || s->right->color == Color::Black) &&
            (s->left == nullptr || s->left->color == Color::Black)) {
          s->color = Color::Red;
          n = n->parent;
        } else {
          if (s->left == nullptr || s->left->color == Color::Black) {
            s->right->color = Color::Black;
            s->color = Color::Red;
            rotateLeft(s);
            s = n->parent->left;
          }
          s->color = n->parent->color;
          n->parent->color = Color::Black;
          s->left->color = Color::Black;
          rotateRight(n->parent);
          n = root;
        }
      }
    }
    if (n != nullptr) {
      n->color = Color::Black;
    }
  }

 public:
  RedBlackTree() : root(nullptr) {}

  void insert(const Key& key, const Value& value) {
    Node* new_node = new Node(std::make_pair(key, value));
    Node* y = nullptr;
    Node* x = root;

    while (x != nullptr) {
      y = x;
      if (new_node->data.first < x->data.first) {
        x = x->left;
      } else {
        x = x->right;
      }
    }
    new_node->parent = y;
    if (y == nullptr) {
      root = new_node;
    } else if (new_node->data.first < y->data.first) {
      y->left = new_node;
    } else {
      y->right = new_node;
    }
    new_node->color = Color::Red;
    fixInsertion(new_node);
  }

  void remove(const Key& key) {
    Node* z = root;
    while (z != nullptr) {
      if (key == z->data.first) {
        break;
      } else if (key < z->data.first) {
        z = z->left;
      } else {
        z = z->right;
      }
    }
    if (z == nullptr) {
      return;
    }
    Node* y = z;
    Color original_color = y->color;
    Node* x = nullptr;
    if (z->left == nullptr) {
      x = z->right;
      transplant(z, z->right);
    } else if (z->right == nullptr) {
      x = z->left;
      transplant(z, z->left);
    } else {
      y = minimum(z->right);
      original_color = y->color;
      x = y->right;
      if (y->parent == z) {
        if (x != nullptr) {
          x->parent = y;
        }
      } else {
        transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    if (original_color == Color::Black) {
      fixDeletion(x);
    }
    delete z;
  }

  Value* search(const Key& key) const {
    Node* current = root;
    while (current != nullptr) {
      if (key == current->data.first) {
        return &(current->data.second);
      } else if (key < current->data.first) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return nullptr;
  }

  void inorderTraversal(Node* node) const {
    if (node == nullptr) {
      return;
    }
    inorderTraversal(node->left);
    std::cout << node->data.first << " : " << node->data.second << " ";
    inorderTraversal(node->right);
  }

  void inorderTraversal() const {
    inorderTraversal(root);
    std::cout << std::endl;
  }

  ~RedBlackTree() { deleteTree(root); }

 private:
  void deleteTree(Node* node) {
    if (node != nullptr) {
      deleteTree(node->left);
      deleteTree(node->right);
      delete node;
    }
  }
};

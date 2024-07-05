#include <ostream>
#include <vector>

/**
 * Base class for segment trees
 * Subclasses should override the following methods from SegmentTreeNode:
 * - fromVal(T val)
 * - combineVals(U leftVal, U rightVal, int leftIndex, int rightIndex)
 * - update()
 *
 * Subclasses should also override the following methods from SegmentTree:
 * - createSegmentTreeNode(T val, int leftIndex, int rightIndex)
 * - createSegmentTreeNode(SegmentTreeNode *left, SegmentTreeNode *right)
 *
 * @param T type of value in the input array
 * @param U type of value that the segment tree calculates/stores
 */
template <typename T, typename U> class SegmentTree {

protected:
  /**
   * This is where most of the segment tree work happens
   */
  class SegmentTreeNode {
  public:
    SegmentTreeNode *left, *right, *parent;
    SegmentTree<T, U> *tree;
    int leftIndex, rightIndex;
    U val;
    SegmentTreeNode()
        : left(nullptr), right(nullptr), parent(nullptr), tree(nullptr),
          leftIndex(-1), rightIndex(-1) {}
    virtual ~SegmentTreeNode() {
      // delete children, but don't delete parent since
      // this is a recursive method
      if (this->left) {
        delete this->left;
      }
      if (this->right) {
        delete this->right;
      }
      this->left = this->right = this->parent = nullptr;
      this->parent = nullptr;
    };

    virtual void init(SegmentTree<T, T> *tree, T val, int leftIndex,
                      int rightIndex) {
      this->tree = tree;
      this->val = fromVal(val);
      this->leftIndex = leftIndex;
      this->rightIndex = rightIndex;
      this->left = this->right = this->parent = nullptr;
    }
    virtual void init(SegmentTree<T, T> *tree, SegmentTreeNode *left,
                      SegmentTreeNode *right) {
      this->tree = tree;
      this->left = left;
      this->right = right;
      this->leftIndex = left->leftIndex;
      this->rightIndex = left->rightIndex; // updated if right is not null

      // set parents
      if (this->left) {
        this->left->parent = this;
      }
      if (this->right) {
        this->right->parent = this;
        this->rightIndex = right->rightIndex;
      }
      update(); // set this->val
    }

    /**
     * Convert the given value to this->val
     */
    virtual U fromVal(T val) const = 0;

    /**
     * Updates this->val to be accurate
     * implementation specific
     */
    virtual void update() = 0;

    /**
     * Updates this->val
     * Should only be called on leaf nodes
     * since it recursively goes up the tree
     * and updates any parent nodes
     */
    void update(T val) {
      this->val = fromVal(val);

      SegmentTreeNode *node = this->parent;
      while (node) {
        node->update();
        node = node->parent;
      }
    }

    /**
     * Combines two values, called during query
     * Only called when the node is partially within the query
     * a child may be null. This method should account for that
     */
    virtual U combineVals(U leftVal, U rightVal, int leftIndex,
                          int rightIndex) const = 0;

    U query(int left, int right) const {
      // if the node is completely w/i the query, return the value
      if (this->leftIndex >= left && this->rightIndex <= right) {
        return this->val;
      }
      // if the node is not within the query, return 0
      else if (right < this->leftIndex || left > this->rightIndex) {
        return 0;
      }
      // query the children
      else {
        U leftVal, rightVal;
        if (this->left) {
          leftVal = this->left->query(left, right);
        }
        if (this->right) {
          rightVal = this->right->query(left, right);
        }
        return combineVals(leftVal, rightVal, left, right);
      }
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const SegmentTreeNode *node) {
      os << node->val;
      if (node->left) {
        os << " left: (" << node->left << ")";
      } else {
        os << " left: null";
      }
      if (node->right) {
        os << " right: (" << node->right << ")";
      } else {
        os << " right: null";
      }
      return os;
    }
  };

  SegmentTreeNode *root; //< root of segment tree, null until initialized
  std::vector<SegmentTreeNode *> leafNodes; //< leaf nodes of tree
  std::vector<T> &arr;                      //< array of input values

protected:
  SegmentTree(std::vector<T> &arr) : root(nullptr), arr(arr) {}
  virtual ~SegmentTree() {
    // will recursively delete all nodes
    // to avoid memory leaks
    delete root;
  }

  /**
   * Initialize the segment tree
   * Must be called before querying/updating
   */
  void init() {
    // initialize bottom of our tree
    for (int i = 0; i < arr.size(); i++) {
      leafNodes.push_back(createSegmentTreeNode(arr[i], i, i));
    }

    // build the tree up
    std::vector<SegmentTreeNode *> nodes = leafNodes;
    while (nodes.size() > 1) {
      std::vector<SegmentTreeNode *> newNodes;
      for (int i = 0; i < nodes.size(); i += 2) {
        if (i + 1 < nodes.size()) {
          newNodes.push_back(createSegmentTreeNode(nodes[i], nodes[i + 1]));
        } else {
          newNodes.push_back(createSegmentTreeNode(nodes[i], nullptr));
        }
      }
      nodes = newNodes;
    }
    root = nodes[0];
  }

  /**
   * Factory methods
   */
  virtual SegmentTreeNode *createSegmentTreeNode(T val, int leftIndex,
                                                 int rightIndex) = 0;
  virtual SegmentTreeNode *createSegmentTreeNode(SegmentTreeNode *left,
                                                 SegmentTreeNode *right) = 0;

public:
  const std::vector<T> &getArr() const { return this->arr; }

  U query(int left, int right) { return root->query(left, right); }

  /**
   * @brief Updates the value at the given index. Note: arr is updated too
   *
   * @param index
   * @param val
   */
  void update(int index, T val) {
    arr[index] = val;
    leafNodes[index]->update(val);
  }

  friend std::ostream &operator<<(std::ostream &os, SegmentTree<T, U> &tree) {
    return os << tree.root;
  }
};
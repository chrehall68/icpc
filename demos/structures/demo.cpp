#include "segment_tree.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

/**
 * Min Segment Tree
 * Example of a segment tree
 */
template <typename T> class MinSegmentTree : public SegmentTree<T, T> {
protected:
  class MinSegmentTreeNode : public SegmentTree<T, T>::SegmentTreeNode {
  public:
    virtual T fromVal(T val) { return val; }
    virtual void update() {
      if (this->left && this->right) {
        this->val = std::min(this->left->val, this->right->val);
      } else if (this->left) {
        this->val = this->left->val;
      } else { //  (this->right)
        this->val = this->right->val;
      }
    }
    virtual T combineVals(T leftVal, T rightVal, int leftIndex,
                          int rightIndex) {
      if (this->left && this->right) {
        return std::min(leftVal, rightVal);
      } else if (this->left) {
        return leftVal;
      } else { //  (this->right)
        return rightVal;
      }
    }
  };

  virtual typename SegmentTree<T, T>::SegmentTreeNode *
  createSegmentTreeNode(T val, int leftIndex, int rightIndex) {
    MinSegmentTreeNode *node = new MinSegmentTreeNode();
    node->init(val, leftIndex, rightIndex);
    return node;
  }

  virtual typename SegmentTree<T, T>::SegmentTreeNode *
  createSegmentTreeNode(typename SegmentTree<T, T>::SegmentTreeNode *left,
                        typename SegmentTree<T, T>::SegmentTreeNode *right) {
    MinSegmentTreeNode *node = new MinSegmentTreeNode();
    node->init(left, right);
    return node;
  }

public:
  MinSegmentTree(std::vector<T> &arr) : SegmentTree<T, T>(arr) {}
  static MinSegmentTree<T> createSegmentTree(std::vector<T> &arr) {
    MinSegmentTree<T> tree(arr);
    tree.init();
    return tree;
  }
};

std::ostream &operator<<(std::ostream &os, std::vector<int> &arr) {
  os << "[ ";
  for (int i = 0; i < arr.size(); i++) {
    os << arr[i] << " ";
  }
  return os << "]";
}

int main() {
  // compare brute force and min segment tree times
  // initialize random array
  int arraySize = 1e5;
  int maxVal = 1e5;
  int numOperations = 1e6;
  std::vector<int> arr(arraySize);
  std::vector<int> arr2(arraySize);
  for (int i = 0; i < arr.size(); i++) {
    int val = rand() % maxVal;
    arr[i] = val;
    arr2[i] = val;
  }
  // initialize queries and updates
  std::vector<std::vector<int>> operations;
  int numQueries = 0;
  for (int i = 0; i < numOperations; i++) {
    int operation = rand() % 2;
    if (operation == 0) {
      // query
      int left = rand() % arraySize;
      int right = left + (rand() % (arraySize - left));
      operations.push_back({operation, left, right});
      ++numQueries;
    } else {
      // update
      int index = rand() % arraySize;
      int val = rand() % maxVal;
      operations.push_back({operation, index, val});
    }
  }

  // allocate space to store results
  std::vector<int> bruteForceResults(numQueries);
  std::vector<int> segmentTreeResults(numQueries);

  // time the brute force
  int i = 0;
  auto start = std::chrono::steady_clock::now();
  for (std::vector<int> &operation : operations) {
    if (operation[0] == 0) {
      // query
      int left = operation[1];
      int right = operation[2];
      int min = *std::min_element(arr.begin() + left, arr.begin() + right + 1,
                                  std::less<int>());
      bruteForceResults[i++] = min;
    } else {
      // update
      int index = operation[1];
      int val = operation[2];
      arr[index] = val;
    }
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

  // time segment tree
  i = 0;
  start = std::chrono::steady_clock::now();
  MinSegmentTree<int> minTree = MinSegmentTree<int>::createSegmentTree(arr2);
  for (std::vector<int> &operation : operations) {
    if (operation[0] == 0) {
      // query
      int left = operation[1];
      int right = operation[2];
      segmentTreeResults[i++] = minTree.query(left, right);
    } else {
      // update
      int index = operation[1];
      int val = operation[2];
      minTree.update(index, val);
    }
  }
  end = std::chrono::steady_clock::now();
  elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

  return 0;
}
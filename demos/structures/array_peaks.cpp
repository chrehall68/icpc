#include "structures/segment_tree.hpp"
#include <iostream>

class Peaks : public SegmentTree<int, int> {
  class PeaksNode : public SegmentTree<int, int>::SegmentTreeNode {
  public:
    virtual int fromVal(int val) { return val; }
    virtual void update() {
      if (this->left && this->right) {
        this->val = this->left->val + this->right->val;
        if (hasPeak()) {
          this->val++;
        }
      } else if (this->left) {
        this->val = this->left->val;
      } else { //  (this->right)
        this->val = this->right->val;
      }
    }
    virtual int combineVals(int leftVal, int rightVal, int leftIndex,
                            int rightIndex) const {
      if (this->left && this->right) {
        return leftVal + rightVal +
               this->hasPeak(leftIndex, rightIndex);
      } else if (this->left) {
        return leftVal;
      } else { //  (this->right)
        return rightVal;
      }
    }

    bool hasPeak() const { return hasPeak(this->leftIndex, this->rightIndex); }
    bool hasPeak(int left, int right) const {
      if (this->left && this->right) {
        // peak is defined by an item that is greater than its neighbors
        // so check left's right and right's left
        const std::vector<int> &arr = this->tree->getArr();

        if (this->left->rightIndex - 1 >= left &&
            arr[this->left->rightIndex] > arr[this->left->rightIndex - 1] &&
            arr[this->left->rightIndex] > arr[this->right->leftIndex]) {
          int ret = left <= this->left->rightIndex - 1 &&
                    this->right->leftIndex <= right;
          return ret;
        }

        if (this->right->leftIndex + 1 <= right &&
            arr[this->right->leftIndex] > arr[this->right->leftIndex + 1] &&
            arr[this->right->leftIndex] > arr[this->left->rightIndex]) {
          int ret = left <= this->left->rightIndex &&
                    this->right->leftIndex + 1 <= right;
          return ret;
        }
      }

      return false;
    }
    int fromVal(int val) const { return 0; }
  };

protected:
  virtual SegmentTree<int, int>::SegmentTreeNode *
  createSegmentTreeNode(int val, int leftIndex, int rightIndex) {
    PeaksNode *node = new PeaksNode();
    node->init(this, val, leftIndex, rightIndex);
    return node;
  }

  virtual SegmentTree<int, int>::SegmentTreeNode *
  createSegmentTreeNode(SegmentTree<int, int>::SegmentTreeNode *left,
                        SegmentTree<int, int>::SegmentTreeNode *right) {
    SegmentTree<int, int>::SegmentTreeNode *node = new PeaksNode();
    node->init(this, left, right);
    return node;
  }

public:
  Peaks(std::vector<int> &arr) : SegmentTree<int, int>(arr) {}

  static Peaks create(std::vector<int> &arr) {
    Peaks p(arr);
    p.init();
    return p;
  }
};

using namespace std;
class Solution {
public:
  vector<int> countOfPeaks(vector<int> &nums, vector<vector<int>> &queries) {
    vector<int> result;
    Peaks peaks = Peaks::create(nums);
    for (vector<int> &query : queries) {
      if (query[0] == 1) {
        // query
        result.push_back(peaks.query(query[1], query[2]));
      } else {
        // update
        peaks.update(query[1], query[2]);
      }
    }
    return result;
  }
};
#include <iostream>
using namespace std;

ostream &operator<<(ostream &os, const vector<int> &v) {
  os << "[";
  for (int i = 0; i < v.size(); i++) {
    os << v[i];
    if (i != v.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

int main() {
  vector<int> nums = {4, 1, 4, 2, 1, 5};
  vector<vector<int>> queries = {{2, 2, 4}, {1, 0, 2}, {1, 0, 4}};

  Solution s;
  cout << s.countOfPeaks(nums, queries) << endl;
  return 0;
}
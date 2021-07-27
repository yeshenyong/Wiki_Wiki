class Solution {
public:
    unordered_map<int, TreeNode*> mmap;
    vector<int> vec;
    void findParent(TreeNode* root) {
        if (!root) return;
        if (root->left!=nullptr) {
            mmap[root->left->val] = root;
            findParent(root->left);
        }
        if (root->right!=nullptr) {
            mmap[root->right->val]  = root;
            findParent(root->right);
        }
    }
    void findAns(TreeNode* root, TreeNode* from, int level, const int k) {
        if (!root) return;
        if (level == k) {
            vec.push_back(root->val);
            return;
        }
        if (root->left!=from) {
            findAns(root->left, root, level+1, k);

        }
        if (root->right!=from) {
            findAns(root->right, root, level+1, k);

        }
        if (mmap[root->val] != from) {
            findAns(mmap[root->val], root, level+1, k);

        }
    }

    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        /*-----------method 1:dfs+回退----------*/
        findParent(root);
        findAns(target, nullptr, 0, k);
        return vec;
    }
};
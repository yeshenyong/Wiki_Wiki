class Solution {
public:

    bool dfs(TreeNode* root) {
        if (!root) return false;
        if (root->left) {
            if (dfs(root->left)) {
                delete root->left;
                root->left = nullptr;
            }
        }
        if (root->right) {
            if (dfs(root->right)) {
                delete root->right;
                root->right = nullptr;
            }
        }
        if (root->val == 0 && (!root->left) && (!root->right)) return true;
        return false;
    }

    bool containOne(TreeNode* root) {
        if (!root) return false;
        bool left = containOne(root->left);
        bool right = containOne(root->right);
        if (!left) {
            delete root->left;
            root->left = nullptr;
        }
        if (!right) {
            delete root->right;
            root->right = nullptr;
        }
        return root->val == 1 || left || right;
    }

    TreeNode* pruneTree(TreeNode* root) {
        /*---method 1:dfs---*/
        /*
        return dfs(root) ? nullptr : root;
        */
        /*---method 2:递归---*/
        return containOne(root) ? root : nullptr;
    }
};
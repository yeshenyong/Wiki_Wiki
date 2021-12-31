class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        /*------------method 1:普通二叉搜索树搜索(迭代、递归)---------------*/
        /*
        while(root) {
            if (root->val == val) break;
            else if (root->val > val) root=root->left;
            else root = root->right;
        }
        return root;
        */
        if (!root)return root;
        if (root->val == val) return root;
        else if (root->val > val)return searchBST(root->left, val);
        return searchBST(root->right, val);
    }
};
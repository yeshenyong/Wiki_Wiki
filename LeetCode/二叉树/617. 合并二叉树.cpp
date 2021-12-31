class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        /*-----------method 1:哪种遍历都可以递归进行merge--------------*/
        if (!root1 && root2) return root2;
        if (root1 && !root2) return root1;
        if (!root1 && !root2) return NULL;
        root1->left = mergeTrees(root1->left, root2->left);
        root1->right = mergeTrees(root1->right, root2->right);
        root1->val = root1->val + root2->val;
        return root1;
    }
};
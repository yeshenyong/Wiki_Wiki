class Solution {
public:
    TreeNode* trimBST(TreeNode* root, int low, int high) {
        /*-------------method 1:二叉搜索数性质--------------*/        
        /*
            情况1:root (high, 无穷)
                if(root->right)trimBST(root->right, low, high)
            情况2:root (无穷, low)
                if(root->left)trimBST(root->left, low , high)
            情况3:root (high, low)
                root->left = trimBST(root->left, low, high)
                root->right = trimBST(root->right, low, high)
        */
        if (!root) return NULL;
        if (root->val < low) {
            return trimBST(root->right, low, high);
        } else if (root->val > high) {
            return trimBST(root->left, low, high);
        } else {
            root->left = trimBST(root->left, low, high);
            root->right = trimBST(root->right, low, high);
        } 
        return root;
    }
};
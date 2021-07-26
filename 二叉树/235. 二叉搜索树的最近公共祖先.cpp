class Solution {
public:
    TreeNode* In_Order_Tree(TreeNode* root, TreeNode* p, TreeNode *q) {
        if (!root) return NULL;
        if (root->val > p->val && root->val > q->val) {
            TreeNode* left = In_Order_Tree(root->left, p, q);
            if (left)return left;
        } else if(root->val < p->val && root->val < q->val) {
            TreeNode* right = In_Order_Tree(root->right, p, q);
            if (right)return right;
        }
        return root;
    }
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        /*----------method 1:搜索二叉树性质（迭代/递归）----------*/
        /*
        while(root != NULL) {
            if (root->val > p->val && root->val > q->val) {
                root = root->left;
            } else if (root->val < p->val && root->val < q->val) {
                root = root->right;
            } else {
                return root;
            }
        }
        return NULL;
        */
        return In_Order_Tree(root, p, q);
    }
};
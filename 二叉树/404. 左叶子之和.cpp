class Solution {
public:
    int lsum = 0;
    void Pre_Order_Tree(TreeNode *root) {
        if (!root)return;
        Pre_Order_Tree(root->left);
        Pre_Order_Tree(root->right);
        if (root->left && (!root->left->left && !root->left->right)) {
            lsum += root->left->val;
        }
    }

    int sumOfLeftLeaves(TreeNode* root) {
        /*-----------method 1:各种方法遍历,只加左子节点的----------*/
        /*
        Pre_Order_Tree(root);
        return lsum;
        */
        /*-----------method 2:精简写法--------------------------*/
        if (!root) return 0;
        int leftValue = sumOfLeftLeaves(root->left);
        int rightValue = sumOfLeftLeaves(root->right);

        int value = 0;
        if (root->left && !root->left->right && !root->left->left) {
            value = root->left->val;
        }
        return leftValue + rightValue + value;
    }
};
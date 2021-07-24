class Solution {
public:
    bool In_Order_Tree(TreeNode* root, const int& targetSum, int curSum) {
        if (!root) return false;
        curSum += root->val;
        if (root->left==NULL && root->right==NULL && curSum == targetSum) return true;
        return In_Order_Tree(root->left, targetSum, curSum) || In_Order_Tree(root->right, targetSum, curSum);
    }

    bool hasPathSum(TreeNode* root, int targetSum) {
        /*-----------method 1:前序遍历+记录路径和-----------*/
        if (!root)return false;
        int curSum = 0;
        return In_Order_Tree(root, targetSum, curSum);
    }
};
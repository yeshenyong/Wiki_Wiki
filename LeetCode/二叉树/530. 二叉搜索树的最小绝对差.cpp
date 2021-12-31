class Solution {
public:
    int minn = INT_MAX;
    TreeNode* pre = nullptr;
    void Mid_Order_Tree(TreeNode* root) {
        if (!root) return;
        Mid_Order_Tree(root->left);
        if (pre != nullptr) {
            minn = min(minn, root->val-pre->val);
        }
        pre = root;
        Mid_Order_Tree(root->right);
    }

    int getMinimumDifference(TreeNode* root) {
        /*-----------method 1:中序遍历+最小值处理----------*/
        Mid_Order_Tree(root);
        return minn;
    }
};
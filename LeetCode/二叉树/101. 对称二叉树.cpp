class Solution {
public:
    bool isBothSame(TreeNode *left, TreeNode *right) {
        if (left == nullptr　&& right == nullptr)return true;
        if ((left == nullptr && right != nullptr) || (left != nullptr && right == nullptr))return false;
        if (left->val != right->val) return false;
        return isBothSame(left->left, right->right) &&　isBothSame(left->right, right->left);
    }

    bool isSymmetric(TreeNode* root) {
        /*----------method 1:递归+左右前序遍历-------------*/
        return isBothSame(root->left, root->right);

        /*----------method 2:迭代-------------------------*/
    }
};
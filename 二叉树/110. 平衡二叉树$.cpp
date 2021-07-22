class Solution {
public:
    int getDepth(TreeNode *root) {
        if (root == NULL) return 0;
        int leftDepth = getDepth(root->left);
        if (leftDepth == -1) return -1;
        int rightDepth = getDepth(root->right);
        if (rightDepth == -1) return -1;
        return abs(leftDepth-rightDepth)>1?-1:1+max(leftDepth,rightDepth);
    }

    bool isBalanced(TreeNode* root) {
        /*---------method 1:递归二叉树遍历---------*/
        return getDepth(root)==-1?false:true;
    }
};
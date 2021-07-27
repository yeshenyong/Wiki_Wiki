class Solution {
public:
    int countNodes(TreeNode* root) {
        /*-----------method 1:bfs/dfs/前序中序后序遍历(没啥意义)--------------*/
        /*-----------method 2:根据完全二叉树和满二叉树的性质------------------*/
        if (!root) return 0;
        int LeftHigh = 0, RightHigh = 0;
        TreeNode* rLeft = root;
        TreeNode* rRight= root;
        while (rLeft->left) {
            rLeft = rLeft->left;
            LeftHigh++;
        }
        while (rRight->right) {
            rRight = rRight->right;
            RightHigh++;
        }
        if (LeftHigh == RightHigh) return (1<<(1+RightHigh)) - 1;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
};
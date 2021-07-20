class Solution {
public:

    void InOrderTree(TreeNode* root, int& maxnLevel, int level) {
        if (root == nullptr)return;
        maxnLevel = maxnLevel < level?level:maxnLevel;
        InOrderTree(root->left, maxnLevel, level+1);
        InOrderTree(root->right, maxnLevel, level+1);
    }

    int maxDepth(TreeNode* root) {
        /*-------------method 1:dfs/bfs--------------*/
        /*-------------method 2:前序遍历记录层数,其实道理都一样----------*/
        int maxnLevel = 0;
        InOrderTree(root, maxnLevel, 1);
        return maxnLevel;
    }
};
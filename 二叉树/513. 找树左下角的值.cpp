class Solution {
public:
    int maxLevel = 0;
    int left;
    void Pre_Order_Tree(TreeNode *root, int level) {
        if (!root) return;
        if (!root->left && !root->right) {
            if (level > maxLevel) {
                maxLevel = level;
                left = root->val;
            }
        }
        Pre_Order_Tree(root->left, level+1);
        Pre_Order_Tree(root->right, level+1);
    }

    int findBottomLeftValue(TreeNode* root) {
        /*----------method 1:bfs + 最后一层记录最左边----------*/
        
        /*----------method 2:递归+记录层数+前序遍历第一个值-----*/
        if (!root) return 0;
        Pre_Order_Tree(root, 1);
        
        return left;
    }
};
class Solution {
public:
    int findMax(const vector<int>& nums, int left, int right) {
        int index = 0;
        int maxn = -1;
        for (int i=left;i <= right;i++) {
            if (nums[i] > maxn) {
                maxn = nums[i];
                index = i;
            }
        }
        return index;
    }
    TreeNode* getTreeNode(const vector<int>& nums, int left, int right) {
        if (left > right)return NULL;
        int index = findMax(nums, left, right);
        TreeNode* root = new TreeNode(nums[index]);
        root->left = getTreeNode(nums, left, index-1);
        root->right = getTreeNode(nums, index+1, right);
        return root;
    }
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        /*--------------method 1:递归分块处理---------------*/
        return getTreeNode(nums, 0, nums.size()-1);
    }
};
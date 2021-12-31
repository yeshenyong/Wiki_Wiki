class Solution {
public:
    TreeNode* ArrayToBST(vector<int>&nums, int left, int right) {
        if (left > right)return NULL;
        int mid = left + (right-left)/2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = ArrayToBST(nums, left, mid - 1);
        root->right = ArrayToBST(nums, mid + 1, right);
        return root;
    }

    TreeNode* sortedArrayToBST(vector<int>& nums) {
        /*-----------method 1:递归回溯------------*/
        return ArrayToBST(nums, 0, nums.size()-1);
    }
};
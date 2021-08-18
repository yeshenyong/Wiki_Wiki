class Solution {
public:
    vector<vector<int>> result;
    void dfs(TreeNode* root, const int targetSum, int sum, vector<int>& cnt) {
        if (root->left == nullptr && root->right == nullptr) {
            if (sum == targetSum) {
                result.push_back(cnt);
            }
            return;
        }
        if (root->left) {
            cnt.push_back(root->left->val);
            dfs(root->left, targetSum, sum+root->left->val, cnt);
            cnt.pop_back();
        }
        if (root->right) {
            cnt.push_back(root->right->val);
            dfs(root->right, targetSum, sum+root->right->val, cnt);
            cnt.pop_back();
        }
    }
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        /*---method 1:dfs+路径记录---*/
        if (!root) return result;
        vector<int> cnt(1, root->val);
        dfs(root, targetSum, root->val, cnt);
        return result;
    }
};
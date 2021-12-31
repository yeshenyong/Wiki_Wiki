class Solution {
public:
    int Sum = 0;
    void dfs(TreeNode* root,const int targetSum, vector<int>& cnt, int totalSum) {
        if (!root) return;
        if (root->val == targetSum) Sum++;
        if (targetSum == totalSum) Sum++;
        int tmpTotal = totalSum;
        int index = cnt.size()-2;
        for (int i=0;i < index;i++) {
            // printf("i = %d, cnt.size() = %d, cnt.size()-2=%d\n", i, cnt.size(), cnt.size()-2);
            tmpTotal -= cnt[i];
            if (tmpTotal == targetSum) Sum++;
        }
        if (root->left) {
            cnt.emplace_back(root->left->val);
            dfs(root->left, targetSum, cnt, totalSum + root->left->val);
            cnt.pop_back();
        }
        if (root->right) {
            cnt.emplace_back(root->right->val);
            dfs(root->right, targetSum, cnt, totalSum + root->right->val);
            cnt.pop_back();
        }
    }

    int pathSum(TreeNode* root, int targetSum) {
        /*---method 1:dfs*/
        if (!root) return 0;
        vector<int> cnt;
        cnt.emplace_back(root->val);
        if (root->val == targetSum) Sum--;
        dfs(root, targetSum, cnt, root->val);
        return Sum;
    }
};
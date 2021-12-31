class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        /*----------method 1:bfs + 逐层比较---------*/
        vector<int> maxnn;
        queue<TreeNode *> MyQueue;
        if (!root) return maxnn;
        MyQueue.push(root);
        while(!MyQueue.empty()) {
            int size = MyQueue.size();
            TreeNode* cur;
            int maxn = pow(-2, 31);
            for (int i=0;i < size;i++) {
                cur = MyQueue.front();
                MyQueue.pop();
                maxn = maxn > cur->val ? maxn : cur->val;
                if (cur->left)MyQueue.push(cur->left);
                if (cur->right)MyQueue.push(cur->right);
            }
            cur = nullptr;
            maxnn.push_back(maxn);
        }
        return maxnn;
    }
};
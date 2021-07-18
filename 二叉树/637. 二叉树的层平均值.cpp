class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        /*----------method 1:bfs + 逐层累除---------*/
        vector<double> average;
        queue<TreeNode *> MyQueue;
        if (!root) return average;
        MyQueue.push(root);
        while(!MyQueue.empty()) {
            int size = MyQueue.size();
            TreeNode* cur;
            double sum = 0;
            for (int i=0;i < size;i++) {
                cur = MyQueue.front();
                MyQueue.pop();
                sum += cur->val;
                if (cur->left)MyQueue.push(cur->left);
                if (cur->right)MyQueue.push(cur->right);
            }
            average.push_back(sum/size);
        }
        return average;
    }
};
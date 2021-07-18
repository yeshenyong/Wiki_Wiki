class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        /*----------method 1:bfs + 仅保存该层最后一个----------*/
        vector<int> tree;
        queue<TreeNode* > MyQueue;
        if (!root) return tree;
        MyQueue.push(root);
        while (!MyQueue.empty()) {
            int size = MyQueue.size();
            TreeNode *cur;
            for (int i=0;i < size;i++){
                cur = MyQueue.front();
                MyQueue.pop();
                if (i == size-1)tree.push_back(cur->val);
                if (cur->left) MyQueue.push(cur->left);
                if (cur->right) MyQueue.push(cur->right);
            }
        }
        return tree;
    }
};
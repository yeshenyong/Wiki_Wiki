class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        /* method 1:bfs + 層數模擬 */
        vector<vector<int>> mmap;
        if (!root) return mmap;
        queue<TreeNode*> myqueue;
        myqueue.emplace(root);
        int level = 1;
        while(!myqueue.empty()) {
            level++;
            int size = myqueue.size();
            vector<int> vec(size);
            vector<TreeNode*> next;
            for (int i=0;i < size;i++) {
                TreeNode* head = myqueue.front();
                myqueue.pop();
                if (level % 2 == 0) {
                    if (head->left) next.emplace_back(head->left);
                    if (head->right) next.emplace_back(head->right);
                } else {
                    if (head->right) next.emplace_back(head->right);
                    if (head->left) next.emplace_back(head->left);
                }
                vec[i]=head->val;
            }
            for (int i=next.size()-1;i >= 0;i--) {
                myqueue.emplace(next[i]);
            }
            mmap.emplace_back(vec);
        }
        return mmap;
    }
};
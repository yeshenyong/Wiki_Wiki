class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        /*------------method 1:bfs + reverse----------*/
        vector<vector<int>> tree;
        if (!root)return tree;
        queue<TreeNode *> MyQue;
        MyQue.push(root);
        while(!MyQue.empty()) {
            int size = MyQue.size();
            vector<int> tmp;
            TreeNode* cur;
            for (int i=0;i < size;i++){
                cur = MyQue.front();
                MyQue.pop();
                tmp.push_back(cur->val);
                if (cur->left) MyQue.push(cur->left);
                if (cur->right) MyQue.push(cur->right);
            }
            tree.push_back(tmp);
        }
        reverse(tree.begin(), tree.end());
        return tree;
    }
};
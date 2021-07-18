/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        /*-------method 1:bfs 迭代---------*/
        vector<vector<int>> level;
        if (root == nullptr)return level;
        queue<TreeNode* > myqueue;
        myqueue.push(root);
        while(!myqueue.empty()){
            int size = myqueue.size();
            vector<int> cur;
            for(int i=0;i < size;i++){
                TreeNode* tmp = myqueue.front();
                cur.push_back(tmp->val);
                if(tmp->left)myqueue.push(tmp->left);
                if(tmp->right)myqueue.push(tmp->right);
                myqueue.pop();
            }
            level.push_back(cur);
        }
        return level;
    }   
};
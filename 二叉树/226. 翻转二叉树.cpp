class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        /*--------method 1:前序遍历 + 递归-----------*/
        /*
        if (!root) return root;
        swap(root->left, root->right);
        invertTree(root->left);
        invertTree(root->right);
        return root;
        */
        /*--------method 2:层序遍历+迭代-----------*/
        if (!root)return root;
        queue<TreeNode*> MyQueue;
        MyQueue.push(root);
        while(!MyQueue.empty()) {
            TreeNode* cur = MyQueue.front();
            MyQueue.pop();
            swap(cur->left, cur->right);
            if (cur->left) MyQueue.push(cur->left);
            if (cur->right) MyQueue.push(cur->right);
        }
        return root;
    }
};
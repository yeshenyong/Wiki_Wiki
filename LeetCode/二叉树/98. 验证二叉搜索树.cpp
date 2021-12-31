class Solution {
public:
    TreeNode* pre = nullptr;
    bool isValidBST(TreeNode* root) {
        /*----------method 1:迭代/递归中序遍历对比（中序遍历结果肯定有序）------------*/
        /*
        if (!root) return true;
        auto left = isValidBST(root->left);
        if (!pre || pre->val < root->val)pre=root;
        else return false;
        auto right = isValidBST(root->right);
        return left && right;
        */
        stack<TreeNode*> MyQueue;
        TreeNode* cur = root;
        TreeNode* pre = nullptr;
        while(cur || !MyQueue.empty()) {
            while(cur) {
                MyQueue.push(cur);
                cur = cur->left;
            }
            cur = MyQueue.top();
            MyQueue.pop();
            if (!pre || pre->val < cur->val)pre=cur;
            else return false;
            cur = cur->right;
        }
        return true;
    }
};
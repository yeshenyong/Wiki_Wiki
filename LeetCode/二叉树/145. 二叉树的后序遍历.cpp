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

    void Post_Order(TreeNode* root, vector<int>& tree) {
        if (!root)return;
        Post_Order(root->left, tree);
        Post_Order(root->right, tree);
        tree.push_back(root->val);
    }

    vector<int> postorderTraversal(TreeNode* root) {
        /*---------method 1:递归---------*/
        /*
        vector<int> tree;
        Post_Order(root, tree);
        return tree;
        */
        /*---------method 2:迭代---------*/

        vector<int> tree;
        if (!root)return tree;
        stack<TreeNode *> st;
        st.push(root);
        while (!st.empty()) {
            TreeNode* cur = st.top();
            st.pop();
            tree.push_back(cur->val);
            if (cur->left)st.push(cur->left);
            if (cur->right)st.push(cur->right);
        }
        reverse(tree.begin(), tree.end());
        return tree;
    }
};
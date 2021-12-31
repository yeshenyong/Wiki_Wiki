class Solution {
public:

    void Pre_Order(TreeNode* root, vector<int>& tree) {
        if (!root)return;
        tree.push_back(root->val);
        Pre_Order(root->left, tree);
        Pre_Order(root->right, tree);
    }

    vector<int> preorderTraversal(TreeNode* root) {
        /* 根左右 */
        /*----------method 1:递归-----------*/
        /*
        vector<int> tree;
        Pre_Order(root, tree);            
        return tree;
        */
        /*----------method 2:迭代*----------*/
        stack<TreeNode *> st;
        vector<int> tree;
        if (!root) return tree;
        st.push(root);
        while(!st.empty()) {
            TreeNode* cur = st.top();
            st.pop();
            tree.push_back(cur->val);
            /*left*/
            if (cur->right != nullptr){
                st.push(cur->right);
            }
            if (cur->left != nullptr){
                st.push(cur->left);
            }
        }
        return tree;
    }
};


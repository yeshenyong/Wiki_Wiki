class Solution {
public:
    void DfsBinaryTree(TreeNode *root, vector<int>& path, vector<string>& url) {
        if (root == nullptr)return;
        if (!root->right && !root->left) {
            string cnt;
            int i;
            for (i=0;i < path.size()-1;i++) {
                cnt += to_string(path[i]);
                cnt += "->";
            }
            cnt += to_string(path[i]);
            url.push_back(cnt);
            return;
        }
        if (root->left) {
            path.push_back(root->left->val);
            DfsBinaryTree(root->left, path, url);
            path.pop_back();
        }
        if (root->right) {
            path.push_back(root->right->val);
            DfsBinaryTree(root->right, path, url);
            path.pop_back();
        }
    }
    vector<string> binaryTreePaths(TreeNode* root) {
        /*-----------method 1:dfs------------*/
        vector<string> url;
        if (!root) return url;
        vector<int> path(1, root->val);
        DfsBinaryTree(root, path, url);
        return url;
    }
};
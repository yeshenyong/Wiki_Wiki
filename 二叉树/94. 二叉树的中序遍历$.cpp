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
    /*
    vector<int> Vec;
    void binary_mid(TreeNode *root) {
        if (root == nullptr)return;
        binary_mid(root->left);
        Vec.push_back(root->val);
        binary_mid(root->right);
    }
    */


    vector<int> inorderTraversal(TreeNode* root) {
        /*----------method 1:递归-------------*/
        /*
        vector<int> Vec;
        function<void(TreeNode*)> binaryMid = [&](TreeNode* r)->void{
            if (r == nullptr)return;
            binaryMid(r->left);
            Vec.push_back(r->val);
            binaryMid(r->right);
        };
        */
        //binaryMid(root);
        // binary_mid(root);
        // return Vec;

        /*----------method 2:迭代-------------*/
        if (root == nullptr)return {};
        vector<int> Vec;
        stack<TreeNode*> mystack;
        TreeNode* cur = root;
        while(cur || !mystack.empty()){
            while(cur != nullptr){
                mystack.push(cur);
                cur = cur->left;
            }
            TreeNode* node = mystack.top();
            Vec.push_back(node->val);
            mystack.pop();
            cur = node->right;
        }
        return Vec;
    }
};
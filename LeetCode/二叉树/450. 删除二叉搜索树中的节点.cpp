class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        /*----------method 1:右子树/左子树上来，然后它们对应的子树调整位置-----------*/
        if (!root) return NULL;
        if (root->val == key) {
            if (!root->left && !root->right) {
                delete root;
                return NULL;
            } else if (!root->left) {
                TreeNode* cur = root->right;
                delete root;
                return cur;
            } else if (!root->right) {
                TreeNode* cur = root->left;
                delete root;
                return cur;
            } else {
                TreeNode* cur = root->right;
                while(cur->left) {
                    cur = cur->left;
                }
                cur->left = root->left;
                TreeNode* tmp = root;
                root = root->right;;
                delete tmp;
                return root;
            }
        }
        if (root->val > key)root->left = deleteNode(root->left, key);
        else root->right = deleteNode(root->right, key);
        return root;
    }
};
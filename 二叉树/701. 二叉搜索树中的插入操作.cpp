class Solution {
public:
    void In_Order_Tree(TreeNode* root, TreeNode* NewNode) {
        if (root->val > NewNode->val) {
            if (root->left)In_Order_Tree(root->left, NewNode);
            else root->left = NewNode;
        } else {
            if (root->right)In_Order_Tree(root->right, NewNode);
            else root->right = NewNode;
        }
    }
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        /*----------method 1:目标是插入尾节点(迭代、递归)----------*/
        /*
        TreeNode* cur = root;
        TreeNode* NewNode = new TreeNode(val);
        if (!root) return NewNode;
        while (cur != NULL) {
            if (val < cur->val) {
                if (cur->left) {
                    cur = cur->left;
                } else {
                    cur->left = NewNode;
                    break;
                }
            } else if(val > cur->val) {
                if (cur->right) {
                    cur = cur->right;
                } else {
                    cur->right = NewNode;
                    break;
                }
            }

        }
        cur = NULL;
        return root;
        */ 
        TreeNode* NewNode = new TreeNode(val);
        if (!root)return NewNode;
        In_Order_Tree(root, NewNode);
        return root;
    }
};
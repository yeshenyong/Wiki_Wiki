class Solution {
public:
    vector<int> mutiTree;
    int maxnKind = 0;
    TreeNode *pre = nullptr;
    void Mid_Order_Tree(TreeNode* root, int& curKind) {
        if (!root) return;
        Mid_Order_Tree(root->left, curKind);
        if (pre == nullptr) {
            curKind = 1;
        }
        /* 结算之前嘚 */
        else if (pre->val != root->val) {
            curKind = 1;
        }
        else curKind++;
        pre = root;
        if (curKind > maxnKind) {
            maxnKind = curKind;
            mutiTree.clear();
            mutiTree.push_back(root->val);
        }
        else if (curKind == maxnKind) {
            mutiTree.push_back(root->val);
        }
        Mid_Order_Tree(root->right, curKind);
    }
    vector<int> findMode(TreeNode* root) {
        /*--------method 1:中序遍历 + 记录众数就可--------*/
        int curKind = 0;
        Mid_Order_Tree(root, curKind);
        return mutiTree;
    }
};
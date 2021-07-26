class Solution {
public
    void convert_Mid(TreeNode root, int& val) {
        if (!root) return;
        convert_Mid(root-right, val);
        val += root-val;
        root-val = val;
        convert_Mid(root-left, val);
    }

    TreeNode convertBST(TreeNode root) {
        -----------method 1有左中序遍历,就有右中序遍历----------
        int val = 0;
        convert_Mid(root, val);
        return root;
    }
};
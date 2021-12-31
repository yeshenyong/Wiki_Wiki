class Solution {
public:

    TreeNode* Build_Train_Tree(vector<int>& inorder, vector<int>& postorder, int midleft, 
            int midright, int backleft, int backright) 
    {
        if (midleft > midright || backleft > backright)return nullptr;
        int val = postorder[backright];
        TreeNode* root = new TreeNode(val);
        int index;
        for (index=midleft;index <= midright;index++) {
            if (val == inorder[index]) break;
        }
        int leftSize = index - midleft;
        root->left = Build_Train_Tree(inorder, postorder, midleft, index-1, backleft, backleft+leftSize-1);
        root->right = Build_Train_Tree(inorder, postorder, index+1, midright, backleft+leftSize, backright-1);
        return root;
    }


    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        /*----------method 1:切分数组，分左中序数组,右中序数组,左后序数组,右后序数组-------------*/
        if (inorder.size() == 0 || postorder.size()==0)return nullptr;
        return Build_Train_Tree(inorder, postorder, 0, inorder.size()-1, 0, postorder.size()-1);
    }
};
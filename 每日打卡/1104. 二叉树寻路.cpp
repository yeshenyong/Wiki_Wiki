class Solution {
public:
    vector<int> pathInZigZagTree(int label) {
        /*----------method 1:每次逆序对比，每次层数对比(总结:这题有病,做了我一个多小时)---------*/
        vector<int> vec;
        int level = 1;
        int tmplabel = label;
        while(tmplabel>>=1)level++;
        int push_num = label;
        if (level%2) {
            while (level--) {
                vec.push_back(push_num);
                if (level%2 == 0) {
                    int WindowSize = (pow(2, level) + pow(2, level+1)-1)/2;
                    if (label > WindowSize) {
                        push_num = label/2 - ((label-WindowSize+1)/2*2 - 1);
                    } else {
                        push_num = label/2 + ((WindowSize-label)/2*2 + 1);
                    }
                } else {
                    push_num = label/2;
                } 
                label /= 2;
            } 
        } else {
            while (level--) {
                if (level == 0)vec.push_back(label);
                else vec.push_back(push_num);
                if (level%2 == 1) {
                    int WindowSize = (pow(2, level) + pow(2, level+1)-1)/2;
                    if (label > WindowSize) {
                        push_num = label/2 - ((label-WindowSize+1)/2*2 - 1);
                    } else {
                        push_num = label/2 + ((WindowSize-label)/2*2 + 1);
                    }
                } else {
                    push_num = label/2;
                } 
                label /= 2;
                // printf("label = %d, push_num = %d\n", label, push_num);
            } 
        }
        reverse(vec.begin(), vec.end());
        return vec;
    }
};
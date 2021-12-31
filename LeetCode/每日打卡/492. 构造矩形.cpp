class Solution {
public:
    vector<int> constructRectangle(int area) {
        /*---method 1:sqrt 求素数---*/
        int L = 0,  W = 0;

        for (int i=sqrt(area);i >= 1;i--) {
            if (area % i == 0) {
                W = i;
                L = area / i;
                break;
            }
        }

        return vector<int> ({L, W});
    }
};
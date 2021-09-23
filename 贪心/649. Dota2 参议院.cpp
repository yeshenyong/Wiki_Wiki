class Solution {
public:
    string predictPartyVictory(string senate) {
        /*---method 1:贪心鬼 + 标记循环---*/
        bool R=true, D=true;
        int flag=0;
        while(R && D) {
            R=false;
            D=false;
            for (int i=0;i < senate.size();i++) {
                if (senate[i] == 'R') {
                    if (flag < 0) senate[i]='N';
                    else R=true;
                    flag++;
                } else if (senate[i] == 'D') {
                    if (flag > 0) senate[i]='N';
                    else D=true;
                    flag--;
                }
            }
        }
        return R==true?"Radiant":"Dire";
    }
};
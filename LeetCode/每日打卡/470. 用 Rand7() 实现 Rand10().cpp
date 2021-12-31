class Solution {
public:
    int rand10() {
        /*---method 1:概率统计+放大---*/
        unordered_map<int, int> mmap;
        mmap.insert(make_pair(2, 1));
        mmap.insert(make_pair(3, 2));
        mmap.insert(make_pair(5, 3));
        mmap.insert(make_pair(7, 4));
        mmap.insert(make_pair(8, 5));
        mmap.insert(make_pair(10, 6));
        mmap.insert(make_pair(14, 7));
        mmap.insert(make_pair(15, 8));
        mmap.insert(make_pair(18, 9));
        mmap.insert(make_pair(20, 10));
        while (true) {
            int rand1 = rand7();
            int rand2 = rand7();
            if (mmap.find(rand1 * rand2) != mmap.end()) {
                return mmap[rand2 * rand1];
            }
        }
        return -1;
    }
};
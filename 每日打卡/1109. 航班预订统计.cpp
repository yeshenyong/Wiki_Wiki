class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        /*---method 1:差分---*/
        vector<int> answer(n);
        int length = bookings.size();
        for (int i=0;i < length;i++) {
            answer[bookings[i][0]-1] += bookings[i][2];
            if (bookings[i][1] < n) {
                answer[bookings[i][1]] -= bookings[i][2];
            }
        }
        for (int i=1;i < n;i++) {
            answer[i] += answer[i-1];
        }
        return answer;
    }
};
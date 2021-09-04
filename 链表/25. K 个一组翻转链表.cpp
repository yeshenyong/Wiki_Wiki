class Solution {
public

    ListNode reverseList(ListNode head, ListNode tail) {
        ListNode prev = tail-next;
        ListNode curr = head;
        while (prev != tail) {
            ListNode next = curr-next;
            curr-next = prev;
            prev = curr;
            curr = next;
        }
        return {tail, head};
    }
    ListNode reverseKGroup(ListNode head, int k) {
        ----method 1链表模拟（重点）----
        ListNode hair = new ListNode(0);
        hair-next = head;
        ListNode pre = hair;
        while(head) {
            ListNode tail = pre;
            for (int i=0;i  k;i++) {
                tail = tail-next;
                if (!tail) {
                    return hair-next;
                }
            }
            ListNode nex = tail-next;
            pairListNode, ListNode result = reverseList(head, tail);
            head = result.first;
            tail = result.second;

            pre-next = head;
            tail-next = nex;
            pre = tail;
            head = tail-next;
        }
        return hair-next;
    }
};
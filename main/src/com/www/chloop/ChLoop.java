package com.www.chloop;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReentrantLock;

public class ChLoop {
    public static final int DEFAULT_NODE_COUNT = 128;
    public static final long FLAG = Integer.MAX_VALUE - 1;
    private int nodeSize = DEFAULT_NODE_COUNT;
    private ReentrantLock lock = new ReentrantLock(false);
    private Map<Integer, MappingNode> nodeMap = new HashMap<>();
    private int[] nodes;

    public int getNextId(String key) {
        long hash = HashUtil.hash(key);
        int targetId = (int) (hash % FLAG);
        int ans = -1;
        try {
            lock.lock();
            ans = binarySearchId(targetId);
            if (ans == -1) {
                ans = binarySearchId(1);
            }
        } catch (Exception e) {

        } finally {
            lock.unlock();
        }
        return ans;
    }

    public void setNodeSize(int nodeSize) {
        this.nodeSize = nodeSize;
    }

    public void addNode(MappingNode mn) {
        int[] ids = new int[nodeSize];
        for (int i = 0; i < nodeSize; i++) {
            long hash = Math.abs(HashUtil.hash(mn.key + "-" + i));
            int targetId = (int) (hash % FLAG);
            nodeMap.put(targetId, mn);
            ids[i] = targetId;
        }
        int[] copyList = new int[nodeSize + (nodes == null ? 0 : nodes.length)];
        if (nodes != null) {
            int run1 = 0, run2 = 0, run3 = 0;
            while (run1 < ids.length || run2 < nodes.length) {
                if (run1 == ids.length) {
                    copyList[run3] = ids[run2];
                    run2++;
                } else if (run2 == ids.length) {
                    copyList[run3] = ids[run1];
                    run1++;
                } else if (ids[run1] <= nodes[run2]) {
                    copyList[run3] = ids[run1];
                    run1++;
                } else if (ids[run1] > nodes[run2]) {
                    copyList[run3] = ids[run2];
                    run2++;
                }
                run3++;
            }
        }
        try {
            lock.lock();
            nodes = copyList;
        } catch (Exception e) {

        } finally {
            lock.unlock();
        }
    }

    private int binarySearchId(int targetId) {
        int left = 0, right = nodes.length - 1;
        int mid = 0;
        int ans = Integer.MAX_VALUE;

        while (left <= right) {
            mid = ((right - left) >> 1) + left;
            int temp = nodes[mid];
            if (temp < targetId) {
                left = mid + 1;
            } else if (temp > targetId) {
                ans = Math.min(ans, temp);
                right = mid - 1;
            } else {
                ans = temp;
                break;
            }
        }
        if (ans == Integer.MAX_VALUE) {
            return -1;
        }
        return ans;
    }

}

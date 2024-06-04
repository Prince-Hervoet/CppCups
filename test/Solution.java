package test;

import java.util.*;

public class Solution {

    boolean isNumber(char c) {
        return (c >= '0' && c <= '9');
    }

    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 比较版本号
     * 
     * @param version1 string字符串
     * @param version2 string字符串
     * @return int整型
     */
    public int compare(String version1, String version2) {
        int a = 0, b = 0;
        int sum1 = 0, sum2 = 0;
        while (a < version1.length() || b < version2.length()) {
            if (a < version1.length() && b < version2.length()) {
                char c1 = version1.charAt(a);
                char c2 = version2.charAt(b);
                if (isNumber(c1)) {
                    a++;
                    sum1 = sum1 * 10 + (c1 - '0');
                }
                if (isNumber(c2)) {
                    b++;
                    sum2 = sum2 * 10 + (c2 - '0');
                }
                if (c1 == '.' && c2 == '.') {
                    if (sum1 > sum2)
                        return 1;
                    else if (sum1 < sum2)
                        return -1;
                    a++;
                    b++;
                    sum1 = 0;
                    sum2 = 0;
                }
            } else if (a < version1.length()) {
                char c1 = version1.charAt(a);
                if (isNumber(c1)) {
                    a++;
                    sum1 = sum1 * 10 + (c1 - '0');
                }
                if (c1 == '.') {
                    if (sum1 > sum2)
                        return 1;
                    else if (sum1 < sum2)
                        return -1;
                    sum1 = 0;
                    sum2 = 0;
                    a++;
                }
            } else if (b < version2.length()) {
                char c2 = version2.charAt(b);
                if (isNumber(c2)) {
                    b++;
                    sum2 = sum2 * 10 + (c2 - '0');
                }
                if (c2 == '.') {
                    if (sum1 > sum2)
                        return 1;
                    else if (sum1 < sum2)
                        return -1;
                    sum1 = 0;
                    sum2 = 0;
                    b++;
                }
            }
        }
        if (sum1 > sum2)
            return 1;
        else if (sum1 < sum2)
            return -1;
        else
            return 0;
    }

    public int findTargetSumWays(int[] nums, int target) {
        return findTargetSumWaysAction(nums, target, target);
    }

    public int findTargetSumWaysAction(int[] nums, int target, int index) {
        if (index == nums.length) {
            if (target == 0)
                return 1;
            return 0;
        }
        int a = findTargetSumWaysAction(nums, target - nums[index], index + 1);
        int b = findTargetSumWaysAction(nums, target + nums[index], index + 1);
        return a + b;
    }

    public boolean canPartition(int[] nums) {
        int sum = 0;
        for (int i = 0; i < nums.length; ++i)
            sum += nums[i];
        if ((sum & 1) != 0)
            return false;
        int target = sum / 2;
        return canPartitionAction(nums, 0, 0, target);
    }

    public boolean canPartitionAction(int[] nums, int index, int sum, int target) {
        if (sum == target)
            return true;
        if (index == nums.length)
            return false;
        if (canPartitionAction(nums, index + 1, sum + nums[index], target))
            return true;
        if (canPartitionAction(nums, index + 1, sum, target))
            return true;
        return false;
    }
}
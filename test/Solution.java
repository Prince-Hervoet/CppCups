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
}
package com.www.chloop;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class HashUtil {

    public static Long hash(byte[] key) {

        ByteBuffer buf = ByteBuffer.wrap(key);
        int seed = 0x1234ABCD;

        ByteOrder byteOrder = buf.order();
        buf.order(ByteOrder.LITTLE_ENDIAN);

        long m = 0xc6a4a7935bd1e995L;
        int r = 47;

        long h = seed ^ (buf.remaining() * m);

        long k;
        while (buf.remaining() >= 8) {
            k = buf.getLong();

            k *= m;
            k ^= k >>> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        if (buf.remaining() > 0) {
            ByteBuffer finish = ByteBuffer.allocate(8).order(
                    ByteOrder.LITTLE_ENDIAN);
            finish.put(buf).rewind();
            h ^= finish.getLong();
            h *= m;
        }

        h ^= h >>> r;
        h *= m;
        h ^= h >>> r;

        buf.order(byteOrder);
        return h;
    }

    public static Long hash(String key) {
        return hash(key.getBytes());
    }

    public static Long readUnsignedLong(long value) {
        if (value >= 0) {
            return value;
        }
        return value & 0x7fffffffffffffffL;
    }

    public static Long hashUnsigned(String key) {
        return readUnsignedLong(hash(key));
    }

    public static Long hashUnsigned(byte[] key) {
        return readUnsignedLong(hash(key));
    }

}
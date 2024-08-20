#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#include <unistd.h>

void print_message() {
    printf("This is a message printed every 0.5 seconds.\n");
}

int main() {
    struct timeval start, now;
    long elapsed_time;

    // 获取开始时间
    gettimeofday(&start, NULL);

    while (1) {
        // 获取当前时间
        gettimeofday(&now, NULL);

        // 计算从开始到现在经过的时间（微秒）
        elapsed_time = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);

        // 如果经过的时间大于等于500000微秒（0.5秒）
        if (elapsed_time >= 500000) {
            print_message();

            // 重置开始时间
            start = now;
        }

        // 短暂休眠以减少CPU占用
        
    }

    return 0;
}

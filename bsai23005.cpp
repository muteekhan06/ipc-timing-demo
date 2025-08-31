#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

using namespace std;

int main() {
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);

    pid_t pid = fork();

    int n = 1000000;
    timeval start, end;

    if (pid == 0) {
        int num;
        for (int i = 0; i < n; i++) {
            read(p1[0], &num, sizeof(int));
            write(p2[1], &num, sizeof(int));
        }
    } else {
        int num;
        gettimeofday(&start, NULL);

        for (int i = 0; i < n; i++) {
            num = i;
            write(p1[1], &num, sizeof(int));
            read(p2[0], &num, sizeof(int));
        }

        gettimeofday(&end, NULL);

        double total_time = (end.tv_sec - start.tv_sec) +
                            (end.tv_usec - start.tv_usec) / 1000000.0;

        cout << "Total time: " << total_time << " seconds" << endl;
        cout << "Requests per second: " << n / total_time << endl;
        cout << "Average roundtrip time: " << (total_time / n) * 1000000 << " microseconds" << endl;
        cout << "Time for 1 forwarding: " << ((total_time / n) * 1000000) / 2 << " microseconds" << endl;

        wait(NULL);
    }

    return 0;
}
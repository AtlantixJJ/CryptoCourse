#include <iostream>
#include <fstream>
using namespace std;

int N;
int M = 14;
int MASK =0b11111111111111;
int HIGH =0b10000000000000;
int TAR = 0b00101010010001;
//int TAR = 0b11111111111111;
int print_bit(int x,int len=M) {
    for(int i=len-1;i>=0;i--) {
        printf("%d", (x & (1 << i)) >> i);
    }
    printf("\n");
}

int xor_plus(int x) {
    int ans = 0;
    while(x > 0){
        ans ^= x & 1;
        x >>= 1;
    }
    return ans;
}

int judge_seq(int sw) {
    int state = 1;
    int ans = 0;
    int preres = 0;

    for(int it = 0; it < (1<<N); it ++) {
        state = it;
        ans = 0;
        for(int i = 0; i < M; i ++) {
            // iter
            //print_bit(state, 4);
            int high = xor_plus(state & sw);
            // output is (state & 1)
            ans = (ans << 1) + (state & 1);
            // early exit
            preres = (TAR >> (M-i-1));
            if(ans != preres) break;
            // next state shift and plus
            state = ((high << (N-1)) + (state >> 1));
        }

        if(ans == TAR) return it;
    }
    //printf("Done.\n");
    return -1;
}

int main() {
    print_bit(MASK);
    print_bit(HIGH);
    print_bit(TAR);

    int sw = 0;
    int seed = -1, weight = -1;
    for(N=2; N<40; N++) {
        printf("N=%d\n", N);
        for(sw = 0; sw < (1<<N); sw ++) {
            seed = judge_seq(sw);
            if(seed > -1){
                weight = sw;
                goto finish;
            }
        }
    }

    finish:

    if(seed > -1) {
        int ans = 0;
        for(int i = 0; i < M; i ++) {
            // iter
            //print_bit(state, 4);
            int high = xor_plus(seed & sw);
            // output is (state & 1)
            ans = (ans << 1) + (seed & 1);
            // next state shift and plus
            seed = ((high << (N-1)) + (seed >> 1));
        }
        printf("Answer:\n");
        printf("Seed:\n");
        print_bit(seed);
        printf("C:\n");
        print_bit(weight);
        printf("Sequence:\n");
        print_bit(ans);
    }

    return 0;
}
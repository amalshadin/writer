#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fifo(int *ref, int n, int frames) {
    int *f = (int *)malloc(frames * sizeof(int));
    for(int i=0; i<frames; i++) f[i] = -1;
    int faults = 0, next = 0;

    for(int i=0; i<n; i++) {
        int found = 0;
        for(int j=0; j<frames; j++) if(f[j] == ref[i]) found = 1;
        if(!found) {
            f[next] = ref[i];
            next = (next + 1) % frames;
            faults++;
        }
    }
    printf("FIFO Faults: %d\n", faults);
    free(f);
}

void lru(int *ref, int n, int frames) {
    int *f = (int *)malloc(frames * sizeof(int));
    int *time = (int *)malloc(frames * sizeof(int));
    for(int i=0; i<frames; i++) f[i] = -1;
    int faults = 0;

    for(int i=0; i<n; i++) {
        int found = 0;
        for(int j=0; j<frames; j++) {
            if(f[j] == ref[i]) {
                found = 1;
                time[j] = i;
                break;
            }
        }
        if(!found) {
            int pos = 0;
            if(i < frames) {
                for(int j=0; j<frames; j++) if(f[j] == -1) { pos = j; break; }
            } else {
                int min = time[0];
                for(int j=1; j<frames; j++) if(time[j] < min) { min = time[j]; pos = j; }
            }
            f[pos] = ref[i];
            time[pos] = i;
            faults++;
        }
    }
    printf("LRU Faults: %d\n", faults);
    free(f); free(time);
}

void optimal(int *ref, int n, int frames) {
    int *f = (int *)malloc(frames * sizeof(int));
    for(int i=0; i<frames; i++) f[i] = -1;
    int faults = 0;

    for(int i=0; i<n; i++) {
        int found = 0;
        for(int j=0; j<frames; j++) if(f[j] == ref[i]) found = 1;
        if(!found) {
            int pos = -1;
            for(int j=0; j<frames; j++) if(f[j] == -1) { pos = j; break; }
            if(pos == -1) {
                int farthest = -1;
                for(int j=0; j<frames; j++) {
                    int k;
                    for(k=i+1; k<n; k++) if(f[j] == ref[k]) break;
                    if(k > farthest) { farthest = k; pos = j; }
                }
            }
            f[pos] = ref[i];
            faults++;
        }
    }
    printf("Optimal Faults: %d\n", faults);
    free(f);
}

int main(int argc, char *argv[]) {
    if(argc != 3) return 1;
    int n = atoi(argv[1]);
    int frames = atoi(argv[2]);
    int *ref = (int *)malloc(n * sizeof(int));

    srand(time(NULL));
    printf("Reference String: ");
    for(int i=0; i<n; i++) {
        ref[i] = rand() % 10;
        printf("%d ", ref[i]);
    }
    printf("\n---\n");

    fifo(ref, n, frames);
    lru(ref, n, frames);
    optimal(ref, n, frames);

    free(ref);
    return 0;
}
//10 3

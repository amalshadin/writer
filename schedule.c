#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

#define MAX 100

typedef struct process {
    int pid, bt, at, prio, rt, ct, wt, tat;
} Process;

float avg_wt_fcfs, avg_wt_srtf, avg_wt_pri, avg_wt_rr;

/* ---------------- RESET ---------------- */
void reset_processes(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
        p[i].ct = 0;
        p[i].wt = 0;
        p[i].tat = 0;
    }
}

/* ---------------- SORT BY ARRIVAL ---------------- */
void sort(Process p[], int n) {
    Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

/* ---------------- FCFS ---------------- */
void fcfs(Process p[], int n) {
    reset_processes(p, n);
    int current_time = 0;
    float total_wt = 0;

    printf("\n--- FCFS ---\n");

    for (int i = 0; i < n; i++) {

        if (current_time < p[i].at)
            current_time = p[i].at;

        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        if (p[i].wt < 0) p[i].wt = 0;

        total_wt += p[i].wt;
        current_time = p[i].ct;

        printf("P%d\t WT=%d\n", p[i].pid, p[i].wt);
    }

    avg_wt_fcfs = total_wt / n;
    printf("Avg WT: %.2f\n", avg_wt_fcfs);
}

/* ---------------- SRTF ---------------- */

int ShortestBT(Process p[], int n, int current_time) {
    int min_rt = INT_MAX;
    int shortest_index = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && p[i].rt > 0) {
            if (p[i].rt < min_rt) {
                min_rt = p[i].rt;
                shortest_index = i;
            }
            else if (p[i].rt == min_rt &&
                     p[i].at < p[shortest_index].at) {
                shortest_index = i;
            }
        }
    }
    return shortest_index;
}

void srtf(Process p[], int n) {
    reset_processes(p, n);

    int current_time = p[0].at;   // FIX
    int completed = 0;
    float total_wt = 0;

    printf("\n--- SRTF ---\n");

    while (completed < n) {

        int s_index = ShortestBT(p, n, current_time);

        if (s_index == -1) {
    int next_arrival = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (p[i].rt > 0 && p[i].at > current_time) {
            if (p[i].at < next_arrival)
                next_arrival = p[i].at;
        }
    }

    if (next_arrival != INT_MAX)
        current_time = next_arrival;
    else
        break;  // safety

    continue;
}

        p[s_index].rt--;
        current_time++;

        if (p[s_index].rt == 0) {
            completed++;

            p[s_index].ct = current_time;
            p[s_index].tat = p[s_index].ct - p[s_index].at;
            p[s_index].wt = p[s_index].tat - p[s_index].bt;

            if (p[s_index].wt < 0) p[s_index].wt = 0;

            total_wt += p[s_index].wt;

            printf("P%d\t WT=%d\n", p[s_index].pid, p[s_index].wt);
        }
    }

    avg_wt_srtf = total_wt / n;
    printf("Avg WT: %.2f\n", avg_wt_srtf);
}

/* ---------------- PRIORITY (NON PREEMPTIVE) ---------------- */

int HighestPrio(Process p[], int n, int current_time, bool visited[]) {
    int max_prio = INT_MIN;
    int index = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && !visited[i]) {

            if (p[i].prio > max_prio) {
                max_prio = p[i].prio;
                index = i;
            }
            else if (p[i].prio == max_prio &&
                     p[i].at < p[index].at) {
                index = i;
            }
        }
    }
    return index;
}

void priority_non_preemptive(Process p[], int n) {

    reset_processes(p, n);

    int current_time = 0;
    int completed = 0;
    float total_wt = 0;
    bool visited[MAX] = { false };

    printf("\n--- Priority (Non-Preemptive) ---\n");

    while (completed < n) {

        int idx = HighestPrio(p, n, current_time, visited);

        if (idx == -1) {
            current_time++;
            continue;
        }

        current_time += p[idx].bt;

        p[idx].ct = current_time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;

        if (p[idx].wt < 0) p[idx].wt = 0;

        total_wt += p[idx].wt;

        visited[idx] = true;
        completed++;

        printf("P%d\t WT=%d\n", p[idx].pid, p[idx].wt);
    }

    avg_wt_pri = total_wt / n;
    printf("Avg WT: %.2f\n", avg_wt_pri);
}

/* ---------------- ROUND ROBIN ---------------- */

void roundrobin(Process p[], int n, int qt) {

    reset_processes(p, n);

    int queue[1000];     // increased size
    int front = 0, rear = 0;

    int current_time = 0;
    int completed = 0;
    float total_wt = 0;

    printf("\n--- ROUND ROBIN ---\n");

    int i = 0;

    current_time = p[0].at;

    while (i < n && p[i].at <= current_time) {
        queue[rear++] = i;
        i++;
    }

    while (completed < n) {

        if (front == rear) {
            current_time++;
            while (i < n && p[i].at <= current_time)
                queue[rear++] = i++;
            continue;
        }

        int idx = queue[front++];

        int exec_time = (p[idx].rt > qt) ? qt : p[idx].rt;

        p[idx].rt -= exec_time;
        current_time += exec_time;

        while (i < n && p[i].at <= current_time)
            queue[rear++] = i++;

        if (p[idx].rt > 0) {
            queue[rear++] = idx;
        }
        else {
            completed++;

            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            if (p[idx].wt < 0) p[idx].wt = 0;

            total_wt += p[idx].wt;

            printf("P%d\t WT=%d\n", p[idx].pid, p[idx].wt);
        }
    }

    avg_wt_rr = total_wt / n;
    printf("Avg WT: %.2f\n", avg_wt_rr);
}

/* ---------------- MAIN ---------------- */

int main() {

    int n, qt;
    Process p[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d (BT AT Priority): ", i + 1);
        scanf("%d %d %d", &p[i].bt, &p[i].at, &p[i].prio);
    }

    printf("Enter time quantum: ");
    scanf("%d", &qt);

    sort(p, n);

    fcfs(p, n);
    srtf(p, n);
    priority_non_preemptive(p, n);
    roundrobin(p, n, qt);

    float min_avg = avg_wt_fcfs;

    if (avg_wt_srtf < min_avg) min_avg = avg_wt_srtf;
    if (avg_wt_pri < min_avg) min_avg = avg_wt_pri;
    if (avg_wt_rr < min_avg) min_avg = avg_wt_rr;

    printf("\nBest Average WT: %.2f\n", min_avg);

    return 0;
}

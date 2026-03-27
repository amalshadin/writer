#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m, i, j;

    printf("Enter the number of Processes: ");
    scanf("%d", &n);
    printf("Enter the number of Resource types: ");
    scanf("%d", &m);

    int alloc[n][m], request[n][m], avail[m];

    printf("\nEnter the Allocation Matrix:\n");
    for (i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    printf("\nEnter the Request Matrix:\n");
    for (i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++) {
            scanf("%d", &request[i][j]);
        }
    }

    printf("\nEnter Available Resources: ");
    for (i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }

    int work[m];
    bool finish[n];

    // Initialize Work = Available
    for (i = 0; i < m; i++) work[i] = avail[i];

    // Initialize Finish: if a process has no resources allocated, 
    // it can't be part of a deadlock cycle.
    for (i = 0; i < n; i++) {
        bool all_zero = true;
        for (j = 0; j < m; j++) {
            if (alloc[i][j] != 0) {
                all_zero = false;
                break;
            }
        }
        finish[i] = all_zero;
    }

    // Detection Algorithm Logic
    
    bool found;
    do {
        found = false;
        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                bool can_be_satisfied = true;
                for (j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        can_be_satisfied = false;
                        break;
                    }
                }

                if (can_be_satisfied) {
                    for (j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
    } while (found);

    // Final Reporting
    bool deadlocked = false;
    printf("\n--- Deadlock Detection Result ---\n");
    for (i = 0; i < n; i++) {
        if (!finish[i]) {
            printf("Process P%d is DEADLOCKED.\n", i);
            deadlocked = true;
        }
    }

    if (!deadlocked) {
        printf("No deadlock detected. The system is in a safe state.\n");
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

struct candidate {
    int id;
    char name[50];
    int votes;
};

struct candidate candidates[MAX];
int count = 0;

int voterQueue[MAX];
int front = -1, rear = -1;

void enqueue(int voterID) {
    if (rear == MAX - 1) {
        printf("Queue full!\n");
        return;
    }
    if (front == -1) front = 0;
    voterQueue[++rear] = voterID;
    printf("Voter %d added to queue.\n", voterID);
}

int dequeue() {
    if (front == -1) {
        printf("Queue empty!\n");
        return -1;
    }
    return voterQueue[front++];
}

struct VoteHistory {
    int voterID;
    int candidateID;
} stack[MAX];

int top = -1;

void push(int voterID, int candidateID) {
    if (top == MAX - 1) {
        printf("Stack full!\n");
        return;
    }
    stack[++top].voterID = voterID;
    stack[top].candidateID = candidateID;
}

void pop() {
    if (top == -1) {
        printf("No vote to undo!\n");
        return;
    }
    int voterID = stack[top].voterID;
    int candidateID = stack[top].candidateID;
    candidates[candidateID].votes--;
    printf("Undo vote: Voter %d's vote removed from %s\n",
            voterID, candidates[candidateID].name);
    top--;
}

struct winner {
    char name[50];
    int votes;
    struct winner *next;
};

struct winner *head = NULL;

void addWinner(char *name, int votes) {
    struct winner *newNode = (struct winner*) malloc(sizeof(struct winner));
    strcpy(newNode->name, name);
    newNode->votes = votes;
    newNode->next = NULL;

    if (head == NULL) head = newNode;
    else {
        struct winner *temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

// ---------- Candidate Functions ----------
void addCandidate(int id, char name[]) {
    candidates[count].id = id;
    strcpy(candidates[count].name, name);
    candidates[count].votes = 0;
    count++;
}

void castVote(int voterID, int candidateID) {
    if (candidateID < 0 || candidateID >= count) {
        printf("Invalid candidate!\n");
        return;
    }
    candidates[candidateID].votes++;
    push(voterID, candidateID);
    printf("Voter %d voted for %s\n", voterID, candidates[candidateID].name);
}

void showResults() {
    printf("\n--- Voting Results ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - %d votes\n", candidates[i].id, candidates[i].name, candidates[i].votes);
    }
}

void findWinner() {
    int maxVotes = -1;
    for (int i = 0; i < count; i++) {
        if (candidates[i].votes > maxVotes) {
            maxVotes = candidates[i].votes;
        }
    }
    printf("\n--- Winner(s) ---\n");
    for (int i = 0; i < count; i++) {
        if (candidates[i].votes == maxVotes) {
            printf("%s with %d votes\n", candidates[i].name, candidates[i].votes);
            addWinner(candidates[i].name, candidates[i].votes);
        }
    }
}

int main() {
    int choice;
    addCandidate(0, "A-Group");
    addCandidate(1, "B-Group");
    addCandidate(2, "C-Group");

    do {
        printf("\n--- Digital Voting System ---\n");
        printf("1. Add Voter to Queue\n");
        printf("2. Cast Vote (from Queue)\n");
        printf("3. Show Results\n");
        printf("4. Undo Last Vote\n");
        printf("5. Declare Winner\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int voterID;
            printf("Enter Voter ID: ");
            scanf("%d", &voterID);
            enqueue(voterID);
        }
        else if (choice == 2) {
            int voterID = dequeue();
            if (voterID != -1) {
                printf("Enter Candidate ID to vote for: ");
                int cid; scanf("%d", &cid);
                castVote(voterID, cid);
            }
        }
        else if (choice == 3) {
            showResults();
        }
        else if (choice == 4) {
            pop();
        }
        else if (choice == 5) {
            findWinner();
        }
    } while (choice != 6);

    return 0;
}


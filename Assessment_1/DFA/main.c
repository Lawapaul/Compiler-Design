#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(){
    int n;
    printf("Enter No of States: ");
    scanf("%d",&n);
    int m;
    printf("Enter No of Alphabets: ");
    scanf("%d",&m);
    int transition_table[n][m];
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            printf("Transition (q%d,%c) or -1: ",i,'a'+j);
            scanf("%d",&transition_table[i][j]);
        }
    }
    int startState;
    printf("\nEnter Start state: ");
    scanf("%d",&startState);
    
    int final_state;
    printf("\nEnter No of Final State: ");
    scanf("%d",&final_state);
    
    int f_states[final_state];
    for(int i=0;i<final_state;i++){
        printf("Enter Final State %d: ",i+1);
        scanf("%d",&f_states[i]);
    }
    
    char input[50];
    printf("Enter Input String: ");
    scanf(" %s",input);
    int start = startState;
    int ptr = 0;
    while(ptr < strlen(input)){
        int col = input[ptr] - 'a'; 
        if(col < 0 || col >= m){
            printf("\nRejected (Invalid symbol)");
            return 0;
        }
        int transition = transition_table[start][col];
        if(transition==-1){
            printf("\nRejected");
            return 0;
        }
        printf("\n(q%d,%c) -> q%d",start,input[ptr],transition);
        start = transition;
        ptr++;
    }
    int accepted=0;
    for(int i=0;i<final_state;i++){
        if(start==f_states[i]){
            printf("\nAccepted");
            return 0;
        }
    }
    printf("\nRejected");
}
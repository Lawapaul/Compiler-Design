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
    
    int f_state[final_state];
    for(int i=0;i<final_state;i++){
        printf("Enter Final State %d: ",i+1);
        scanf("%d",&f_state[i]);
    }
    
    //Non Terminals
    printf("Non Terminals: { ");
    for(int i=0;i<n;i++){
        if(i!=n-1) printf("%c,",'A' + i);
        else printf("%c",'A' + i);
    }
    printf(" }\n");
    
    //Terminals
    printf("Terminals: { ");
    for(int i=0;i<m;i++){
        if(i!=m-1) printf("%c,",'a' + i);
        else printf("%c",'a' + i);
    }
    printf(" }\n");
    
    //Grammer
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(transition_table[i][j] != -1){
                int transition = transition_table[i][j];
                printf("\n%c -> %c%c",'A'+i,'a'+j,'A'+transition);
                for(int k=0;k<final_state;k++){
                    if(transition == f_state[k]){
                        printf("\n%c -> %c\n",'A'+i,'a'+j);
                    }
                }
            }
        }
    }
    int isStartFinal = 0;
    for(int i=0;i<final_state;i++){
        if(startState == f_state[i]){
            isStartFinal = 1;
            break;
        }
    }
    if(isStartFinal){
        printf("\n%c -> Epsilon",'A' + startState);
    }
}
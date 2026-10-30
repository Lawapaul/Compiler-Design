#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char nfa[20][20][20];
char dfa[20][20];
char dfa_table[20][20][20];

int main(){
    int n;
    printf("\nEnter no of States in NFA: ");
    scanf("%d",&n);
    
    int m;
    printf("Enter no of Alphabets in NFA: ");
    scanf("%d",&m);
    
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            printf("Transition (q%d,%c) else (-) : ",i,'a'+j);
            scanf("%s",nfa[i][j]);
        }
    }
    char start_state[50];
    printf("Enter Start State: ");
    scanf("%s",start_state);
    
    int f_states;
    printf("Enter no of Final States: ");
    scanf("%d",&f_states);
    
    char final_states[f_states][20];
    for(int i=0;i<f_states;i++){
        printf("Enter Final State %d: ",i+1);
        scanf("%s",final_states[i]);
    }
    
    int total = 0;
    strcpy(dfa[total++],start_state);
    for(int i=0;i<total;i++){
        for(int j=0;j<m;j++){
            char result[50] = "";
            for(int k=0;dfa[i][k]!='\0';k++){
                char c = dfa[i][k] - '0';
                char* t = nfa[c][j];
                if(strcmp(nfa[c][j], "-") != 0){
                    strcat(result, nfa[c][j]);
                }
            }
            char unique[50]="";
            for(int x=0;result[x] != '\0';x++){
                int found=0;
                for(int y=0;unique[y] != '\0';y++){
                    if(result[x] == unique[y]){
                        found=1;
                        break;
                    }
                }
                if(!found){
                    int len = strlen(unique);
                    unique[len] = result[x];
                    unique[len+1] = '\0';
                }
            }
            strcpy(result,unique);
            strcpy(dfa_table[i][j],result);
            int exists = 0;
            for(int k=0;k<total;k++){
                if(strcmp(dfa[k],result)==0){
                    exists=1;
                    break;
                }
            }
            if(!exists && strlen(result) > 0){
                strcpy(dfa[total++],result);
            }
        }
    }
    for(int i=0;i<total;i++){
        int isFinal = 0;
        for(int x=0;dfa[i][x] != '\0';x++){
            char temp[2];
            temp[0] = dfa[i][x];
            temp[1] = '\0';

            for(int j=0;j<f_states;j++){
                if(strcmp(temp, final_states[j]) == 0){
                    isFinal = 1;
                }
            }
        }
        if(isFinal) printf("Dfa State %d %s: ",i,"FINAL");
        else printf("Dfa State %d:",i);
        for(int j=0;j<m;j++){
            printf("\nTransition (%d,%c): %s",i,'a'+j,dfa_table[i][j]);
        }
    }
    return 0;
    
}
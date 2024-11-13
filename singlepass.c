#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct optab{
    int pos;
    struct optab* next;
}optab;

struct symbol{
    char symbol[10];
    int address;
    int isdefined;
    struct optab* next;
}symbol[50];

int main(){
    FILE *f1,*f2,*f3;
    int start;
    int locctr;
    int symrec = 0;
    char label[50],op[50],operand[50],mne[10],hexcode[10];
    f1 = fopen("input_pgm.txt","r");
    f2 = fopen("opcode.txt","r");
    f3 = fopen("s_p_result.txt","w");
    if(!f1 || !f2 || !f3){
        printf("error in file opening");
        return 1;
    }

    fscanf(f1,"%s %s %s", label,op,operand);
    if(strcmp(op,"START") == 0){
        start = strtol(operand,NULL,16);
        locctr = start;
        fprintf(f3,"H^%s^%06x\n",label,start);
    }else{
        locctr = 0;
    }
    fprintf(f3,"T^%06x^",locctr);
    int pos = ftell(f3);
    fprintf(f3,"000");
    int record_text = 0;

    while(fscanf(f1,"%s %s %s",label,op,operand)!=EOF){
         if(strcmp(label,"**")!=0){
            int symbolfound = -1;
            for(int i = 0; i < symrec; i++){
                if(strcmp(label,symbol[i].symbol) == 0){
                    symbolfound = i;
                    break;
                }
            }

            if(symbolfound == -1){
                strcpy(symbol[symrec].symbol,label);
                symbol[symrec].address = locctr;
                symbol[symrec].isdefined = 1;
                symbol[symrec].next = NULL;
                symrec++;
            }else{
                if(!symbol[symbolfound].isdefined){
                    symbol[symbolfound].address = locctr;
                    symbol[symbolfound].isdefined = 1;
                    struct optab *ptr = symbol[symbolfound].next;
                    while(ptr != NULL){
                        fseek(f3,ptr->pos,SEEK_SET);
                        fprintf(f3,"%04X",symbol[symbolfound].address);
                        struct optab *temp = ptr;
                        ptr = ptr->next;
                        free(temp);
                    }
                    symbol[symbolfound].next = NULL;
                    fseek(f3,0,SEEK_END);
                }
            }
        }
        rewind(f2);
        int found = 0;
        while(fscanf(f2,"%s %s",mne,hexcode) != EOF){
            if(strcmp(op,mne) == 0){
                found = 1;
                break;
            }
        }
        if(found){
            int symbolfound = -1;
            for(int i = 0; i < symrec; i++){
                if(strcmp(operand,symbol[i].symbol) == 0){
                    symbolfound = i;
                    break;
                }
            }
            fprintf(f3,"%s",hexcode);
            int fp = ftell(f3);

            if(symbolfound == -1){
                strcpy(symbol[symrec].symbol,operand);
                symbol[symrec].isdefined = 0;
                fprintf(f3,"0000^");
                symbol[symrec].next = (struct optab*)malloc(sizeof(struct optab));
                symbol[symrec].next->pos = fp;
                symbol[symrec].next->next = NULL;
                symrec++;
            }else{
                if(!symbol[symbolfound].isdefined){
                    fprintf(f3,"0000^");
                    struct optab *ptr = symbol[symbolfound].next;
                    while(ptr->next != NULL){
                        ptr = ptr->next;
                    }
                    ptr->next = (struct optab*)malloc(sizeof(struct optab));
                    ptr->next->pos = fp;
                    ptr->next->next = NULL;
                }else{
                    fprintf(f3,"%04X^",symbol[symbolfound].address);
                }
            }
            locctr+=3;
            record_text+=3;
        }else{
            if(strcmp(op,"WORD") == 0){
                fprintf(f3,"%06d^",atoi(operand));
                locctr +=3;
                record_text+=3;
            }else if(strcmp(op,"RESW") == 0){
                locctr += (3 * strtol(operand,NULL,16));
            }else if(strcmp(op,"RESB") == 0){
                locctr +=  atoi(operand);
            }
        }
       
    }
    fprintf(f3,"\nE^%06X",start);
    fseek(f3,pos,SEEK_SET);
    fprintf(f3,"%02X^",record_text);
    fseek(f3,0,SEEK_END);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    return 0;
}
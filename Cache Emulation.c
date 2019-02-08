#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define MAX_ADDRESS_LENGTH 10
struct queue{
    char *address;
    struct queue* left;
    struct queue* right;
};

void InsertLRUCacheEntry(struct queue** head, char* newaddress, int capacity, int* cmplmisses){
    //assumes entry is not present(miss). Queues a new node and makes it the head.
    //just create a new node, and make it the head. No frees needed.
    struct queue* new=(struct queue*)malloc(sizeof(struct queue));
    new->left=NULL;new->right=*head;
    new->address=(char *)malloc(MAX_ADDRESS_LENGTH * sizeof(char));
    strcpy(new->address, newaddress);
    if(*head!=NULL) (*head)->left=new;
    *head=new;
    //now recognise which miss it is.
    struct queue* temp=*head;
    for(int i=0;i<capacity && temp!=NULL;i++){ /*traverse till the end of cache*/
        temp=temp->right;
    }//temp now points to the first entry after cache.
    if(temp==NULL) {    //means cache is not overflown.
        *cmplmisses=*cmplmisses+1;
        return;
    }
    while(temp!=NULL && (strcmp(temp->address, newaddress)!=0)){ /**/
        temp=temp->right;
    }
    if(temp==NULL) {
        *cmplmisses=*cmplmisses+1;
        return;
    }

}


int QueryLRUCache(struct queue** head, char *key, int capacity){ 
    //returns 1 if hit, -1 if miss. If it's a hit, bring it to the front.
    struct queue* temp=*head;
    for(int i=0;i<capacity && temp!=NULL;i++){
        if(!strcmp(temp->address, key)){    //hit; and found at temp. modify the queue and return 1.
            if(temp==*head) return 1;
            if(temp->left!=NULL) (temp->left)->right=temp->right;
            if(temp->right!=NULL) (temp->right)->left=temp->left;
            temp->right=*head;
            temp->left=NULL;
            (*head)->left=temp;
            *head=temp;
            return 1;
        }
        temp=temp->right;   //if not found now, traverse right and look.
    }
    return -1;
    
}

void InsertFIFOCacheEntry(struct queue** head, char* newaddress, int capacity, int* cmplmisses){
    //just insert a new head. no frees required.
    struct queue* new=(struct queue*)malloc(sizeof(struct queue));
    new->left=NULL;new->right=*head;
    new->address=(char*)malloc(MAX_ADDRESS_LENGTH * sizeof(char));
    strcpy(new->address, newaddress);
    if(*head!=NULL) (*head)->left=new;
    *head=new;
    struct queue* temp=*head;
    for(int i=0;i<capacity && temp!=NULL;i++){
        temp=temp->right;
    }
    if(temp==NULL) {
        *cmplmisses=*cmplmisses+1;
        return;
    }
    while(temp!=NULL && (strcmp(temp->address, newaddress)!=0)){
        temp=temp->right;
    }
    if(temp==NULL) {
        *cmplmisses=*cmplmisses+1;
        return;
    }
    



}

int QueryFIFOCache(struct queue** head, char* key, int capacity){
    //returns 1 if hit, -1 if miss.
    struct queue* temp=*head;
    for(int i=0;i<capacity && temp!=NULL;i++){
        if(!strcmp(temp->address, key)) return 1;
        temp=temp->right;
    }
    return -1;
}

int QueryOPTIMALCache(char** cache, int i, int ways, int* cmplmisses){   //returns 1 if HIT, -1 else.
    int found=-1;
    for(int j=i;j<i+ways;j++){
        if(!strcmp(cache[j], cache[i+ways])){
            found=1;
            break;
        }
    }
    if(found==-1){  //If it's a miss, find which miss it is.
        for(int j=0;j<i;j++){
            if(!strcmp(cache[i+ways], cache[j])) break;
            if(strcmp(cache[j], cache[i+ways])!=0 && j==i-1) *cmplmisses+=1;
        }
        if(i==0) *cmplmisses+=1;
    }
    return found;
}

int FindMax(char** cache, int count, int i, int ways){
    int max=0;
    int maxindex=i;
    for(int j=i;j<ways+i;j++){
        bool flag=false;  //for each cache element from i to i+ways-1, find one with maximum reuse time.
        for(int k=ways+i;k<count;k++){  //compare cache[j] with all entries in the future, i.e. from k=ways to k=count-1.
            if(!strcmp(cache[j], cache[k])){
                flag=true;   //if the entry occurs in the future...
                if((k-ways-i+1)>max){
                    max=k-ways-i+1;     //if reuse interval is more than max, then replace max.
                    maxindex=j;
                    if (k==count-1) return maxindex;     //j is the best candidate to replace so far.
                    break;  //stop looking into the future, since we've found the first reuse of cache[j]
                }
                break;          
            }
            if(k==count-1 && !flag){     //if there's no match for all k, that means, the entry is never gonna be reused.
                maxindex=j;     //hence, update maxindex, and this entry is the best candidate to be replaced of all entries.
                max=count;
                return maxindex;    //no candidate is better than j, so return j.
            }
        }
    }
    return maxindex;
}

void swap(char* a, char* b){
    char temp[MAX_ADDRESS_LENGTH];
    strcpy(temp,a);
    strcpy(a, b);
    strcpy(b, temp);
}

int main(int argc, char *argv[]){
    int ways;   //indicates number of ways in the cache.
    sscanf(argv[3], "%d", &ways);
    FILE *file;
    int c;
    int count=0;
    struct queue *head=NULL;
    int misses=0;
    int cmplmisses=0, capmisses=0;
    file=fopen(argv[2], "r");
    if(file==NULL){
        printf("Failed to read given input file.\n");
        return 0;
    }
    char a[MAX_ADDRESS_LENGTH];
    char temp[]="temp";
    char outname[100];strcpy(outname, "");
    strcat(outname, "CS17BTECH11022");strcat(outname, "_");
    strcat(outname, argv[1]);strcat(outname, "_");
    int len=strlen(argv[2]);
    argv[2][len-4]='\0';    /*Only to remove the .txt extension from the input file. Change 4 for other extensions.*/
    strcat(outname, argv[2]);strcat(outname, "_");
    strcat(outname, argv[3]);strcat(outname, ".out");
    FILE* outfile;
    outfile=fopen(outname, "w");
    if(!strcmp(argv[1], "LRU")){    //if LRU is specified...
        FILE* buffer;
        buffer=fopen(temp, "w+");
        while((c=fgetc(file))!=EOF){
            if(c==' ' || c=='\n'){  //skip whitespace
                continue;
            }
            ungetc(c, file);    //push back last read character.
            fscanf(file, "%s", a);  //scan the address from input file.
            count++;    //increment the address count.
            if (QueryLRUCache(&head, a, ways)!=1){
                InsertLRUCacheEntry(&head, a, ways, &cmplmisses);
                fprintf(buffer, "MISS\n");
                ++misses;
            }
            else{
                fprintf(buffer, "HIT\n");
            }
    
        }
        capmisses=misses-cmplmisses;
        fclose(file);
        fprintf(outfile,"TOTAL_ACCESSES = %d\nTOTAL_MISSES = %d\nCOMPULSORY_MISSES = %d\nCAPACITY_MISSES = %d\n", count, misses, cmplmisses, capmisses);
        fseek(buffer, 0, SEEK_SET);
        c=fgetc(buffer);
        while(c!=EOF){
            fputc(c, outfile);
            c=fgetc(buffer);
        }

        fclose(outfile);
        fclose(buffer);
        remove(temp);
        return 0;
    }   
    if(!strcmp(argv[1], "FIFO")){   //if FIFO is specified...
        FILE* buffer;
        buffer=fopen(temp, "w+");
        while((c=fgetc(file))!=EOF){
            if(c==' ' || c=='\n'){  //skip whitespace
                continue;
            }
            ungetc(c, file);
            fscanf(file, "%s", a);  //scan the address from input file.
            count++;    //increment the address count.
            if (QueryFIFOCache(&head, a, ways)!=1){
                InsertFIFOCacheEntry(&head, a, ways, &cmplmisses);
                fprintf(buffer, "MISS\n");
                ++misses;
            }
            else{
                fprintf(buffer, "HIT\n");
            }
    
        }
        //conclusion and file closing.
        capmisses=misses-cmplmisses;
        fclose(file);
        fprintf(outfile,"TOTAL_ACCESSES = %d\nTOTAL_MISSES = %d\nCOMPULSORY_MISSES = %d\nCAPACITY_MISSES = %d\n", count, misses, cmplmisses, capmisses);
        fseek(buffer, 0, SEEK_SET);
        c=fgetc(buffer);
        while(c!=EOF){
            fputc(c, outfile);
            c=fgetc(buffer);
        }

        fclose(outfile);
        fclose(buffer);
        remove(temp);
        return 0;
    }
    if(!strcmp(argv[1], "OPTIMAL")){    //if OPTIMAL is specified...
        FILE* buffer;
        buffer=fopen(temp, "w+");
        char **cache=NULL;
        while((c=fgetc(file))!=EOF){
            if(c==' ' || c=='\n'){  //skip whitespace
                continue;
            }
            ungetc(c, file);
            fscanf(file, "%s", a);  //scan the address from input file.
            count++;    //increment the address count.
            cache = realloc(cache, count * sizeof(char*));
            cache[count-1]= (char *)malloc(MAX_ADDRESS_LENGTH * sizeof(char));
            strcpy(cache[count-1], a);
        }
        /* Now, cache[i] contains the all the addresses from the file. */
        /* Now comes the main part of managing the cache. */
        int cache_start=0;int cache_end=0;//indices of first and last entries of the cache.
        fprintf(buffer, "MISS\n");  //The first one is a corner case, hence handle it manually.
        misses++;cmplmisses++;  //cache now contains only one entry, the first one.
        int j=1;
        for (j=1;misses<ways && j<count;j++){   //misses<ways implies cache has room and j<count implies addresses are pending to be accessed.
            for(int k=cache_start;k<=cache_end;k++){
                if(!strcmp(cache[j], cache[k])){// cache[j] is the access to be made, cache[k] is in the cache.
                    fprintf(buffer, "HIT\n");
                    swap(cache[j], cache[cache_start]); //Hits get accumulated at the left of the cache.
                    ++cache_start;++cache_end;
                    break;  //cache[j] is found. no more looking is necessary.
                }
                if(k==cache_end){
                    //printf("Address %s is a MISS\n", cache[j]);
                    fprintf(buffer, "MISS\n");
                    ++misses;
                    ++cmplmisses;
                    ++cache_end;
                    break;    //misses widen the cache.
                }
                
            }
        }
        if(j==count){   //this means all addresses have been accessed. Just print the results.
            fprintf(outfile,"TOTAL_ACCESSES = %d\nTOTAL_MISSES = %d\nCOMPULSORY_MISSES = %d\nCAPACITY_MISSES = %d\n", count, misses, cmplmisses, misses-cmplmisses);
            fseek(buffer, 0, SEEK_SET);
            c=fgetc(buffer);
            while(c!=EOF){
                fputc(c, outfile);
                c=fgetc(buffer);
            }
            fclose(file);
            free(cache);
            fclose(outfile);
            fclose(buffer);
            remove(temp);
            return 0;
        }
        if(misses==ways){    //cache is full and addresses are pending to be accessed. Replacement is needed.   
            for(;cache_start<count-ways;cache_start++, cache_end++){ //for each window...
                if(QueryOPTIMALCache(cache, cache_start, ways, &cmplmisses)!=1){   //the next entry is not found, replacement is needed.
                    j=FindMax(cache, count, cache_start, ways);
                    swap(cache[cache_start], cache[j]);   //the first entry in the cache is swapped with cache[j].
                    misses++;
                    //printf("%s is a MISS\n", cache[cache_start]);
                    fprintf(buffer, "MISS\n");
                }
                else{
                    swap(cache[cache_start], cache[cache_start+ways]);
                    fprintf(buffer, "HIT\n");
                    //fprintf(buffer, "HIT\n");
                }
            }
            fprintf(outfile,"TOTAL_ACCESSES = %d\nTOTAL_MISSES = %d\nCOMPULSORY_MISSES = %d\nCAPACITY_MISSES = %d\n", count, misses, cmplmisses, misses-cmplmisses);
            fseek(buffer, 0, SEEK_SET);
            c=fgetc(buffer);
            while(c!=EOF){
                fputc(c, outfile);
                c=fgetc(buffer);
            }
            fclose(file);
            free(cache);
            fclose(outfile);
            fclose(buffer);
            remove(temp);
            return 0;
            
        }
    }

}
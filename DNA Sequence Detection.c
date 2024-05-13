// Asad Zangi - 2546042
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct node{
   char sequence[81];
   struct node *next;
};

struct pattern{
    char p[81];
    int times;
    int sequences;
};

struct node * readSequences(char filename[]);
void searchPatterns(struct node *head, struct pattern *patterns, int numberOfPatterns);
void sortPatterns(struct pattern *patterns, int numberOfPatterns);
void printPatterns(struct pattern *patterns, int numberOfPatterns);

int main(int argc, char *argv[]){
    struct node *head = NULL,*temp;
    struct pattern *patterns = NULL;
    int pat,i=0;
    char temp_pattern[81];

    // WRITE YOUR CODE HERE
    //Declaring the Filename;
    char filename[50];
    printf("Enter the file path containing the DNA sequence: ");
    scanf("%s",filename);
    //Calling the readSequences function and passing the file name as a argument;
    head = readSequences(filename);
    //Number of patterns user want to search;
    printf("Enter the number of patterns: ");
    scanf("%d",&pat);
    
    //Memory allocation for the array;
    patterns = malloc(pat * sizeof(struct pattern));
    if(patterns == NULL){
    	printf("Memory allocation for array failed");
	}
	//User will enter pattern;
	for(i=0;i<pat;i++){
		printf("Enter the Pattern: ");
		scanf("%s",temp_pattern);
		strcpy(patterns[i].p,temp_pattern);
		patterns[i].times=0;
		patterns[i].sequences=0;
	}
	

	//Calling the searchPatterns function;
	searchPatterns(head,patterns,pat);	
	//Calling the sort function to sort the data;
    sortPatterns(patterns,pat);
    //calling the print pattern function;
   printPatterns(patterns,pat);
    
    

    return 0;
}

struct node * readSequences(char filename[]){
	//Declared varables;
	struct node *head=NULL;
	struct node *temp=NULL;
	struct node *pointer;
	//Declaring the file;
	FILE *inFile;
	inFile = fopen("DNAsequences.txt","r");
	if(inFile == NULL){
		printf("File not Found. Check the destination\n");
	}
	// WRITE YOUR CODE HERE
	//Temp values to store the DNA for temporary time;
	char temp_dna[81];
	//Reading the file line by line;
	while(fscanf(inFile,"%80s\n",temp_dna) != EOF){
		//When head is null,it will store the first dna;
		if(head == NULL){
			//allocating memory for head;
			head = malloc(sizeof(struct node));
			if(head==NULL){
				printf("Failed to allocate memory");
			}
			head->next = NULL;
			//Copying the data;
			strcpy(head->sequence, temp_dna);
			//printf("%s\n",head->sequence);
			//making the pointer to point the first linked list;
			//It will help us to store the second dna easily;
			pointer = head;		
		}
		else{
			//allocating memory for new linked list;
			temp = malloc(sizeof(struct node));
			if(temp==NULL){
				printf("Failed to allocate memory");
			}
			temp->next = NULL;
			//pointer next will point to temp;
			//moving the pointer to the temp;
			pointer->next = temp;
			pointer=temp;
			strcpy(temp->sequence, temp_dna);
		//	printf("%s\n",temp->sequence);
		}
	}

	return head;
}

void searchPatterns(struct node *head, struct pattern *patterns, int numberOfPatterns){
	struct node *temp;
	int len_pat,len_text;
	int j=0,hash,hash_p,hash_t,i,s,k,found=0,count=0;
	temp = head;
	//This while loop will go through all nodes of linked list;
	while(temp != NULL){
		//This while loop will go through all indexes of pattern array;
		for(j=0;j<numberOfPatterns;j++){
			//this code will take one linked list and compare all of the patterns;
			//length of pattern stored in index j;
			len_pat = strlen(patterns[j].p);
			//length of sequence;
			len_text = strlen(temp->sequence);
			//hash formula; and it's value at the start;
			hash = (int)pow(4,len_pat-1) % 11;
			//hash of pattern and text are zero at start;	
			hash_p=0;
			hash_t=0;
			
			//finding the hash for both text and pattern entered by user;
			for(i=0;i<len_pat;i++){
				
				hash_p = ((4*hash_p + patterns[j].p[i]) % 11);
				hash_t = ((4*hash_t + temp->sequence[i]) % 11);
			}
			//this for loop is shifting the pattern to left;
			for(s=0;s<=(len_text-len_pat);s++){
				//when pattern hash and text hash are equal;
				if(hash_p == hash_t){
					//we will compare the each char of pattern with text;
					//sometime it causes spurious heat;
					if(strncmp(temp->sequence+s,patterns[j].p,len_pat) == 0){
								//increamenting the times of pattern with one;
								patterns[j].times++;
								
								found=1;
								// this if condition will increament sequences by one time only when
								//pattern is avaiable at text;
								if(found==1 && count==0){
									count=1;
									patterns[j].sequences++;
								}
					}
					
				} // End of if Statement;
				if(s<(len_text-len_pat)){
					hash_t = (4* (hash_t-temp->sequence[s]*hash) + temp->sequence[s+len_pat]) % 11;
				}
				//sometime text hash goes to negative so add 11;
				if(hash_t < 0){
					hash_t=hash_t+11; 
				}
			}//End of for loop;
			
		}
		//reseting the values;
		found=0;
		count=0;
		//moving to the next linked list node which contain the next dna text;
		
		temp = temp->next;
	}
	

}

void sortPatterns(struct pattern *patterns, int numberOfPatterns){


	// I converted the pusedo code for insertion code to c program;
	//the puesdo code was taken for cng 213 notes available on odtuclass;
	int i=0,j;
	int temp_times,temp_seq;
	char temp_p[81];
	for(i=1;i<numberOfPatterns;i++){
		temp_times = patterns[i].times;
		temp_seq = patterns[i].sequences;
		strcpy(temp_p,patterns[i].p);
		
		for(j=i;j>0 && temp_times >  patterns[j-1].times;j--){
			patterns[j].times = patterns[j-1].times;
			patterns[j].sequences = patterns[j-1].sequences;
			strcpy(patterns[j].p, patterns[j-1].p);
		}
		patterns[j].times = temp_times;
		patterns[j].sequences = temp_seq;
		strcpy(patterns[j].p, temp_p);	
			
	}
	

}

void printPatterns(struct pattern *patterns, int numberOfPatterns){

	int i=0;
	printf("\n");
	for(i=0;i<numberOfPatterns;i++){
		printf("%s is detected %d times in %d DNA sequences\n",patterns[i].p,patterns[i].times,patterns[i].sequences);
	}

}

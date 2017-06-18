#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

// global data
int *PID_array;
#define path "/home/jaydeep/Desktop/Tejas_heckathon_Jaydeep/"

typedef struct process             // process data
{
	int pid;
	int dest;
	int src;
	int weight;
	struct process *next;
}my_data;

// List of process data;
typedef struct process_head
{
	my_data *head;
}List;

typedef struct{
	int V;
	struct List *array;
}Graph;
//--------------------------------------------------------------------------------
void print_array(int *arr)
{
	int i;
	for(i=0;i<5;i++)
		printf(" %d ",arr[i]);
}
//----------------------------------------------------------------------------------

void createProcess(Graph **p)
{
	printf("\n\n");
	if(!*p)
		{
		int n=rand()%10;
		while(n--);
		}
	FILE *fp=fopen("info.txt","r");
	char line[7],*tok;
	
	while(fgets(line,7,fp))
	{
		//puts(line);
		int src=line[0]- '0';
		int dest=line[2]- '0';
		int weight=line[4]- '0';
		printf("  %d  %d  %d \n",src,dest,weight);
	}
	
	
}


//--------------------------------------------------------------------------------
void update_file(FILE *fp)
{
	
	fprintf(fp,"%s","2-1-20\n");
	fprintf(fp,"%s","1-1-20\n");
	fprintf(fp,"%s","2-3-30\n");
	fprintf(fp,"%s","2-4-40\n");
	fprintf(fp,"%s","3-1-50\n");
	fprintf(fp,"%s","4-1-60\n");
}



//---------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if(argc!=2)
	{
		printf("MSG: ./a.out NUMER_OF_PROCESS\n");
		exit(0);
	}
	int num=atoi(argv[1]);
	int j=0,i;
	int s;
	Graph *p=NULL;
	// File in that Edge and wight are 
//	FILE *fp=fopen("info.txt","r+");
//	update_file(fp);	
	
	PID_array=calloc(num,sizeof(int));
	
	for(i=0;i<num;i++)
	{
		int pid_t=fork(); 
		if(pid_t==-1)
			{
				perror("Creating process:");
				exit(1);
			}
		else if(pid_t > 0)
		{	

		//	sleep(2);
			wait(&s);
			exit(0);
	
			//execl(path,(char const * )NULL);
			
		}	

		else if(pid_t==0)
		{


			PID_array[j++]=getppid();
			createProcess(&p);
			break;
			//exit(0);
		}

	}
	printf("\n");	
	print_array(PID_array);
	printf("\n");	
	return 0;	
}


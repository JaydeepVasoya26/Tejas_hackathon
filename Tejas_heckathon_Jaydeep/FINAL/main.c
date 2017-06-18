#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<pthread.h>


//----------------------------------idea-------------------------------------
/*

    Array of process IDs

	--------       ------    ------
	| -28	|----->|  -20|-->| 10  |
	|	|      -------   -------
	|-------|      ------    -------
	| -20	|----->| -28 |-->| 10  |
	|	|      -------   ------
	|-------|      -------
	| 13	|----->|  100 |
	|	|      -------
	|-------|      -------   -------
	|10	|----->| -20 |-->| 100 |
	|	|      -------   -------
	|-------|      ------    -------   ------
	|100	|----->|  10 |-->| 13  |-->|     |
	|	|      ------    -------    ------
	--------

--> Things taken care
	1) Each process(Thread here), sets its edge
	2) Each process waits for some condition to know PID of all process at a time
	3) while exiting from process, it will free the appropriate link

*/
//--------------------------------------------------------------------
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
	List *array;
}Graph;
Graph *p=NULL;
int j=0;

int cond_met=0;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


#include"short_path.c"

//--------------------------------------------------------------------------------
void print_array(int *arr)
{
	int i;
	for(i=0;i<5;i++)
		printf(" %d ",arr[i]);
}
//---------------------------------------------------------------------------------
my_data* allocNode(int pid)
{
	my_data *temp=malloc(sizeof(my_data));
	temp->dest=pid;
	temp->src=0;
	temp->next=NULL;
	return temp;
}
//----------------------------------------------------------------------
void printGraph()
{
	int v;
	FILE *fp=fopen("jay.txt","w");
	for(v=0;v<p->V;v++)
	{
		my_data *my=p->array[PID_array[v]].head;
		printf("\nAdjacency list of %d ",PID_array[v]);
//		fprintf(fp,"%d",PID_array[v]);
		while(my)
		{
			printf(" --> %d",my->dest);
//			fprintf(fp,"-->%d",my->dest);
			my=my->next;
		}

		printf("\n");
	}
}

//---------------------------------------------------------------------------------
void addEdge(Graph *g,int src, int dest,int w)
{	
//		print_array(PID_array);
		int m_src=PID_array[src-1];
		int m_dest=PID_array[dest-1];
	//	printf(" %d  %d %d-------\n",m_src,m_dest,g);

		my_data  *newNode=allocNode(m_dest);
		newNode->weight=w;
		newNode->next= p->array[m_src].head;

		p->array[m_src].head=newNode;

		// our graph is unidirected
		
		newNode=allocNode(m_src);
		newNode->weight=w;
		newNode->next= p->array[m_dest].head;
		p->array[m_dest].head=newNode;
		printf("Edge added %d <--> %d\n",m_src,m_dest);
		printf("%d\n",newNode->pid);

}
//----------------------------------------------------------------------------------

void *CreateThread(void *pif)
{
	printf("\n\n");
//	Graph *p=(Graph *)pif;
	if(!p)
		{
		int n=rand()%10;
		while(n--);
		sleep(5);
		}
	if (pthread_mutex_lock(&mutex) != 0) {                                        
  	  	perror("pthread_mutex_lock() error");                                       
    		exit(6);
	}                                                                    
  
	int pid=pthread_self();
	printf("pid=%d, j= %d\n",pid,j);

	PID_array[j++]=pid%100;
	print_array(PID_array);
	while(!cond_met)
	{
	if (pthread_cond_wait(&cond, &mutex) != 0) {                                  
	    perror("pthread_cond_timedwait() error");                                   
	    exit(7);   
	}

//	print_array(PID_array);                                                                 
 	 } 
	
	sleep(rand()%10);
	//--------------------------------------------
/*	// When process is about to exit ,remove head entry from list
	int i;
	for (i=0;pid%100 ==PID_array[i];i++)	
		{
					
		free(p->array[PID_array[i]].head);
		}

*/	//---------------------------------------------
	if (pthread_mutex_unlock(&mutex) != 0) {                                   
                perror("pthread_mutex_lock() error");                            
                exit(6);
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
//----------------------------------------------------------------------------------

Graph* CreateGraph(int num_of_process,int  *V)  // list of process ID number
	{


		Graph *g=malloc(sizeof(Graph));
		g->V =num_of_process;
		// Create graph array
		g->array=malloc((g->V) * sizeof(List));
		
		//Initiallize each list 
		int i;
		for(i=0;i<(g->V);i++)
			{
			
		printf("\n%d ...graph\n",V[i]);
			g->array[V[i]].head=NULL;
			}
		sleep(10);
		return g;
	}

//---------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if(argc!=2)
	{
		printf("MSG: ./a.out NUMER_OF_PROCESS\n");
//		exit(0);
	}

//	int num=atoi(argv[1]);

	int num=5;
	pthread_t tid[num];
	int j=0,i,rec;
	int s;

	 if (pthread_mutex_init(&mutex, NULL) != 0) {                                  
	    perror("pthread_mutex_init() error");                                       
	    exit(1);                                                                    
	  }                                                                             
                                                                                
	  if (pthread_cond_init(&cond, NULL) != 0) {                                    
	    perror("pthread_cond_init() error");                                        
	    exit(2);                                                                    }   

	// File in that Edge and wight are 
//	FILE *fp=fopen("info.txt","r+");
//	update_file(fp);	
		
	PID_array=calloc(num,sizeof(int));
	
	for(i=0;i<num;i++)
		pthread_create(&tid[i],NULL,CreateThread,(void *)0);
	
	printf("\n");
	p=CreateGraph(num,PID_array);

	printf("j_out=%d num=%d\n",j,num);
	sleep(10);
//	if(j==num-1)
//		{
//		printf("checking..\n");

		cond_met=1;
	 if (pthread_cond_broadcast(&cond) != 0) {                                        

  	  	perror("pthread_cond_signal() error");    
		exit(4);                                                                       }




	FILE *fp=fopen("info.txt","r");
	char line[7],*tok;
	
	while(1)
	{
	int ret=fscanf(fp,"%s",line);
	
		puts(line);
		int src=line[0]- '0';
		int dest=line[2]- '0';
		int weight=line[4]- '0';
		printf("\n  src=%d  %d  %d \n",PID_array[src],dest,weight);
		addEdge(p,src,dest,weight);
		if(ret==EOF)
			break;
	}

		printGraph();                //  printing the whole valuw
//		dijkstra(p,0);
	if (pthread_mutex_unlock(&mutex) != 0) {                                   
                perror("pthread_mutex_lock() error");                            
                exit(6);
        }
	for(i=0;i<num;i++)
		{
		rec=pthread_join(tid[i],NULL);

		}

	pthread_exit(NULL);
	printf("\n");	
	return 0;	
}


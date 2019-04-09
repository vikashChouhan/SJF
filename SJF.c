#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<malloc.h>
/*
defined functions
*/

struct process
{
	int arrivalTime;
	int burstTime;
	int PID;
	
	int bstTime;
	int waitTime;
	int lastRun;
	int turnTime;
	float noWait;
};


void showHeading();
void get_input(struct process*,int);
void gotoxy(int x,int y);
void show_real_time_process(struct process* , int);

int main()
{
	
/*
Display Heading
*/
showHeading();

/*
Input the process information 
*/
	int noProcess;
	printf("\n\nEnter number of processes : ");
	scanf("%d",&noProcess);
	
	
	struct process *p_array =  (struct process *)malloc(sizeof(struct process)*noProcess);;
	if(p_array==NULL)
	{
		printf("\n...Memory Error...");
		return -1;
	}
	
	//function to get input
	get_input(p_array,noProcess);

	//schedule the processes and print the result
	printf("\nOrder of execution :\n");
	schedule_processes(p_array,noProcess);
	
	//showing result (waiting time, turnaround time)
	printf("\n\nProcess info : ");
	show_process_info(p_array,noProcess);
}

void show_process_info(struct process *p, int noProcess)
{
	int i = 0;
	float WaitSum = 0;
	for(i = 0;i<noProcess;i++)
	{
		printf("\n\n--------------------------------------------");
		printf("\nP%d",(p+i)->PID);
		
		(p+i)->waitTime = (p+i)->lastRun - (p+i)->burstTime;
		printf("\nWaiting Time : \t%d",(p+i)->waitTime);
		
		WaitSum += (p+i)->waitTime ;
		
		(p+i)->turnTime += (p+i)->waitTime ;
		printf("\nTurnaround Time : \t%d",(p+i)->turnTime);
	}
	printf("\n\nAvg. Waiting Time : \t%.2f",WaitSum/noProcess);
}
struct process *find_next_process(struct process *p,int time,int noProcess)
{
	//Functiont to find next process based on lowest burst time. It is simple linear search O(n)
	struct process *res = NULL;
	int minBrust = 10000;

	int i=0;
	for(i=0;i<noProcess;i++)
	{
		if((p+i)->arrivalTime<=time && (p+i)->bstTime<=minBrust && (p+i)->bstTime>0)
		{
			if(res!=NULL && res->bstTime==(p+i)->bstTime)
			{
				continue;			//ignoring process with same burst time
			}
			else
			{
				res = (p+i);
				minBrust = (p+i)->bstTime;
				
			}
		}
	}
	
	return res;
}
void schedule_processes(struct process *p,int noProcess)
{
	//function to schedulet the processes 
	
	int time=0;
	struct process* t;
	struct process* prev;
	int prev_time=1;
	int flag=1;
	int k=0;
	
	int j=0;
	time = p->arrivalTime;
	
	//loop to find process based on min arrival time
	for(j=0;j<noProcess;j++)
		if(time>(p+j)->arrivalTime)
			time = (p+j)->arrivalTime;
			
	for(; ; time++)
	{
	
		k++;
		t = find_next_process(p,time,noProcess);
		
		if(t!=NULL)
		{
			t->bstTime -=1;
			
			if(time>1 && prev->PID==t->PID)
			{
				flag=0;
				
				t->lastRun = time+1;
				continue;
			}
			else
			{
				
				if(k!=1)
					{
						printf("[%d - %d] -> (2AT) P%d",prev_time,time,t->PID);
						
					}
				else
					printf("(2AT) P%d",t->PID);
				
				
				
				time+=2;
				t->lastRun  = time+1;	
				prev_time = time;
				prev = t;
			
				
				if(t->bstTime!=0)
					t->noWait+=1;
					
			}	
		}
		else
		{
			printf("[%d - %d]",prev_time,time);
			break;
		}
		
	}
}


void get_input(struct process *p,int noProcess)
{
	
	
	printf("\nEnter Process information ");
	printf("\n\n\tProcessID\t\tArrival Time\t\tBurst Time\n");
	gotoxy(8,8);
	printf("---------------------------------------------------------");
	

	int depth = 9;
	int i =0;
	for(i=0;i<noProcess;i++)
	{
		gotoxy(12,depth);
		
		printf("%d",i+1);
		(p+i)->PID = (i+1);
		gotoxy(35,depth);
		scanf("%d",&(p+i)->arrivalTime);
		if((p+i)->arrivalTime==0)
			{
				printf("\nError Arrival Time cannot be zero...");
				exit(0);
			}
		gotoxy(60,depth);
		scanf("%d",&(p+i)->burstTime);
		
		(p+i)->bstTime = (p+i)->burstTime;
		(p+i)->turnTime = (p+i)->burstTime;
		(p+i)->waitTime = 0;
		(p+i)->lastRun = (p+i)->arrivalTime;
		(p+i)->noWait = 0;
		depth+=1;
	}
	
}

void showHeading()
{
	printf("\t\t S H O R T E S T \t J O B \t\t F I R S T\n");
	printf("\t\t--------------------------------------------------- ");
}

void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(  GetStdHandle(STD_OUTPUT_HANDLE) , c);
}



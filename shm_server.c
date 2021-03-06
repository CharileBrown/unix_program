#include "common.h"

struct msg_form
{
	long mtype;
	char mtext;
};

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int init_sem(int sem_id,int value)
{
	union semun tmp;
	tmp.val = value;
	if( semctl(sem_id,0,SETVAL,tmp)==-1 )
	{
		perror("Init Semaphore Error");
		return -1;
	}
	return 0;
}

int sem_p(int sem_id)
{
	struct sembuf sbuf;
	sbuf.sem_num = 0;
	sbuf.sem_op = -1;
	sbuf.sem_flg = SEM_UNDO;
	if( semop(sem_id,&sbuf,1) == -1 )
	{
		perror("Semaphore Action P Error\n");
		return -1;
	}
	return 0;
}

int sem_v(int sem_id)
{
	struct sembuf sbuf;
	sbuf.sem_num = 0;
	sbuf.sem_op = 1;
	sbuf.sem_flg = SEM_UNDO;
	if( semop(sem_id,&sbuf,1) == -1 )
	{
		perror("Semaphore Action V Error\n");
		return -1;
	}	
	return 0;
}

int del_sem(int sem_id)
{
	union semun tmp;
	if(semctl(sem_id,0,IPC_RMID,tmp)==-1)
	{
		perror("Delete Semaphore Error");
		return -1;
	}
	return 0;
}

int creat_sem(key_t key)
{
	int sem_id;
	if( (sem_id = semget(key,1,IPC_CREAT|0666)) == -1 )
	{
		perror("semget error");
		exit(-1);
	}
	init_sem(sem_id,1);
	return sem_id;
}

int main()
{
	key_t key;
	int shmid,semid,msqid;
	char *shm;
	char data[] = "this is server";
	struct shmid_ds buf1;
	struct msqid_ds buf2;
	struct msg_form msg;

	if( (key = ftok("./test.txt",256)) <0 )
=======
#include "unix.h"


int main()
{
	int key_t key;
	if( (key = ftok(".",'z')) < 0 )
>>>>>>> 08f859ad510fdcc522baaa32eece87567a7c5057
	{
		perror("ftok error");
		exit(1);
	}
<<<<<<< HEAD
	if((shmid = shmget(key,1024,IPC_CREAT|0666)) == -1 )
	{
		perror("Create Shared Memory Error");
		exit(1);
	}
	shm = (char*)shmat(shmid,0,0);
	if( (long)shm == -1 )
	{
		perror("Attach Shared Memory Error");
		exit(1);
	}
	msqid = msgget(key,IPC_CREAT|0777);
	if( msqid == -1 )
	{
		perror("Msgget Error");
		exit(1);
	}
	semid = creat_sem(key);
	while(1)
	{
		msgrcv(msqid,&msg,1,888,0);
		if( msg.mtext == 'q' )
			break;
		if( msg.mtext == 'r' )
		{
			sem_p(semid);
			printf("%s\n",shm);
			sem_v(semid);
		}
	}
	shmdt(shm);
	shmctl(shmid,IPC_RMID,&buf1);
	msgctl(msqid,IPC_RMID,&buf2);
	del_sem(semid);
	return 0;
	
}

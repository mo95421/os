#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semid1,semid2; //两个信号量

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
	};
	
void p1(int semid1){
	struct sembuf op1[1];
	op1[0].sem_num = 0;
	op1[0].sem_op = -1; //p进行-1操作
	op1[0].sem_flg = 0;
	semop(semid1,op1,1);
}

void p2(int semid2){
	struct sembuf op2[1];
	op2[0].sem_num = 0;
	op2[0].sem_op = -1; //p进行-1操作
	op2[0].sem_flg = 0;
	semop(semid2,op2,1);
}

void v1(int semid1){
	struct sembuf op1[1];
	op1[0].sem_num = 0;
	op1[0].sem_op = 1; //p进行-1操作
	op1[0].sem_flg = 0;
	semop(semid1,op1,1);
}

void v2(int semid2){
	struct sembuf op2[1];
	op2[0].sem_num = 0;
	op2[0].sem_op = 1; //p进行-1操作
	op2[0].sem_flg = 0;
	semop(semid2,op2,1);
}

void driver(){
	while(1){
		printf("driver drive the bus\n");
		v2(semid2);
		p1(semid1);
		printf("the bus leave\n");
		sleep(1);
	}
}

void conductor(){
	while(1){
		printf("conductor sell tickets\n");
		p2(semid2);
		printf("conductor open the door\n");
		printf("passanger get in/out the bus\n");
		printf("conductor close the door\n");
		v1(semid1);
		sleep(1);
	}
}

int main(){
	key_t key1,key2;//定义信号量的key值
	union semun v1,v2;
	int r1,r2;
	key1 = ftok(".",36);
	key2 = ftok(".",98);
	if(key1 == -1){
		printf("key1 get error\n");
		exit(-1);
	}
	if(key2 == -1){
		printf("key2 get error\n");
		exit(-1);
	}
	//获得信号量
	semid1 = semget(key1,1,IPC_CREAT|IPC_EXCL|0666);
	semid2 = semget(key2,1,IPC_CREAT|IPC_EXCL|0666);
	if(semid1 == -1){
		printf("semid1 get error\n");
		exit(-1);
	}
	if(semid2 == -1){
		printf("semid2 get error\n");
		exit(-1);
	}
	//初始化信号量
	v1.val = 0;
	v2.val = 0;
	r1 = semctl(semid1,0,SETVAL,v1);
	r2 = semctl(semid2,0,SETVAL,v2);
	if(r1 == -1){
		printf("r1 get error\n");
		exit(-1);
	}
	if(r2 == -1){
		printf("r2 get error\n");
		exit(-1);
	}
	if(fork()>0){ //父进程
		driver();
	}else{        //子进程
		conductor();
	}

	semctl(semid1,0,IPC_RMID);
	semctl(semid2,0,IPC_RMID);
	
	return 0;
}

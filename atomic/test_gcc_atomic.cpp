#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lhf.h"
////////////////////variables////////////////////
pthread_mutex_t gmutex;
long long gnum=1000;
////////////////////functions////////////////////
void* work_thread(void* arg){
	long long now_val;
	bool atomic;
	while((now_val=gnum)>0){
		atomic=__sync_bool_compare_and_swap(&gnum,now_val,now_val-1);
		if(atomic){
			//ok,work here
			//long long next_val=now_val-1
		}
	}
	return NULL;
}
////////////////////main////////////////////
int main(int argc,char* argv[]){
	int thread_num=100;
	int re;
	clock_t begin,end,use;
	if(argc>1){
		thread_num=atoi(argv[1]);
	}
	if(argc>2){
		gnum=strtoll(argv[2],NULL,10);
	}
	begin=clock();
	pthread_t *threads=(pthread_t*)calloc(thread_num,sizeof(pthread_t));
	pthread_mutex_init(&gmutex,NULL);
	for(int i=0;i<thread_num;i++){
		if(pthread_create(threads+i,NULL,work_thread,(void *)(long)i)<0){
			print_error(1,"pthread_create",strerror(errno));
		}
	}
	void *exist_status;
	for(int i=0;i<thread_num;i++){
		if((re=pthread_join(threads[i],&exist_status))<0){
			print_error(1,"pthread_create",strerror(errno));
		}
	}
	pthread_mutex_destroy(&gmutex);
	free(threads);
	end=clock();
	use=end-begin;
	printf("in main:%lld use %ld second(%ld)\n",gnum,use/CLOCKS_PER_SEC,use);
	return 0;
}

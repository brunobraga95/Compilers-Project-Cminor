#include<stdio.h>
#include<stdlib.h>
int used[16] = {1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0};
const char *register_name(int r){
	if(r == 0)return "%rax";
	if(r == 1)return "%rbx";
	if(r == 2)return "%rcx";
	if(r == 3)return "%rdx";
	if(r == 4)return "%rsi";
	if(r == 5)return "%rdi";
	if(r == 6)return "%rbp";
	if(r == 7)return "%rsp";
	if(r == 8)return "%r8";
	if(r == 9)return "%r9";
	if(r == 10)return "%r10";
	if(r == 11)return "%r11";
	if(r == 12)return "%r12";
	if(r == 13)return "%r13";
	if(r == 14)return "%r14";
	if(r == 15)return "%r15";
	return "";

}

int register_alloc(){
	int i;
	for(i= 0;i<=15;i++){
		if(used[i] == 0){
			used[i] = 1;
			return i;
		}
	}
	printf("sorry, we ran out of registers :(\n");
	exit(0);	
	return -1;
	
}
void register_free(int r){
	used[r] = 0;
	used[0] = 1;
	used[2] = 1;	
	used[3] = 1;
	used[4] = 1;
	used[5] = 1;
	used[6] = 1;
	used[7] = 1;
	used[8] = 1;
	used[9] = 1;
}








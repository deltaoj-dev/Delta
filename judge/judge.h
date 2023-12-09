// This header provides judge utilities.
// Copyleft 2023 Delta. All wrongs reserved.
#pragma once
#ifndef DELTA_JUDGE_H
#define DELTA_JUDGE_H
#include<sched.h>
#include<stdarg.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/sched.h>
#include<sys/syscall.h>
#ifdef __cplusplus
extern"C"{
#endif
// Command must be shorter than 1000.
int systemf(const char*format,...)
{
	char cmd[1001]="";
	va_list args;
	va_start(args,format);
	vsprintf(cmd,format,args);
	va_end(args);
	return system(cmd);
}
/**
 * It will print the compiling logs if exist.
 * All the file paths must exist and be shorter than 128.
 * @param code Path to the code.
 * @param language Language and language options. (TODO)
 * @param data Path to the data directory.
 * @return -1 if failed. Otherwise the point.
 */
int judge(const char*code,const char*language,const char*data)
{
	FILE*f=popen("mktemp -d /tmp/Delta_XXXXXXXX","r");
	char dn[20]="";
	fread(dn,1,19,f);
	pclose(f);
	systemf("cp %s %s/source.cpp &> /dev/null",code,dn);
	if(systemf("g++ -lm -std=c++14 -static %s/source.cpp",dn))
		return -1;
	char json[143]="";
	sprintf(json,"%s/data.json",data);
	f=fopen(json,"r");
	fscanf(f,"[");
	int tot=0;
	while(1)
	{
		char in[131],out[131],b[3];
		int pt;
		if(fscanf(f," [ \"%[^\"]\" , \"%[^\"]\" , %d ] %s",in,out,&pt,b)<4)
			break;
		uint64_t args[11];
		args[0]=CLONE_INTO_CGROUP;// TODO, requires Linux 5.7
		syscall(SYS_clone3,args,11);
		if(b[0]==']')
			break;
	}
	fclose(f);
	if(systemf("rm -r %s &> /dev/null",dn))
		return -1;
	return tot;
}
#ifdef __cplusplus
}
#endif
#endif // DELTA_JUDGE_H

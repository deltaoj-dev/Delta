// This header provides judge utilities.
// Copyleft 2023 Delta. All wrongs reserved.
#pragma once
#ifndef DELTA_JUDGE_H
#define DELTA_JUDGE_H
#include<linux/sched.h>
#include<sched.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<unistd.h>
// Command must be shorter than 1000.
int systemf(const char*format,...)
{
	char cmd[1001];
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
 * @param data Path to the JSON file to describe the data.
 * @return -1 if failed. Otherwise the point.
 */
int judge(const char*code,const char*language,const char*data)
{
	FILE*f=popen("mktemp -d /tmp/Delta_XXXXXXXX","r");
	char dn[20]={0,[19]=0};
	fread(dn,1,19,f);
	pclose(f);
	systemf("cp %s %s/source.cpp &> /dev/null",code,dn);
	if(systemf("g++ -lm -std=c++14 -static %s/source.cpp -o %s/source",dn,dn))
		return -1;
	f=fopen(data,"r");
	char b[3]="\0\0\0";
	fscanf(f,"{");
	while(1)
	{
		char in[131],out[131];
		if(fscanf(f," \"%[^\"]\" : \"%[^\"]\" %s",in,out,b)<3)
			break;
		//syscall(SYS_clone3); // TODO
		if(b[0]=='}')
			break;
	}
	fclose(f);
	if(systemf("rm -r %s &> /dev/null",dn))
		return -1;
}
#endif // DELTA_JUDGE_H
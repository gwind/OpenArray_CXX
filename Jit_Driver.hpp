#ifndef __JIT_DRIVER_HPP__
#define __JIT_DRIVER_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <functional>
#include "jit/Jit.hpp"

using namespace std;

typedef void fusion_kernel_rawptr (void**&, int);
typedef void (*kernel_func)(void**&, int);
typedef std::function<fusion_kernel_rawptr> FusionKernelPtr;

typedef unordered_map<size_t, FusionKernelPtr> FKPtrMap;

class Jit_Driver{
private:
  FKPtrMap kernel_dict;

public:
  FusionKernelPtr get(size_t hash) {
    if (kernel_dict.find(hash) == kernel_dict.end()) return NULL;
    return kernel_dict[hash];
  }

  void insert(size_t hash, const stringstream& code) {
    FusionKernelPtr fk_ptr = get(hash);
    if (fk_ptr != NULL) return ;

    int fack_argc = 4;
    char arg0[] = "-O3" ;
    char arg1[] = "-O3";
    char arg2[] = "-O3";
    char arg3[]= ""; //"-I/home/wangdong/comp/llvm.debug/lib/clang/6.0.0/include";
    char **fake_argv = new char *[fack_argc+1]{arg0, arg1, arg2, arg3};
    //char code_char[] = code.str().c_str();
    stringstream ss;
    ss<<"kernel_"<<hash;
    //char kernel_name[] = ss.str().c_str();

	const string& scode = code.str();  
	const char* cccode = scode.c_str(); 
	//printf("code=\n-> \n%s \n<-\n",cccode);
	const string& sname = ss.str();  
	const char* ccname = sname.c_str(); 
	//printf("name=\n-> \n%s \n<-\n",ccname);

	char *ccode = new char[strlen(cccode)+1];
	char *cname = new char[strlen(ccname)+1];
	strcpy(ccode, cccode);
	strcpy(cname, ccname);

    Jit ji(fack_argc, fake_argv, cname, ccode);
    uint64_t Entry = ji.compile();
    
    fk_ptr = (kernel_func)Entry;
    kernel_dict[hash] = fk_ptr;

	delete []ccode;
	delete []cname;

    return ;
  }

  static Jit_Driver* global() {
    static Jit_Driver jit;
    return &jit;
  }
};

#endif

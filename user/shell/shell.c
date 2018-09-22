#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SHELL_CMD_NUM 17
int SHELL_CALL_FUNTION(void *addr,int argc,char *argv[])
{
	uint32_t ret;
	__asm__ __volatile__("push %%esi;push %%eax;call *%k1;\
	addl $0x8,%%esp":"=a"(ret):"r"(addr),"a"(argc),"S"(argv));
	return ret;
}


struct command {
  char *name;
  void *fun;
  char *help;
};
extern struct command cmd_table[];

char *pwd;
char *_pwd (char *pwd)
{

	return pwd;
}


int cmd_help(int argc,char **argv)
{
        int i;
        printf("Commands:\n");
        for(i=0;i< SHELL_CMD_NUM;i++){
        printf("%s",cmd_table[i].name);
        asm volatile("int $0x69"::"a"(38),"d"(20),"c"(0));
        printf("%s\n",cmd_table[i].help);

        }

        return 0;

}

//cd ..
static char *cdpp(const char *path) 
{
	int i =strlen(path) -1; // 0
	do {
		(*(char*)(path +i)) = 0;
		if((*(char*)(path +i-1)) =='/')break;

	}while(i--);

	return (char*)path;
}

int cmd_cd(int argc,char *argv[])
{

	int ret = 0, len =0;
	int z0[256*16];
	char *path = (char*)(z0);
	char (*arg)[256] =(char*) argv;

	char *io= (char *)0x400000;
	uint32_t cr3;

	if(argc!=2) {
		printf ("cmd: cd: %s+%s...: No such file or directory\n",arg[1],arg[2]);
		return 1;
	}


	if(!(strcmp (arg[1],".")))return 0;
	else if(!(strcmp (arg[1],".."))) {
		if(strcmp(_pwd(pwd),"d0:/"))cdpp(_pwd(pwd));
		return 0;
		
	}
	else {

		
		
		memset(path,0,256*16);
		strcpy(path,_pwd(pwd));
		strcat(path,arg[1]);
		//asm volatile("int $0x69":"=a"(ret):"a"(31),"d"(path));

		asm volatile("int $0x69":"=a"(cr3):"a"(5));

		while((*(uint32_t*)(io +0x04))); // wait cr

		(*(uint32_t*)(io +0x0C)) = cr3;  // PDBR
		(*(uint32_t*)(io +0x28)) = (uint32_t)path; // arg1

		(*(uint32_t*)(io +0x08)) = 1; // int
		(*(uint32_t*)(io +0x04)) = 2; // sms cd

		while((*(uint32_t*)(io +0x08))); // wait int

		ret = (*(uint32_t*)(io +0x24)); // ret

		if(!ret) {
			strcat(_pwd(pwd),arg[1]);
			len = strlen(_pwd(pwd));
			path = (char*)_pwd(pwd);

			if((*(char*)(path + len -1)) != '/') {

				(*(char*)(path + len)) = '/';
				(*(char*)(path + len +1)) = '\0';
			} 
			
			
		}else {
		
			printf ("cmd: cd: %s: No such file or directory\n",arg[1]);
			return 1;
		}

	}
	
    	return 0;
}

int cmd_cls(int argc,char *argv[])
{
    cls();
    return 0;
}

int cmd_copy(int argc,char *argv[])
{

    return 0;
}

int cmd_date(int argc,char *argv[])
{
    return 0;

}


int cmd_del(int argc,char *argv[])
{

    return 0;
}


int cmd_dir(int argc,char *argv[])
{

	char *io= (char *)0x400000;
	uint32_t cr3;
	char (*arg)[256] =argv;
		if(argc!=1) {
		printf ("cmd: dir: %s: No such file or directory\n",arg[1]);
		return 1;
	}


	asm volatile("int $0x69":"=a"(cr3):"a"(5));

	while((*(uint32_t*)(io +0x04))); // wait cr

	(*(uint32_t*)(io +0x0C)) = cr3;  // PDBR
	(*(uint32_t*)(io +0x28)) =(uint32_t)_pwd(pwd); // arg1

	(*(uint32_t*)(io +0x08)) = 1; // int 
	(*(uint32_t*)(io +0x04)) = 1; // sms rdir

	while((*(uint32_t*)(io +0x08))); // wait int
	
	//asm volatile("int $0x69"::"a"(30),"d"(_pwt(pwt)));

	if((*(uint32_t*)(io +0x24)))printf ("cmd: dir: %s: No such file or directory\n",_pwd(pwd));
	else putchar('\n');
    	return  (*(uint32_t*)(io +0x24));
}

int cmd_echo(int argc,char *argv[])
{
    return 0;
}
int cmd_exit(int argc,char *argv[])
{
    printf("Sirius\n");
    return 0;
}
int cmd_mov(int argc,char *argv[])
{

    return 0;
}
int cmd_new(int argc,char *argv[])
{

    return 0;
}


int cmd_reboot(int argc,char *argv[])
{
    // This syscall reboot system.
    __asm__ __volatile__("int $0x69"::"a"(6)); 
    return 0;
}
int cmd_rename(int argc,char *argv[])
{
    printf("Function not implemented\n");
    return 0;
}



int cmd_shutdown(int argc,char *argv[])
{
    printf("Too much claw, the road is long.\n");
    return 0;

}
int cmd_time(int argc,char *argv[])
{
    return 0;

}
int cmd_version(int argc,char *argv[])
{
    //printf("Kernel Dev Standard, bash version 0.01 - 2018.24.4 (Khole OS version 0.3)\n");
    printf("Sirius Operating System v1.00\n");
    return 0;
}




struct command cmd_table[] = {
    {"?",           cmd_help,           "This help"                                     },
    {"cd",          cmd_cd,             "Change current directory"                      },
    {"cls",         cmd_cls,            "Clear screen"                                  },
    {"copy",        cmd_copy,           "Copy file or directory"                        },
    {"date",        cmd_date,           "Date"                                          },
    {"del",         cmd_del,            "Delete file or directory"                      },
    {"dir",         cmd_dir,            "List directory"                                },
    {"echo",        cmd_echo,           "This ---"                                      },
    {"exit",        cmd_exit,           "Exit shell"                                    },
    {"help",        cmd_help,           "This help"                                     },
    {"mov",         cmd_mov,            "Move file or directory"                        },
    {"new",         cmd_new,            "New file or directory"                         },
    {"reboot",      cmd_reboot,         "Reboot system"                                 },
    {"rename",      cmd_rename,         "Rename file or directory"                      },
    {"shutdown",    cmd_shutdown,       "Shutdown your computer locally or remotely"    },
    {"time",        cmd_time,           "Time"                                          },
    {"version",     cmd_version,        "Shell version"                                 },
};


int strarg (char *argv[],char *b)
{

	char (*a)[256] =(char**)(argv);
	int i = 0, t =0;

	char *p = b;

	while(true) {

		while(true) {

			if(*p == '\0')break;
			a[i][t++] = *p++;
			if(*p == ' ')break;
		}

		if(*p == '\0')break;

		while(*p++ != ' ');
		t = 0;
		i++;	

	}	

	return i +1;
}

int main(int argc,char *argv[])
{
    int i;
    char cmd_buffer[256];
    int z0[(32*256)/4];
    char (*_argv)[256] = (char**)(&z0);
	
    int z1[(256*32)/4];
    pwd = (char*)(z1);
    
    strcpy(pwd,"d0:/Users/Default/\0");

  //  cls();

	char cmdname[256];
	int _argc = 0;

    while(true)
    {

	memset((char**)_argv,0,32*256);
        color(0x2);
        printf("%s:>",pwd);
        color(0xff);
        gets(cmd_buffer);

	strtok(cmdname,cmd_buffer,' ');
	_argc = strarg ((char**)_argv,cmd_buffer);

        for(i=0;i < SHELL_CMD_NUM;i++)
        {
            if((strcmp(cmdname,cmd_table[i].name)) == 0)
            {
                SHELL_CALL_FUNTION((uint32_t*)cmd_table[i].fun,_argc,(char**)_argv);
                break;
            }
            else if(i == (SHELL_CMD_NUM - 1))
            printf("%s: command not found\n",cmd_buffer);
        }

    

    }



    return 0;
}

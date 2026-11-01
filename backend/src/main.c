#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <windows.h>
static volatile sig_atomic_t stop=0;
static void stop_server(int sig){(void)sig;stop=1;}
int main(int argc,char **argv) {
 const char *env=getenv("APP_ENV"),*dev=getenv("ENABLE_DEV_AUTH"),*port=getenv("PORT"),*db=getenv("DB_PATH");
 char address[64],*end;long p;App app;struct mg_context *ctx;int seed=0;
 if(!env)env="production";if(!dev)dev="0";if(!port)port="8080";if(!db)db="data/campus.db";
 if(strcmp(env,"development") && strcmp(env,"production")){fprintf(stderr,"Invalid APP_ENV\n");return 2;}
 if(strcmp(dev,"0") && strcmp(dev,"1")){fprintf(stderr,"ENABLE_DEV_AUTH must be 0 or 1\n");return 2;}
 if(!strcmp(dev,"1") && strcmp(env,"development")){fprintf(stderr,"Development auth is forbidden outside development\n");return 2;}
 p=strtol(port,&end,10);if(*end || p<1024 || p>65535){fprintf(stderr,"Invalid PORT\n");return 2;}
 if(argc>1 && !strcmp(argv[1],"--seed-dev"))seed=1;
 else if(argc>1 && strcmp(argv[1],"--migrate")){fprintf(stderr,"Use --migrate or --seed-dev\n");return 2;}
 if(seed && strcmp(env,"development")){fprintf(stderr,"Development seed is forbidden\n");return 2;}
 if(!db_initialize(db,seed)){fprintf(stderr,"Database initialization failed; check directory and migrations\n");return 1;}
 if(argc>1){printf("Database initialized successfully\n");return 0;}
 app.db_path=db;app.dev_auth=!strcmp(env,"development")&&!strcmp(dev,"1");
 snprintf(address,sizeof(address),"127.0.0.1:%ld",p);
 {const char *options[]={"listening_ports",address,"document_root","frontend","enable_directory_listing","no","num_threads","4","request_timeout_ms","5000",NULL};
 mg_init_library(0);ctx=mg_start(NULL,NULL,options);}
 if(!ctx){fprintf(stderr,"Cannot start server (port may be occupied)\n");return 1;}
 mg_set_request_handler(ctx,"/api",api_handler,&app);
 signal(SIGINT,stop_server);signal(SIGTERM,stop_server);
 printf("B1-1 local server: http://%s | dev auth: %s\n",address,app.dev_auth?"ON (local test identities only)":"OFF");fflush(stdout);
 while(!stop)Sleep(100);
 mg_stop(ctx);mg_exit_library();return 0;
}

#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int execute_file(sqlite3 *db, const char *path) {
 FILE *f=fopen(path,"rb"); long size; char *sql; int rc;
 if(!f) {fprintf(stderr,"Cannot read %s\n",path);return 0;}
 fseek(f,0,SEEK_END);size=ftell(f);rewind(f);
 if(size<0 || size>1024*1024){fclose(f);return 0;}
 sql=malloc((size_t)size+1);if(!sql){fclose(f);return 0;}
 if(fread(sql,1,(size_t)size,f)!=(size_t)size){free(sql);fclose(f);return 0;}
 fclose(f);sql[size]=0;rc=sqlite3_exec(db,sql,0,0,0);free(sql);
 if(rc!=SQLITE_OK)fprintf(stderr,"SQL error: %s\n",sqlite3_errmsg(db));
 return rc==SQLITE_OK;
}
int db_initialize(const char *path,int seed) {
 sqlite3 *db=0;sqlite3_stmt *q=0;int version=0,ok=0;
 if(sqlite3_open(path,&db)!=SQLITE_OK)goto done;
 sqlite3_busy_timeout(db,3000);
 if(sqlite3_exec(db,"PRAGMA foreign_keys=ON; BEGIN IMMEDIATE;",0,0,0)!=SQLITE_OK)goto done;
 if(sqlite3_prepare_v2(db,"PRAGMA user_version",-1,&q,0)!=SQLITE_OK)goto rollback;
 if(sqlite3_step(q)==SQLITE_ROW)version=sqlite3_column_int(q,0);
 sqlite3_finalize(q);q=0;
 if(version>1){fprintf(stderr,"Database schema is newer than this application\n");goto rollback;}
 if(version==0){
  if(!execute_file(db,"backend/migrations/001_identity.sql"))goto rollback;
  if(sqlite3_exec(db,"PRAGMA user_version=1",0,0,0)!=SQLITE_OK)goto rollback;
 }
 if(seed && !execute_file(db,"backend/seeds/development.sql"))goto rollback;
 ok=sqlite3_exec(db,"COMMIT",0,0,0)==SQLITE_OK;goto done;
 rollback: sqlite3_exec(db,"ROLLBACK",0,0,0);
 done: if(q)sqlite3_finalize(q);if(db)sqlite3_close(db);return ok;
}
int db_user(const char *path,const char *id,User *out) {
 sqlite3 *db=0;sqlite3_stmt *q=0;int found=0;
 if(sqlite3_open_v2(path,&db,SQLITE_OPEN_READONLY,0)!=SQLITE_OK)goto done;
 sqlite3_busy_timeout(db,3000);
 if(sqlite3_prepare_v2(db,"SELECT id,role,campus_verified FROM users WHERE id=?",-1,&q,0)!=SQLITE_OK)goto done;
 sqlite3_bind_text(q,1,id,-1,SQLITE_TRANSIENT);
 if(sqlite3_step(q)==SQLITE_ROW){
  snprintf(out->id,sizeof(out->id),"%s",sqlite3_column_text(q,0));
  snprintf(out->role,sizeof(out->role),"%s",sqlite3_column_text(q,1));
  out->verified=sqlite3_column_int(q,2);found=1;
 }
 done: if(q)sqlite3_finalize(q);if(db)sqlite3_close(db);return found;
}

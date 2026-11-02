#include "app.h"
#include <string.h>
int json_reply(struct mg_connection *c,int status,const char *body) {
 const char *reason=status==200?"OK":status==401?"Unauthorized":status==403?"Forbidden":status==404?"Not Found":status==405?"Method Not Allowed":"Error";
 mg_printf(c,"HTTP/1.1 %d %s\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length: %lu\r\nCache-Control: no-store\r\nX-Content-Type-Options: nosniff\r\n%s\r\n",status,reason,(unsigned long)strlen(body),status==405?"Allow: GET\r\n":"");
 mg_write(c,body,strlen(body));return status;
}
int api_error(struct mg_connection *c,int status,const char *code,const char *message) {
 char *body=sqlite3_mprintf("{\"error\":{\"code\":\"%s\",\"message\":\"%s\"}}",code,message);
 int result=json_reply(c,status,body);sqlite3_free(body);return result;
}

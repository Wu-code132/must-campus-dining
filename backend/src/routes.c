#include "app.h"
#include <stdio.h>
#include <string.h>
int api_handler(struct mg_connection *c,void *data) {
 App *app=data;User user;char body[256];
 const struct mg_request_info *r=mg_get_request_info(c);const char *uri=r->local_uri;
 if(strcmp(r->request_method,"GET"))return api_error(c,405,"METHOD_NOT_ALLOWED","Use GET");
 if(!strcmp(uri,"/api/health"))return json_reply(c,200,"{\"status\":\"ok\",\"milestone\":\"B1-1\"}");
 if(!strcmp(uri,"/api/me")){
  if(!require_user(c,app,&user))return 1;
  snprintf(body,sizeof(body),"{\"id\":\"%s\",\"role\":\"%s\",\"campusVerified\":%s}",user.id,user.role,user.verified?"true":"false");
  return json_reply(c,200,body);
 }
 if(!strcmp(uri,"/api/admin/ping")){
  if(!require_staff(c,app,&user))return 1;
  return json_reply(c,200,"{\"message\":\"Staff permission verified\"}");
 }
 return api_error(c,404,"NOT_FOUND","Endpoint not implemented");
}

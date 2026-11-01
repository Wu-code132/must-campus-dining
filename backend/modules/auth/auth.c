#include "app.h"
#include <string.h>
int require_user(struct mg_connection *c,App *app,User *out) {
 const char *id=mg_get_header(c,"X-Dev-User");
 const char *remote=mg_get_request_info(c)->remote_addr;
 /* Replace this adapter with verified sessions in B1-3. Never trust a role header. */
 if(!app->dev_auth || strcmp(remote,"127.0.0.1")!=0 || !id ||
   (strcmp(id,"user-demo-001") && strcmp(id,"user-demo-002") && strcmp(id,"staff-demo-001")) ||
   !db_user(app->db_path,id,out)){
  api_error(c,401,"UNAUTHENTICATED","Authentication required");return 0;
 }
 if(!out->verified){api_error(c,403,"CAMPUS_VERIFICATION_REQUIRED","Campus verification required");return 0;}
 return 1;
}
int require_staff(struct mg_connection *c,App *app,User *out) {
 if(!require_user(c,app,out))return 0;
 if(strcmp(out->role,"canteen_staff")){api_error(c,403,"FORBIDDEN","Staff role required");return 0;}
 return 1;
}

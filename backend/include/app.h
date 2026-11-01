#ifndef CAMPUS_APP_H
#define CAMPUS_APP_H
#include "civetweb.h"
#include "sqlite3.h"
typedef struct { char id[64]; char role[32]; int verified; } User;
typedef struct { const char *db_path; int dev_auth; } App;
int db_initialize(const char *path, int seed);
int db_user(const char *path, const char *id, User *out);
int json_reply(struct mg_connection *c, int status, const char *body);
int api_error(struct mg_connection *c, int status, const char *code, const char *message);
int require_user(struct mg_connection *c, App *app, User *out);
int require_staff(struct mg_connection *c, App *app, User *out);
int api_handler(struct mg_connection *c, void *data);
#endif

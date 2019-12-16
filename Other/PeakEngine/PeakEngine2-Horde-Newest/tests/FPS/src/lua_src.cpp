/*
** Lua binding: peak
** Generated automatically by tolua++-1.0.92 on Mon Jun  1 15:37:30 2009.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_peak_open (lua_State* tolua_S);

#include "Player.h"
#include "Weapon.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_WeaponEntity (lua_State* tolua_S)
{
 WeaponEntity* self = (WeaponEntity*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_PlayerEntity (lua_State* tolua_S)
{
 PlayerEntity* self = (PlayerEntity*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"peak::Entity");
 tolua_usertype(tolua_S,"PlayerEntity");
 tolua_usertype(tolua_S,"WeaponEntity");
 tolua_usertype(tolua_S,"peak::Buffer");
}

/* method: new of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_new00
static int tolua_peak_PlayerEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PlayerEntity* tolua_ret = (PlayerEntity*)  new PlayerEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"PlayerEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_new00_local
static int tolua_peak_PlayerEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PlayerEntity* tolua_ret = (PlayerEntity*)  new PlayerEntity();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"PlayerEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_delete00
static int tolua_peak_PlayerEntity_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerEntity* self = (PlayerEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_load00
static int tolua_peak_PlayerEntity_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Buffer",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerEntity* self = (PlayerEntity*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(name,data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: update of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_update00
static int tolua_peak_PlayerEntity_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerEntity* self = (PlayerEntity*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'",NULL);
#endif
  {
   self->update(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMovement of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_setMovement00
static int tolua_peak_PlayerEntity_setMovement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerEntity* self = (PlayerEntity*)  tolua_tousertype(tolua_S,1,0);
  int forward = ((int)  tolua_tonumber(tolua_S,2,0));
  int back = ((int)  tolua_tonumber(tolua_S,3,0));
  int left = ((int)  tolua_tonumber(tolua_S,4,0));
  int right = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMovement'",NULL);
#endif
  {
   self->setMovement(forward,back,left,right);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMovement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  PlayerEntity */
#ifndef TOLUA_DISABLE_tolua_peak_PlayerEntity_setRotation00
static int tolua_peak_PlayerEntity_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlayerEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlayerEntity* self = (PlayerEntity*)  tolua_tousertype(tolua_S,1,0);
  float yaw = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(yaw);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  WeaponEntity */
#ifndef TOLUA_DISABLE_tolua_peak_WeaponEntity_new00
static int tolua_peak_WeaponEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"WeaponEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   WeaponEntity* tolua_ret = (WeaponEntity*)  new WeaponEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"WeaponEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  WeaponEntity */
#ifndef TOLUA_DISABLE_tolua_peak_WeaponEntity_new00_local
static int tolua_peak_WeaponEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"WeaponEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   WeaponEntity* tolua_ret = (WeaponEntity*)  new WeaponEntity();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"WeaponEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  WeaponEntity */
#ifndef TOLUA_DISABLE_tolua_peak_WeaponEntity_delete00
static int tolua_peak_WeaponEntity_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WeaponEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WeaponEntity* self = (WeaponEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  WeaponEntity */
#ifndef TOLUA_DISABLE_tolua_peak_WeaponEntity_load00
static int tolua_peak_WeaponEntity_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WeaponEntity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Buffer",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WeaponEntity* self = (WeaponEntity*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(name,data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: update of class  WeaponEntity */
#ifndef TOLUA_DISABLE_tolua_peak_WeaponEntity_update00
static int tolua_peak_WeaponEntity_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WeaponEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WeaponEntity* self = (WeaponEntity*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'",NULL);
#endif
  {
   self->update(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_peak_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"PlayerEntity","PlayerEntity","peak::Entity",tolua_collect_PlayerEntity);
  #else
  tolua_cclass(tolua_S,"PlayerEntity","PlayerEntity","peak::Entity",NULL);
  #endif
  tolua_beginmodule(tolua_S,"PlayerEntity");
   tolua_function(tolua_S,"new",tolua_peak_PlayerEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_peak_PlayerEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_peak_PlayerEntity_new00_local);
   tolua_function(tolua_S,"delete",tolua_peak_PlayerEntity_delete00);
   tolua_function(tolua_S,"load",tolua_peak_PlayerEntity_load00);
   tolua_function(tolua_S,"update",tolua_peak_PlayerEntity_update00);
   tolua_function(tolua_S,"setMovement",tolua_peak_PlayerEntity_setMovement00);
   tolua_function(tolua_S,"setRotation",tolua_peak_PlayerEntity_setRotation00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"WeaponEntity","WeaponEntity","peak::Entity",tolua_collect_WeaponEntity);
  #else
  tolua_cclass(tolua_S,"WeaponEntity","WeaponEntity","peak::Entity",NULL);
  #endif
  tolua_beginmodule(tolua_S,"WeaponEntity");
   tolua_function(tolua_S,"new",tolua_peak_WeaponEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_peak_WeaponEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_peak_WeaponEntity_new00_local);
   tolua_function(tolua_S,"delete",tolua_peak_WeaponEntity_delete00);
   tolua_function(tolua_S,"load",tolua_peak_WeaponEntity_load00);
   tolua_function(tolua_S,"update",tolua_peak_WeaponEntity_update00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_peak (lua_State* tolua_S) {
 return tolua_peak_open(tolua_S);
};
#endif


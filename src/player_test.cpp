#include <lua.hpp>
#include <new>
#include <string>
#include <cstring>

class Vec2
{
public:
    float x, y;
    Vec2() {}
    Vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void print(){
        printf("{ %f , %f }\n",x,y);
    }

    ~Vec2() {}
};
static int new_Vec2(lua_State *L)
{
    if (lua_gettop(L) == 2)
    {
        int x = luaL_checknumber(L, 1);
        int y = luaL_checknumber(L, 2);
        void *ptr = lua_newuserdata(L, sizeof(Vec2));
        new (ptr) Vec2(x, y);
        luaL_getmetatable(L, "Vec2");
        lua_setmetatable(L, -2);
        return 1;
    }
    void *ptr = lua_newuserdata(L, sizeof(Vec2));
    new (ptr) Vec2();
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}
static int destroy_Vec2(lua_State *L)
{
    Vec2 *obj = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    obj->~Vec2();
    return 0;
}

static int vec2_index(lua_State* L) {
    Vec2* obj = (Vec2*)luaL_checkudata(L, 1, "Vec2");
    const char* key = luaL_checkstring(L, 2);
    
    if (strcmp(key, "x") == 0) {
        lua_pushnumber(L, obj->x);
        return 1;
    }
    else if (strcmp(key, "y") == 0) {
        lua_pushnumber(L, obj->y);
        return 1;
    }
    lua_getmetatable(L, 1);
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
    return 1;
}

static int vec2_newindex(lua_State* L) {
    Vec2* obj = (Vec2*)luaL_checkudata(L, 1, "Vec2");
    const char* key = luaL_checkstring(L, 2);
    float value = (float)luaL_checknumber(L, 3);
    
    if (strcmp(key, "x") == 0) {
        obj->x = value;
    }
    else if (strcmp(key, "y") == 0) {
        obj->y = value;
    }
    else {
        luaL_error(L, "Invalid field '%s'", key);
    }
    
    return 0;
}

static int vec2_print(lua_State* L){
    Vec2* obj = (Vec2*)luaL_checkudata(L, 1, "Vec2");
    obj->print();
    return 0;
}

static const luaL_Reg vec2_methods[] = {
    {"print",vec2_print},
    {NULL,NULL}
};

void register_Vec2(lua_State *L)
{
    luaL_newmetatable(L, "Vec2");
    lua_pushcfunction(L, destroy_Vec2);
    lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, vec2_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, vec2_newindex);
    lua_setfield(L, -2, "__newindex");
    luaL_setfuncs(L, vec2_methods, 0);
    lua_pop(L, 1);

    lua_newtable(L);
    lua_newtable(L);
    lua_pushcfunction(L, new_Vec2);
    lua_setfield(L, -2, "__call");
    lua_setmetatable(L, -2);
    lua_setglobal(L, "Vec2");
}


class Player
{
public:
    std::string name;
    Vec2 position;
    Player() {}
    Player(std::string name, Vec2 position)
    {
        this->name = name;
        this->position = position;
    }
    void identfy()
    {
        printf("name: \"%s\" position: { %f , %f }\n", name.c_str(), position.x, position.y);
    }
};

void create_Player(lua_State *L)
{
}
void register_Player(lua_State *L)
{
}

void register_classes(lua_State *L)
{
    register_Vec2(L);
    //register_Player(L);
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_loadstring(L,
        "local v2 = Vec2()\n"
        "v2.x = 2\n"
        "v2.y = 4\n"
        "print(v2.x + v2.y)\n"
        "v2:print()\n"

        /*
                    "local pos1 = Vec2(10,20)\n"
                    "local player1 = Player(\"Alam\",pos1)\n"
                    "player2.identfy()\n"

                    "local pos2 = Vec2()\n"
                    "pos2.x = 32\n"
                    "pos2.y = 64\n"
                    "local player2 = Player(\"Adam\",pos2)\n"
                    "player2.identfy()\n"
                    */
            );

    register_classes(L);

    lua_pcall(L, 0, 0, 0);

    lua_close(L);
}
#include <lua.hpp>
#include <new>
#include <string>

/*

static int create_class(lua_State *L)
{
    int x = luaL_checkinteger(L, 1);
    void *ptr = lua_newuserdata(L, sizeof(MyClass));
    new (ptr) MyClass(x);
    luaL_getmetatable(L, "MyClassMetatable");
    lua_setmetatable(L, -2);
    return 1;
}

static int MyClassFunction_MyClassFunction(lua_State *L)
{
    MyClass *obj = (MyClass *)luaL_checkudata(L, 1, "MyClassMetatable");
    lua_pushinteger(L, obj->MyClassFunction());
    return 1;
}

static int destroy_class(lua_State *L)
{
    MyClass *obj = (MyClass *)luaL_checkudata(L, 1, "MyClassMetatable");
    obj->~MyClass();
    return 0;
}

static const luaL_Reg lua_functions[] = {
    {"MyClassFunction", MyClassFunction_MyClassFunction},
    {NULL, NULL}};

void registrar_classe(lua_State *L)
{
    luaL_newmetatable(L, "MyClassMetatable");
    lua_pushcfunction(L, destroy_class);
    lua_setfield(L, -2, "__gc");
    luaL_setfuncs(L, lua_functions, 0);
    lua_pop(L, 1);

    lua_newtable(L);
    lua_newtable(L);
    lua_pushcfunction(L, create_class);
    lua_setfield(L, -2, "__call");
    lua_setmetatable(L, -2);
    lua_setglobal(L, "MyClass");
}
*/

class Vec2
{
public:
    float x, y;
    Vec2() {}
    Vec2(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    ~Vec2() {}
};
static int create_Vec2(lua_State *L)
{
    if (lua_gettop(L) == 2)
    {
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
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

void register_Vec2(lua_State *L)
{
    luaL_newmetatable(L, "Vec2Metatable");
    lua_pushcfunction(L, destroy_Vec2);
    lua_setfield(L, -2, "__gc");
    // luaL_setfuncs(L, vec2_lua_functions, 0);
    lua_pop(L, 1);

    lua_newtable(L);
    lua_newtable(L);
    lua_pushcfunction(L, create_Vec2);
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
    register_Player(L);
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_loadstring(L,

                    "local pos1 = Vec2(10,20)\n"
                    "local player1 = Player(\"Alam\",pos1)\n"
                    "player2.identfy()\n"

                    "local pos2 = Vec2()\n"
                    "pos2.x = 32\n"
                    "pos2.y = 64\n"
                    "local player2 = Player(\"Adam\",pos2)\n"
                    "player2.identfy()\n"
            );

    register_classes(L);

    lua_pcall(L, 0, 0, 0);

    lua_close(L);
}
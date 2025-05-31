#include <lua.hpp>
#include <new>

class MyClass
{
public:
    MyClass() { printf("hello!\n"); }
    MyClass(int x) : value(x) { printf("hello!\n"); }
    ~MyClass() { printf("bye!\n"); }
    int MyClassFunction() { return value * 2; }

private:
    int value;
};

static int create_class(lua_State *L)
{
    if (lua_gettop(L) == 2)
    {
        int x = luaL_checkinteger(L, 2);
        void *ptr = lua_newuserdata(L, sizeof(MyClass));
        new (ptr) MyClass(x);
        luaL_getmetatable(L, "MyClassMetatable");
        lua_setmetatable(L, -2);
    }
    else
    {
        void *ptr = lua_newuserdata(L, sizeof(MyClass));
        new (ptr) MyClass(0);
        luaL_getmetatable(L, "MyClassMetatable");
        lua_setmetatable(L, -2);
    }

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

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_loadstring(L,
                    "print(\"start\")\n"
                    "print(MyClass)\n"
                    "local obj = MyClass(42)\n"
                    "print(obj:MyClassFunction())\n"
                    "print(\"end\")\n");

    registrar_classe(L);

    lua_pcall(L, 0, 0, 0);

    lua_close(L);
}
#pragma push_macro("fninit")
#define fninit(name) name=(decltype(name))GetProcAddress(libm, #name)
#pragma push_macro("fndec")
#define fndec(ret,call,name,...) ret(call*name)(__VA_ARGS__)=0
#pragma push_macro("fnrdec")
#define fnrdec(name) const decltype(instance::name)& name = 0;

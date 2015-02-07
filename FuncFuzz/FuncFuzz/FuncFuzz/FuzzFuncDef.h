//dll 导出函数
typedef void (*isExp)(char *dst, char *src); //myCopy

//dll 非导出函数
typedef void (*unExp)(char *dst);	//myShow
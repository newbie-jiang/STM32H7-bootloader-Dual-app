#include <stdio.h>
#include <string.h>
#include "command.h"


typedef struct {
    uint8_t para1;
    uint8_t para2;
    uint8_t para3;
} sparament;

// 假设这是你的环境变量
sparament environment_variable={
  .para1 =1,
	.para2 =2,
	.para3 =3,
};


static int setenv(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: setenv <parameter> <value>\r\n");
        return 1;  // 返回错误码
    }

    // 获取环境变量的参数名和值
    char *parameter_name = argv[1];
    uint8_t value;

    // 从命令行参数中获取值
    if (sscanf(argv[2], "%hhu", &value) != 1) {
        fprintf(stderr, "Invalid value\r\n");
        return 1;
    }

    // 根据参数名设置相应的环境变量
    if (strcmp(parameter_name, "para1") == 0) {
        environment_variable.para1 = value;
    } else if (strcmp(parameter_name, "para2") == 0) {
        environment_variable.para2 = value;
    } else if (strcmp(parameter_name, "para3") == 0) {
        environment_variable.para3 = value;
    } else {
        fprintf(stderr, "Invalid parameter name\r\n");
        return 1;
    }

    // 在这里可以执行其他设置环境变量的操作，比如保存到文件或者其他地方

    printf("Environment variable set: %s=%u\r\n", parameter_name, value);

    return 0;  // 返回成功码
}

struct command sevent_cmd = {
    "setenv",
    "Setting environment variables\r\n",
    "Usage: sevent <parameter> <value>\r\n",
    setenv,
};




//打印所有当前环境变量的值
static int printenv(int argc, char **argv) {

     // 打印环境变量
    printf("para1=%u\r\n", environment_variable.para1);
    printf("para2=%u\r\n", environment_variable.para2);
    printf("para3=%u\r\n", environment_variable.para3);
    return 0;  // 返回成功码
}

struct command printeven_cmd = {
    "env",
    "print all environment variables\r\n",
    "Usage: printenv\r\n",
    printenv,
};

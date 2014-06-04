#!Makefile

# ------------------------------------------------------------------------
#
# 项目的 Makefile 示例，对于此结构代码，该 Makefile 均通用。
#
# ------------------------------------------------------------------------
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc
C_FLAGS = -D_REENTRANT -DSERVER_DEBUG -c -Wall -Iinclude -g
LINK_FLAGS = -L/usr/lib/mysql -lmysqlclient $(mysql_config --cflags) $(mysql_config --libs)
PROGRAM = driver

all: $(C_OBJECTS) 
	@echo 链接 ...
	$(CC) $(C_OBJECTS) $(LINK_FLAGS) -o $(PROGRAM)

.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.PHONY:clean
clean:
	@echo 清理临时文件
	$(RM) $(C_OBJECTS) $(PROGRAM)


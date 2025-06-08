# 编译器设置
CXX = g++
TARGET = life_sdl

# 源文件列表
SRCS = lifegrid.cpp cell.cpp sdl_renderer.cpp main.cpp

# 自动生成目标文件列表
OBJS = $(SRCS:.cpp=.o)

# SDL2配置（通过brew自动检测路径）
SDL2_CFLAGS = -I$(shell brew --prefix sdl2)/include
SDL2_LDFLAGS = -L$(shell brew --prefix sdl2)/lib -lSDL2

# 编译选项
CXXFLAGS = -std=c++11 -Wall -Wextra $(SDL2_CFLAGS)
LDFLAGS = $(SDL2_LDFLAGS)

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# 通用编译规则
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理规则
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

# 安装依赖（MacOS）
.PHONY: install
install:
	brew install sdl2

# 运行程序
.PHONY: run
run: $(TARGET)
	./$(TARGET)
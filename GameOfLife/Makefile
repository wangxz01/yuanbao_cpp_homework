# 编译器设置
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# 项目文件
SRC_DIR = .
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = lifegrid.h cell.h

# 最终目标
TARGET = life

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 通用编译规则
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理规则
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

# 运行规则
.PHONY: run
run: $(TARGET)
	./$(TARGET)
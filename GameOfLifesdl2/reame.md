## 目录结构
GameOfLife/
├── Makefile
├── cell.h
├── cell.cpp
├── lifegrid.h
├── lifegrid.cpp
├── sdlrenderer.h
├── sdlrenderer.cpp
└── main.cpp


```bash
make          # 完整编译
make clean    # 清理构建产物
make run      # 编译并运行
```

相比于前面的控制台版本,**实现了利用SDL2库来渲染**
是非常可喜的成就
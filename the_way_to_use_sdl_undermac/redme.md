## 测试了很多次,至少这样可以保证成功编译并且成功运行,在sdl库下
首先下载sdl
```bash
brew install sdl2
```
编译运行
```bash
g++ sdl_window.cpp -o test -I$(brew --prefix sdl2)/include -L$(brew --prefix sdl2)/lib -lSDL2 

./test
```

测试代码在同一目录下

g++ life_game.cpp -o test -I$(brew --prefix sdl2)/include -L$(brew --prefix sdl2)/lib -lSDL2 

g++ f.cpp -o test -I$(brew --prefix sdl2)/include -L$(brew --prefix sdl2)/lib -lSDL2 
# MKAul

AviUtlプラグイン作成用の自作ライブラリです。(開発途中)

AviUtlのプラグインを作成する上での基本的な機能を提供するほか、GDI+とDirect2Dを統合したラッパーとしても機能します。

## 使い方
`mkaul.hpp`をインクルードし、`mkaul.lib`をリンクします。  
```cpp
#include <mkaul.hpp>
#pragma comment(lib, "mkaul.lib")
```

## Graphicsの使用方法
1. `Manager::startup()`関数で描画方式を指定して描画環境を用意します。
```cpp
mkaul::graphics::Manager::startup(mkaul::graphics::Manager::GraphicMethod::Directx);
```

2. `Manager::create_graphics()`関数でGraphicsオブジェクトを作成し、`Graphics::init()`メソッドで初期化します。
```cpp
mkaul::graphics::Graphics* p_graphics;
mkaul::graphics::Manager::create_graphics(&p_graphics);

p_graphics->init(hwnd);
```

3. `Graphics::begin_draw()`を呼び出して描画を開始し、`Graphics::end_draw()`を呼び出して描画を終了します。
```cpp
p_graphics->begin_draw();
// ここに描画のコードを記述する
p_graphics->end_draw();
```

4. 最後に`Graphics::exit()`メソッドでGraphicsオブジェクトを破棄し、`Manager::shutdown()`関数で描画環境を破棄します。
```cpp
p_graphics->exit();

mkaul::graphics::Manager::shutdown();
```

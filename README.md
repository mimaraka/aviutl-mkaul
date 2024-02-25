# MKAul

AviUtlプラグイン作成用の自作ライブラリです。(開発途中)

AviUtlのプラグインを作成する上での基本的な機能を提供するほか、GDI+とDirect2Dを統合したラッパーとしても機能します。

## 使い方
`mkaul.hpp`をインクルードし、`mkaul.lib`をリンクします。  
```cpp
#include <mkaul.hpp>
#pragma comment(lib, "mkaul.lib")
```

## Graphicsオブジェクトの使用方法
1. `Factory::startup()`関数で描画方式を指定して描画環境を用意します。
```cpp
using graphics = mkaul::graphics;
using GraphicEngine = graphics::Factory::GraphicEngine;
graphics::Factory::startup(GraphicEngine::Directx);
```

2. `Factory::create_graphics()`関数でGraphicsオブジェクトを作成し、`Graphics::init()`メソッドで初期化します。
```cpp
auto p_graphics = graphics::Factory::create_graphics();
p_graphics->init(hwnd);
```

3. `Graphics::begin_draw()`メソッドを呼び出して描画を開始し、`Graphics::end_draw()`メソッドを呼び出して描画を終了します。
```cpp
p_graphics->begin_draw();
// ここに描画のコードを記述する
p_graphics->end_draw();
```

4. 最後に`Graphics::release()`メソッドでGraphicsオブジェクトを破棄し、`Factory::shutdown()`関数で描画環境を破棄します。
```cpp
p_graphics->release();
graphics::Factory::shutdown();
```

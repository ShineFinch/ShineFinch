## ShineFinch 撷影灵雀 — A minimal and elegant Qt-based screen capture utility. Designed for smooth, quick, and beautiful screen capturing. Capture light, capture moments.  

撷影灵雀 —— 基于 Qt 的极简优雅截图工具，为流畅、快速、美观的截图体验而生。撷取光影，捕捉瞬间。

### Macos build
```shell
./Qt/6.10.2/macos/bin/macdeployqt ~/{path}/ShineFinch/build/Qt_6_10_2_for_macOS-Release/ShineFinch.app -qmldir=/{qmlPath}/ShineFinch -dmg
```

### Install
```shell
xattr -rd com.apple.quarantine /{path}/ShineFinch.app
```
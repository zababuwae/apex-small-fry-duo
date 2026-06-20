# Apex Small Fry Duo

## English

A small 3D game engine developed collaboratively in C++ by two developers.

### Development Environment

- Visual Studio 2022
- MSVC v143
- C++17
- DirectX 11
- CMake 3.25 or later
- Windows SDK 10
- x64

Configure and build from PowerShell:

```powershell
cmake --preset vs2022-x64
cmake --build --preset debug
```

The Debug executable is generated at:

```text
build/vs2022-x64/Debug/ApexSmallFryDuo.exe
```

### First Milestone

Our first goal is to build a simple playable 3D scene with:

- A window and game loop
- Keyboard and mouse input
- Basic 3D rendering
- A movable camera
- A textured cube
- Simple lighting

### Branches

- `main`: Stable and integrated code
- `wa`: Engine core, window, input, and build system
- `kon`: Rendering, camera, scene, and graphics features

All completed work is reviewed and merged into `main`.

---

## 日本語

C++を使って2人で共同開発する、小規模な3Dゲームエンジンです。

### 開発環境

- Visual Studio 2022
- MSVC v143
- C++17
- DirectX 11
- CMake 3.25以降
- Windows SDK 10
- x64

PowerShellから以下のコマンドで構成とビルドを行います。

```powershell
cmake --preset vs2022-x64
cmake --build --preset debug
```

Debug版の実行ファイルは以下の場所に生成されます。

```text
build/vs2022-x64/Debug/ApexSmallFryDuo.exe
```

### 最初の目標

最初のマイルストーンでは、以下の機能を備えたシンプルで操作可能な3Dシーンを作成します。

- ウィンドウとゲームループ
- キーボードとマウスの入力
- 基本的な3Dレンダリング
- 移動可能なカメラ
- テクスチャ付きの立方体
- シンプルなライティング

### ブランチ

- `main`: 安定した統合コード
- `wa`: エンジンコア、ウィンドウ、入力、ビルドシステム
- `kon`: レンダリング、カメラ、シーン、グラフィックス機能

完成した作業は相互にレビューし、`main`へマージします。

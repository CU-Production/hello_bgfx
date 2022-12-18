# hello_bgfx
GLFW + bgfx demo

## how to install dependencies

```shell
mkdir 3rdparty
cd 3rdparty
git clone https://github.com/bkaradzic/bx.git --depth 1
git clone https://github.com/bkaradzic/bimg.git --depth 1
git clone https://github.com/bkaradzic/bgfx.git --depth 1
git clone https://github.com/glfw/glfw.git --depth 1 --branch 3.3.8
cd bgfx
..\bx\tools\bin\windows\genie --with-examples --with-tools vs2022
msbuild .build\projects\vs2022\bgfx.sln -target:examples\example-01-cubes;tools\shaderc\shaderc -property:Configuration=Release -property:Platform="x64"
# If you need debug bgfx use
# msbuild .build\projects\vs2022\bgfx.sln -target:examples\example-01-cubes;tools\shaderc\shaderc -property:Configuration=Debug -property:Platform="x64"
```

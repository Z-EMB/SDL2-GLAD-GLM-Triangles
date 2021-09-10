## Example opengl 3.3 with SDL2 GLAD GLM

### Requirements
* cmake minimum 3.16.3
* gcc 8+
* make
* graphics card or mesa with 3.3 gl software rendering

### Clone Build Run
Execute below from your projects folder
```shell
git clone https://github.com/Z-EMB/SDL2-GLAD-GLM-Triangles.git && cd SDL2-GLAD-GLM-Triangles && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make SDL2_GLAD_GLM_Triangles && cd ../bin/ && ./SDL2_GLAD_GLM_Triangles
```

### Clean up
Remove `bin` and `build` directories from root of the project.

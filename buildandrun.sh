#!/bin/bash

g++ -std=c++17 -o bin/game main.cpp Logger.cpp RenderSystem.cpp SoundSystem.cpp -I . -I vendor/raylib/src -I vendor/spdlog/include/ -I vendor/entt/src -I vendor/FMOD/inc -L vendor/FMOD/lib -L vendor/raylib/ -lfmod -lfmodstudio -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && cp -r assets bin/
#export LD_LIBRARY_PATH=/home/anto/Escritorio/SoundSeum/vendor/FMOD/lib:$LD_LIBRARY_PATH 
#export LD_LIBRARY_PATH="/media/pierre/Storage/git/fmod-example-ecs/vendor/FMOD/lib/:$LD_LIBRARY_PATH" 
#./bin/game
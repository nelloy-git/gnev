#ifdef WIN32
#include <vld.h>
#endif

#include <chrono>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GlfwConveyor.hpp"

#include "Drawer.hpp"

int main(int argc, const char** argv) {
    GlfwConveyor conveyor;

    conveyor.worker.push([&conveyor](){
        gnev::Drawer drawer;
        auto err = drawer.init(conveyor.get_proc_address());

        std::cout << "Init status: " << (err.first ? "ok" : "fail") << std::endl;
        if (!err.first){
            std::cout << err.second.c_str() << std::endl;
            return;
        }

        for (int i = 0; i < 100; i++){
            for (int j = 0; j < 100; j++){
                for (int k = 0; k < 100; k++){
                    drawer.add_voxel(-i, -j, -k);
                }
            }
        }

        // drawer.add_voxel(0, 0, 0);
        // drawer.add_voxel(-1, -1, 0);
        // drawer.add_voxel(0, -1, -1);
        // drawer.add_voxel(-1, 0, -1);
        // drawer.add_voxel(-0.5, 0.5, 0);
        // drawer.add_voxel(0.5, -0.5, 0);
        // drawer.add_voxel(0.5, -0.5, 0);

        bool alive = true;
        conveyor.key_callback = [&alive, &drawer](GlfwConveyor* conveyor, int key, int scancode, int action, int mods){
            const float vel = 0.05;

            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
                alive = false;
            }

            if (action == GLFW_PRESS || action == GLFW_REPEAT){
                bool moved = false;
                switch (key){
                case GLFW_KEY_A: drawer.camera->pos.x += vel; moved = true; break;
                case GLFW_KEY_D: drawer.camera->pos.x -= vel; moved = true; break;
                case GLFW_KEY_LEFT_CONTROL: drawer.camera->pos.y += vel; moved = true; break;
                case GLFW_KEY_SPACE: drawer.camera->pos.y -= vel; moved = true; break;
                case GLFW_KEY_W: drawer.camera->pos.z += vel; moved = true; break;
                case GLFW_KEY_S: drawer.camera->pos.z -= vel; moved = true; break;
                default: break;
                }
                if (moved){
                    drawer.camera->applyLookAt({0,0,0});
                }
            }
        };

        auto last_print = std::chrono::steady_clock::now();
        std::queue<std::chrono::microseconds> times;

        while(alive){
            conveyor.poll_events();

            auto start = std::chrono::steady_clock::now();
            conveyor.swap_buffers();
            drawer.draw();
            auto end = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            times.push(dt);

            if (std::chrono::duration_cast<std::chrono::seconds>(end - last_print).count() > 0){
                size_t count = times.size();
                size_t sum = 0;
                while (times.size() > 0){
                    sum += times.front().count();
                    times.pop();
                }
                std::cout << "Average time: " << sum / count << "us" << std::endl;
                last_print = std::chrono::steady_clock::now();
            }
        }
    });

    return 0;
}
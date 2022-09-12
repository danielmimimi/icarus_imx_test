ln -sf aarch64-obj_rel/debug/libposenetimx8.so.0.0.0 libposenetimx8.so.0
g++ main.cpp -I. -ggdb -L/home/root/general_inference_imx8/aarch64-obj_rel/debug -l:libposenetimx8.so.0.0.0 -l:libtensorflow-lite.so.2.6.0 -l:libgooglecoraledgetpuposenet.so.0  -o out

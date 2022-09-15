ln -sf aarch64-obj_dbg/libposenetimx8.so.0.0.0 libposenetimx8.so.0
g++ main.cpp -I/home/root/general_inference_imx8/include -I/home/root/general_inference_imx8/src -ggdb -L/home/root/general_inference_imx8/aarch64-obj_dbg -l:libposenetimx8.so.0.0.0 -l:libtensorflow-lite.so.2.6.0 -l:libgooglecoraledgetpuposenet.so.0  -o out

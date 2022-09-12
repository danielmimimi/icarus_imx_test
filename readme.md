scp -r C:\projects\icarus\trunk\externals\general_inference_imx8 root@192.168.61.121:

ln -sf aarch64-obj_rel/libposenetimx8.so.0.0.0 libposenetimx8.so.0


g++ main.cpp -I. -ggdb -L/home/root/general_inference_imx8/aarch64-obj_rel -l:libposenetimx8.so.0.0.0 -l:libtensorflow-lite.so.2.6.0 -l:libgooglecoraledgetpuposenet.so.0  -o out

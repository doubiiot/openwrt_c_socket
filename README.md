# openwrt_c_socket
该代码作用主要是通过socket与服务器连接，通过接收服务器的消息来控制openwrt板子的PWM口实现相应的功能。
编译环境安装见：http://doubiiot.cn/wordpress/?p=244

编译：mipsel-openwrt-linux-gcc wrt_sock.c -o wrt_sock
运行：./wrt_sock

运行环境：widora MT7688

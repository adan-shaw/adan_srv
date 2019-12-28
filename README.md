# adan_srv ***made in eclipse cdt programming tools***
# this open source project is not design for production environment ...
# it just a personal sharing project ...
# so welcome you to comment,give me some good advice ...
# ***********************************
# if this project has any infringement, please let me know, here's our contact email, thank you.
# email:adan_shaw@qq.com.
# ***********************************
# and if you want to reprint this project please indicate the source from https://github.com/adan-shaw/adan_srv
# ***********************************
# how to use ?
# make clean
# make x3 (gmake)
# chmod 755 ./x3
# ./x3
# input number 1 choose command_1 to start the service
# ***********************************
# how to change the running mode of this server ?
# 1.add service io pthread:
# open static_val.h
# change #define pool_max 4 or 8 or 2 or ...
# 2.simulation business operation time consuming:
# open pickup_data.c
# change usleep(100);//0.1ms 
#
# Date: Oct 13, 2016 by adan shaw

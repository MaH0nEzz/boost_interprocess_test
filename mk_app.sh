#!/bin/sh

echo "\n==========begin=========="

cowsay hahahahahahaha!!

g++ create_map_class.cpp -o create_map -lrt -pthread

echo "\ncreate_map_class finished!"

g++ open_map_class.cpp -o open_map -lrt -pthread

echo "\nopen_map_class finished!"

echo "\nstart create_map_class!"

./create_map

echo "\nstart open_map_class!"

./open_map


echo "\n==========finish========="

#!/bin/bash
ndk-build -C src/jni
cp ./src/obj/local/armeabi/libsafejni.so ./dist/libs/armeabi/libsafejni.so
cp ./src/obj/local/armeabi-v7a/libsafejni.so ./dist/libs/armeabi-v7a/libsafejni.so
cp ./src/safejni.h ./dist/safejni.h

#!/bin/bash
ndk-build
cp ./obj/local/armeabi/libsafejni.a ../bin/armeabi/libsafejni.a
cp ./obj/local/armeabi-v7a/libsafejni.a ../bin/armeabi-v7a/libsafejni.a
cp ./safejni.h ../bin/safejni.h

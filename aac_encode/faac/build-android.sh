
#!/bin/bash
ANDROID_API=android-9
export ANDROID_NDK=/opt/android-ndk
export ANDROID_SDK=${HOME}/android-sdk
SYSROOT=$ANDROID_NDK/platforms/$ANDROID_API/arch-arm
export PATH=$PATH:$ANDROID_SDK/tools:$ANDROID_SDK/platform-tools



for version in armeabi armeabi-v7a x86; do
#for version in x86; do
#for version in armeabi; do

export ARM_INC=$ANDROID_NDK/platforms/$ANDROID_API/arch-arm/usr/include
export ARM_LIB=$ANDROID_NDK/platforms/$ANDROID_API/arch-arm/usr/lib

ANDROID_BIN=$ANDROID_NDK/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86/bin
CROSS_COMPILE=${ANDROID_BIN}/arm-linux-androideabi-
export ARCH=arm

  case $version in
    armeabi-v7a)
      EXTRA_CFLAGS="-mthumb -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp"
      EXTRA_LDFLAGS="-Wl,--fix-cortex-a8"
      ;;
    armeabi)
      EXTRA_CFLAGS="-march=armv6"
      EXTRA_LDFLAGS=""
      ;;
    x86)
      export ARM_INC=$ANDROID_NDK/platforms/$ANDROID_API/arch-x86/usr/include
      export ARM_LIB=$ANDROID_NDK/platforms/$ANDROID_API/arch-x86/usr/lib
      
      export ARCH=x86
      #export CROSS_PREFIX=i686-linux-android-
      ANDROID_BIN=$ANDROID_NDK/toolchains/x86-4.8/prebuilt/linux-x86/bin  
      CROSS_COMPILE=${ANDROID_BIN}/i686-linux-android-
      #export FFMPEG_VERSION=$ARCH

      #export CC=${CROSS_COMPILE}gcc-4.8
      #export LD=${CROSS_COMPILE}ld
      #export AR=${CROSS_COMPILE}ar
      #export STRIP=${CROSS_COMPILE}strip
      EXTRA_CFLAGS="-mincoming-stack-boundary=4"
      EXTRA_LDFLAGS=""
      ;;
  esac

export ARM_INC=$ANDROID_NDK/platforms/$ANDROID_API/arch-$ARCH/usr/include
export ARM_LIB=$ANDROID_NDK/platforms/$ANDROID_API/arch-$ARCH/usr/lib


#-mthumb-interwork -mthumb -march=armv5te -mtune=xscale
CFLAGS=" $EXTRA_CFLAGS -I$ARM_INC -Wall -DANDROID -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -msoft-float  -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -DANDROID  -Wa,--noexecstack -MMD -MP -fno-stack-protector "

LDFLAGS=" -nostdlib -Bdynamic -Wl,--whole-archive -Wl,--no-undefined -Wl,-z,noexecstack  -Wl,-z,nocopyreloc -Wl,-soname,/system/lib/libz.so -Wl,-rpath-link=$ARM_LIB,-dynamic-linker=/system/bin/linker -L$ANDROID_NDK/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/arm-linux-androideabi/lib -L$ANDROID_NDK/sources/cxx-stl/gnu-libstdc++/4.8/libs/$version -L$ARM_LIB  -lc -lgcc -lm -ldl -lgnustl_shared "

FLAGS="--host=arm-androideabi-linux --enable-static --disable-shared --prefix=$HOME --enable-armv5e  "

export CPPFLAGS="$CFLAGS"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CFLAGS"
export CXX="${CROSS_COMPILE}g++ --sysroot=${SYSROOT}"
export LDFLAGS="$LDFLAGS"
export CC="${CROSS_COMPILE}gcc --sysroot=${SYSROOT}"
export NM="${CROSS_COMPILE}nm"
export STRIP="${CROSS_COMPILE}strip"
export RANLIB="${CROSS_COMPILE}ranlib"
export AR="${CROSS_COMPILE}ar"


./configure $FLAGS
make clean
make -j4 || exit 1
#make install || exit 1

  cp ./common/mp4v2/libmp4v2.a /home/CommonFile/Lib/android_static/$version -rf
  cp ./libfaac/.libs/libfaac.a /home/CommonFile/Lib/android_static/$version -rf
  if test "$?" != 0; then
    echo -e "\033[31m compiler failed. \033[0m"
    exit 1
  else
    echo "==============================================================="
    echo "compile success: /home/CommonFile/Lib/android_static/$version"
    echo "==============================================================="
  fi

done


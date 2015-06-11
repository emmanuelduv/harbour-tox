# harbour-tox
Tox client for Sailfish OS on Jolla phone

This is now a first version of Tox (https://tox.im/) client for Sailfish OS, it may be able to send and receive text message. I did not test it for a long time on my Jolla and it may drain the battery... Current version must be launched from command line in a writable directory.

Al others tox features are yet missing :
 - File transfert & avatars
 - Video/Voice call
 - Chat room

Patchs to correct bugs or add new features are welcome!

##Requirements
###Running
devel packages mentionned in Compilation should have their binary version installed (without "-devel"), libtool not required, it's a build tool.
###Compilation
Build process is done via QtCreator provided in Sailfish SDK. It wil generate everything for armv7 including RPMs (generated source RPM has missing files)
Required packages : qt5-qtcore-devel qt5-qtgui-devel libsailfishapp-devel qt5-qtdeclarative-qtquick-devel libogg-devel  libtool, optionnal : tar, nano

Others libraries not packaged in Sailfish OS and the versions i used (other versions may work) :
- libvpx (1.4.0)
configured with

./configure --prefix=/usr/share/harbour-tox --disable-examples --disable-docs --enable-vp9-highbitdepth --enable-vp9 --enable-vp9-postproc --enable-postproc --enable-error-concealment --enable-shared --disable-static --enable-pic --target=generic-gnu --enable-runtime-cpu-detect --enable-vp9-temporal-denoising --enable-multi-res-encoding
- opus (1.1)
configured with

./configure --prefix=/usr/share/harbour-tox --disable-doc --disable-extra-programs --enable-rtcd --enable-float-approx --enable-static=no
- libsodium (1.0.2)
configured with

./configure --prefix=/usr/share/harbour-tox
- toxcore (cloned from https://github.com/irungentoo/toxcore.git)
configured with

./configure --prefix=/usr/share/harbour-tox --with-libsodium-headers=/usr/share/harbour-tox/include --with-libsodium-libs=/usr/share/harbour-tox/lib

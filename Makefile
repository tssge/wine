# This Makefile understands the following targets:
#
# all (default):   build wine
# clean:           remove all intermediate files
# distclean:       also remove all files created by configure
# test:            run tests
# testclean:       clean test results to force running all tests again
# install-lib:     install libraries needed to run applications
# install-dev:     install development environment
# install:         install everything
# uninstall:       uninstall everything
# ctags:           create a tags file for vim and others.
# etags:           create a TAGS file for Emacs.

SHELL = /bin/sh
PATH_SEPARATOR = :
PACKAGE_NAME = Wine
PACKAGE_TARNAME = wine
PACKAGE_VERSION = 10.0
PACKAGE_STRING = Wine 10.0
PACKAGE_BUGREPORT = wine-devel@winehq.org
PACKAGE_URL = https://www.winehq.org
exec_prefix = ${prefix}
prefix = /usr/local
program_transform_name = s,x,x,
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datarootdir = ${prefix}/share
datadir = ${datarootdir}
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
runstatedir = ${localstatedir}/run
includedir = ${prefix}/include
oldincludedir = /usr/include
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
infodir = ${datarootdir}/info
htmldir = ${docdir}
dvidir = ${docdir}
pdfdir = ${docdir}
psdir = ${docdir}
libdir = ${exec_prefix}/lib
localedir = ${datarootdir}/locale
mandir = ${datarootdir}/man
DEFS = -DHAVE_CONFIG_H
ECHO_C = 
ECHO_N = -n
ECHO_T = 
LIBS = 
build_alias = 
host_alias = 
target_alias = 
system_dllpath = 
build = x86_64-pc-linux-gnu
build_cpu = x86_64
build_vendor = pc
build_os = linux-gnu
host = x86_64-pc-linux-gnu
host_cpu = x86_64
host_vendor = pc
host_os = linux-gnu
srcdir = .
SARIF_CONVERTER = 
CC = gcc -m64
CFLAGS = -g -O2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0
LDFLAGS = 
CPPFLAGS = 
ac_ct_CC = gcc
EXEEXT = 
OBJEXT = o
CXX = g++ -m64
CXXFLAGS = -g -O2
ac_ct_CXX = g++
CPPBIN = cpp
LD = ld
TARGETFLAGS =  -m64
toolsext = 
HOST_ARCH = x86_64
aarch64_CC = 
aarch64_CFLAGS = 
aarch64_EXTRACFLAGS = 
aarch64_CXXFLAGS = 
aarch64_LDFLAGS = 
aarch64_DEBUG = 
aarch64_TARGET = 
aarch64_STRIP = 
aarch64_DELAYLOADFLAG = 
aarch64_DISABLED_SUBDIRS = 
arm_CC = 
arm_CFLAGS = 
arm_EXTRACFLAGS = 
arm_CXXFLAGS = 
arm_LDFLAGS = 
arm_DEBUG = 
arm_TARGET = 
arm_STRIP = 
arm_DELAYLOADFLAG = 
arm_DISABLED_SUBDIRS = 
arm64ec_CC = 
arm64ec_CFLAGS = 
arm64ec_EXTRACFLAGS = 
arm64ec_CXXFLAGS = 
arm64ec_LDFLAGS = 
arm64ec_DEBUG = 
arm64ec_TARGET = 
arm64ec_STRIP = 
arm64ec_DELAYLOADFLAG = 
arm64ec_DISABLED_SUBDIRS = 
i386_CC = 
i386_CFLAGS = 
i386_EXTRACFLAGS = 
i386_CXXFLAGS = 
i386_LDFLAGS = 
i386_DEBUG = 
i386_TARGET = 
i386_STRIP = 
i386_DELAYLOADFLAG = 
i386_DISABLED_SUBDIRS = 
x86_64_CC = clang -D__STDC__
x86_64_CFLAGS = -g -O2
x86_64_EXTRACFLAGS = -D__WINE_PE_BUILD -Wall -target x86_64-windows -fuse-ld=lld --no-default-config -fno-strict-aliasing -Wdeclaration-after-statement -Wempty-body -Wignored-qualifiers -Winit-self -Wno-pragma-pack -Wno-microsoft-enum-forward-reference -Wstrict-prototypes -Wtype-limits -Wunused-but-set-parameter -Wvla -Wwrite-strings -Wpointer-arith -Wabsolute-value -ffunction-sections -fms-hotpatch -Wformat-overflow -Wnonnull -mcx16 -mcmodel=small -gdwarf-4
x86_64_CXXFLAGS = 
x86_64_LDFLAGS =  --no-default-config -fms-hotpatch -Wl,-debug:dwarf
x86_64_DEBUG = dwarf
x86_64_TARGET = x86_64-windows
x86_64_STRIP = llvm-strip
x86_64_DELAYLOADFLAG = -Wl,-delayload,
x86_64_DISABLED_SUBDIRS =  dlls/avifile.dll16 dlls/comm.drv16 dlls/commdlg.dll16 dlls/compobj.dll16 dlls/ctl3d.dll16 dlls/ctl3dv2.dll16 dlls/ddeml.dll16 dlls/dispdib.dll16 dlls/display.drv16 dlls/gdi.exe16 dlls/ifsmgr.vxd dlls/imm.dll16 dlls/keyboard.drv16 dlls/krnl386.exe16 dlls/lzexpand.dll16 dlls/mmdevldr.vxd dlls/mmsystem.dll16 dlls/monodebg.vxd dlls/mouse.drv16 dlls/msacm.dll16 dlls/msvideo.dll16 dlls/ole2.dll16 dlls/ole2conv.dll16 dlls/ole2disp.dll16 dlls/ole2nls.dll16 dlls/ole2prox.dll16 dlls/ole2thk.dll16 dlls/olecli.dll16 dlls/olesvr.dll16 dlls/rasapi16.dll16 dlls/setupx.dll16 dlls/shell.dll16 dlls/sound.drv16 dlls/storage.dll16 dlls/stress.dll16 dlls/system.drv16 dlls/toolhelp.dll16 dlls/twain.dll16 dlls/typelib.dll16 dlls/user.exe16 dlls/vdhcp.vxd dlls/ver.dll16 dlls/vmm.vxd dlls/vnbt.vxd dlls/vnetbios.vxd dlls/vtdapi.vxd dlls/vwin32.vxd dlls/w32skrnl dlls/w32sys.dll16 dlls/win32s16.dll16 dlls/win87em.dll16 dlls/winaspi.dll16 dlls/windebug.dll16 dlls/wineps16.drv16 dlls/wing.dll16 dlls/winnls.dll16 dlls/winsock.dll16 dlls/wintab.dll16 dlls/wow32 dlls/xtajit64 programs/rundll.exe16 programs/winevdm programs/winhelp.exe16 programs/winoldap.mod16
toolsdir = 
RUNTESTFLAGS = -q -P wine
SED_CMD = LC_ALL=C sed -e 's,@bindir@,${bindir},g' -e 's,@PACKAGE_STRING@,Wine 10.0,g' -e 's,@PACKAGE_VERSION@,10.0,g'
FLEX = flex
BISON = bison
STRIP = strip
LN_S = ln -s
PKG_CONFIG = pkg-config
FONTFORGE = 
RSVG = 
CONVERT = 
ICOTOOL = 
MSGFMT = false
I386_LIBS = 
OPENGL_LIBS = 
DLLFLAGS =  -fPIC -fasynchronous-unwind-tables
LDDLLFLAGS = -fPIC -fasynchronous-unwind-tables
LDEXECFLAGS = 
EXTRACFLAGS =  -Wall -pipe -fcf-protection=none -fvisibility=hidden -fno-stack-protector -fno-strict-aliasing -Wdeclaration-after-statement -Wempty-body -Wignored-qualifiers -Winit-self -Wlogical-op -Wno-packed-not-aligned -Wpointer-arith -Wshift-overflow=2 -Wstrict-prototypes -Wtype-limits -Wunused-but-set-parameter -Wvla -Wwrite-strings -Wno-misleading-indentation -gdwarf-4
UNIXDLLFLAGS = -fPIC -fasynchronous-unwind-tables
UNIXLDFLAGS = -shared -Wl,-Bsymbolic -Wl,-soname,$(UNIXLIB) -Wl,-z,defs
TOP_INSTALL_LIB =  fonts loader/wine.inf nls programs/msidb/msidb programs/msiexec/msiexec programs/notepad/notepad programs/regedit/regedit programs/regsvr32/regsvr32 programs/wineboot/wineboot programs/winecfg/winecfg programs/wineconsole/wineconsole programs/winedbg/winedbg programs/winefile/winefile programs/winemine/winemine programs/winepath/winepath server/wineserver loader/wine.desktop
TOP_INSTALL_DEV =  include
WINELOADER_LDFLAGS = -Wl,--export-dynamic -pie
WINEPRELOADER_LDFLAGS = -nostartfiles -nodefaultlibs -static-pie
DLLEXT = .so
LDD = /usr/bin/ldd
OTOOL = otool
READELF = readelf
SUBDIRS =  \
	dlls/acledit \
	dlls/aclui \
	dlls/activeds.tlb \
	dlls/activeds \
	dlls/activeds/tests \
	dlls/actxprxy \
	dlls/adsldp \
	dlls/adsldp/tests \
	dlls/adsldpc \
	dlls/advapi32 \
	dlls/advapi32/tests \
	dlls/advpack \
	dlls/advpack/tests \
	dlls/amsi \
	dlls/amd_ags_x64 \
	dlls/amstream \
	dlls/amstream/tests \
	dlls/apisetschema \
	dlls/apphelp \
	dlls/apphelp/tests \
	dlls/appwiz.cpl \
	dlls/appxdeploymentclient \
	dlls/atiadlxx \
	dlls/atl \
	dlls/atl/tests \
	dlls/atl100 \
	dlls/atl100/tests \
	dlls/atl110 \
	dlls/atl110/tests \
	dlls/atl80 \
	dlls/atl80/tests \
	dlls/atl90 \
	dlls/atlthunk \
	dlls/atlthunk/tests \
	dlls/atmlib \
	dlls/audioses \
	dlls/authz \
	dlls/avicap32 \
	dlls/avifil32 \
	dlls/avifil32/tests \
	dlls/avifile.dll16 \
	dlls/avrt \
	dlls/bcp47langs \
	dlls/bcrypt \
	dlls/bcrypt/tests \
	dlls/bcryptprimitives \
	dlls/bluetoothapis \
	dlls/bluetoothapis/tests \
	dlls/browseui \
	dlls/browseui/tests \
	dlls/bthprops.cpl \
	dlls/cabinet \
	dlls/cabinet/tests \
	dlls/capi2032 \
	dlls/cards \
	dlls/cdosys \
	dlls/cfgmgr32 \
	dlls/cfgmgr32/tests \
	dlls/clusapi \
	dlls/cng.sys \
	dlls/colorcnv \
	dlls/combase \
	dlls/combase/tests \
	dlls/comcat \
	dlls/comcat/tests \
	dlls/comctl32 \
	dlls/comctl32/tests \
	dlls/comdlg32 \
	dlls/comdlg32/tests \
	dlls/coml2 \
	dlls/comm.drv16 \
	dlls/commdlg.dll16 \
	dlls/compobj.dll16 \
	dlls/compstui \
	dlls/compstui/tests \
	dlls/comsvcs \
	dlls/comsvcs/tests \
	dlls/concrt140 \
	dlls/concrt140/tests \
	dlls/connect \
	dlls/coremessaging \
	dlls/credui \
	dlls/credui/tests \
	dlls/crtdll \
	dlls/crypt32 \
	dlls/crypt32/tests \
	dlls/cryptdlg \
	dlls/cryptdll \
	dlls/cryptext \
	dlls/cryptnet \
	dlls/cryptnet/tests \
	dlls/cryptowinrt \
	dlls/cryptowinrt/tests \
	dlls/cryptsp \
	dlls/cryptui \
	dlls/cryptui/tests \
	dlls/ctapi32 \
	dlls/ctl3d.dll16 \
	dlls/ctl3d32 \
	dlls/ctl3dv2.dll16 \
	dlls/d2d1 \
	dlls/d2d1/tests \
	dlls/d3d10 \
	dlls/d3d10/tests \
	dlls/d3d10_1 \
	dlls/d3d10_1/tests \
	dlls/d3d10core \
	dlls/d3d10core/tests \
	dlls/d3d11 \
	dlls/d3d11/tests \
	dlls/d3d12 \
	dlls/d3d12/tests \
	dlls/d3d12core \
	dlls/d3d8 \
	dlls/d3d8/tests \
	dlls/d3d8thk \
	dlls/d3d9 \
	dlls/d3d9/tests \
	dlls/d3dcompiler_33 \
	dlls/d3dcompiler_34 \
	dlls/d3dcompiler_35 \
	dlls/d3dcompiler_36 \
	dlls/d3dcompiler_37 \
	dlls/d3dcompiler_38 \
	dlls/d3dcompiler_39 \
	dlls/d3dcompiler_40 \
	dlls/d3dcompiler_41 \
	dlls/d3dcompiler_42 \
	dlls/d3dcompiler_43 \
	dlls/d3dcompiler_43/tests \
	dlls/d3dcompiler_46 \
	dlls/d3dcompiler_46/tests \
	dlls/d3dcompiler_47 \
	dlls/d3dcompiler_47/tests \
	dlls/d3dim \
	dlls/d3dim700 \
	dlls/d3drm \
	dlls/d3drm/tests \
	dlls/d3dx10_33 \
	dlls/d3dx10_34 \
	dlls/d3dx10_34/tests \
	dlls/d3dx10_35 \
	dlls/d3dx10_35/tests \
	dlls/d3dx10_36 \
	dlls/d3dx10_36/tests \
	dlls/d3dx10_37 \
	dlls/d3dx10_37/tests \
	dlls/d3dx10_38 \
	dlls/d3dx10_38/tests \
	dlls/d3dx10_39 \
	dlls/d3dx10_39/tests \
	dlls/d3dx10_40 \
	dlls/d3dx10_40/tests \
	dlls/d3dx10_41 \
	dlls/d3dx10_41/tests \
	dlls/d3dx10_42 \
	dlls/d3dx10_42/tests \
	dlls/d3dx10_43 \
	dlls/d3dx10_43/tests \
	dlls/d3dx11_42 \
	dlls/d3dx11_42/tests \
	dlls/d3dx11_43 \
	dlls/d3dx11_43/tests \
	dlls/d3dx9_24 \
	dlls/d3dx9_25 \
	dlls/d3dx9_26 \
	dlls/d3dx9_27 \
	dlls/d3dx9_28 \
	dlls/d3dx9_29 \
	dlls/d3dx9_30 \
	dlls/d3dx9_31 \
	dlls/d3dx9_32 \
	dlls/d3dx9_33 \
	dlls/d3dx9_34 \
	dlls/d3dx9_35 \
	dlls/d3dx9_36 \
	dlls/d3dx9_36/tests \
	dlls/d3dx9_37 \
	dlls/d3dx9_38 \
	dlls/d3dx9_39 \
	dlls/d3dx9_40 \
	dlls/d3dx9_41 \
	dlls/d3dx9_42 \
	dlls/d3dx9_43 \
	dlls/d3dx9_43/tests \
	dlls/d3dxof \
	dlls/d3dxof/tests \
	dlls/dataexchange \
	dlls/dataexchange/tests \
	dlls/davclnt \
	dlls/dbgeng \
	dlls/dbgeng/tests \
	dlls/dbghelp \
	dlls/dbghelp/tests \
	dlls/dciman32 \
	dlls/dcomp \
	dlls/ddeml.dll16 \
	dlls/ddraw \
	dlls/ddraw/tests \
	dlls/ddrawex \
	dlls/ddrawex/tests \
	dlls/desk.cpl \
	dlls/devenum \
	dlls/devenum/tests \
	dlls/dhcpcsvc \
	dlls/dhcpcsvc/tests \
	dlls/dhcpcsvc6 \
	dlls/dhtmled.ocx \
	dlls/diasymreader \
	dlls/difxapi \
	dlls/dinput \
	dlls/dinput/tests \
	dlls/dinput8 \
	dlls/directmanipulation \
	dlls/directmanipulation/tests \
	dlls/dispdib.dll16 \
	dlls/dispex \
	dlls/dispex/tests \
	dlls/display.drv16 \
	dlls/dmband \
	dlls/dmband/tests \
	dlls/dmcompos \
	dlls/dmcompos/tests \
	dlls/dmime \
	dlls/dmime/tests \
	dlls/dmloader \
	dlls/dmloader/tests \
	dlls/dmscript \
	dlls/dmscript/tests \
	dlls/dmstyle \
	dlls/dmstyle/tests \
	dlls/dmsynth \
	dlls/dmsynth/tests \
	dlls/dmusic \
	dlls/dmusic/tests \
	dlls/dmusic32 \
	dlls/dnsapi \
	dlls/dnsapi/tests \
	dlls/dplay \
	dlls/dplayx \
	dlls/dplayx/tests \
	dlls/dpnaddr \
	dlls/dpnet \
	dlls/dpnet/tests \
	dlls/dpnhpast \
	dlls/dpnhupnp \
	dlls/dpnlobby \
	dlls/dpvoice \
	dlls/dpvoice/tests \
	dlls/dpwsockx \
	dlls/drmclien \
	dlls/dsdmo \
	dlls/dsdmo/tests \
	dlls/dsound \
	dlls/dsound/tests \
	dlls/dsquery \
	dlls/dssenh \
	dlls/dssenh/tests \
	dlls/dsuiext \
	dlls/dswave \
	dlls/dswave/tests \
	dlls/dwmapi \
	dlls/dwmapi/tests \
	dlls/dwrite \
	dlls/dwrite/tests \
	dlls/dx8vb \
	dlls/dxcore \
	dlls/dxdiagn \
	dlls/dxdiagn/tests \
	dlls/dxgi \
	dlls/dxgi/tests \
	dlls/dxtrans \
	dlls/dxva2 \
	dlls/dxva2/tests \
	dlls/esent \
	dlls/evr \
	dlls/evr/tests \
	dlls/explorerframe \
	dlls/explorerframe/tests \
	dlls/faultrep \
	dlls/faultrep/tests \
	dlls/feclient \
	dlls/fltlib \
	dlls/fltmgr.sys \
	dlls/fntcache \
	dlls/fontsub \
	dlls/fusion \
	dlls/fusion/tests \
	dlls/fwpuclnt \
	dlls/gameux \
	dlls/gameux/tests \
	dlls/gamingtcui \
	dlls/gdi.exe16 \
	dlls/gdi32 \
	dlls/gdi32/tests \
	dlls/gdiplus \
	dlls/gdiplus/tests \
	dlls/geolocation \
	dlls/geolocation/tests \
	dlls/glu32 \
	dlls/glu32/tests \
	dlls/gphoto2.ds \
	dlls/gpkcsp \
	dlls/graphicscapture \
	dlls/graphicscapture/tests \
	dlls/hal \
	dlls/hhctrl.ocx \
	dlls/hid \
	dlls/hid/tests \
	dlls/hidclass.sys \
	dlls/hidparse.sys \
	dlls/hlink \
	dlls/hlink/tests \
	dlls/hnetcfg \
	dlls/hnetcfg/tests \
	dlls/hrtfapo \
	dlls/http.sys \
	dlls/httpapi \
	dlls/httpapi/tests \
	dlls/hvsimanagementapi \
	dlls/hvsimanagementapi/tests \
	dlls/ia2comproxy \
	dlls/iccvid \
	dlls/icmp \
	dlls/icmui \
	dlls/icmui/tests \
	dlls/icu \
	dlls/ieframe \
	dlls/ieframe/tests \
	dlls/ieproxy \
	dlls/iertutil \
	dlls/iertutil/tests \
	dlls/ifsmgr.vxd \
	dlls/imaadp32.acm \
	dlls/imagehlp \
	dlls/imagehlp/tests \
	dlls/imm.dll16 \
	dlls/imm32 \
	dlls/imm32/tests \
	dlls/inetcomm \
	dlls/inetcomm/tests \
	dlls/inetcpl.cpl \
	dlls/inetmib1 \
	dlls/inetmib1/tests \
	dlls/infosoft \
	dlls/infosoft/tests \
	dlls/initpki \
	dlls/inkobj \
	dlls/inseng \
	dlls/iphlpapi \
	dlls/iphlpapi/tests \
	dlls/iprop \
	dlls/irprops.cpl \
	dlls/itircl \
	dlls/itss \
	dlls/itss/tests \
	dlls/joy.cpl \
	dlls/jscript \
	dlls/jscript/tests \
	dlls/jsproxy \
	dlls/jsproxy/tests \
	dlls/kerberos \
	dlls/kernel32 \
	dlls/kernel32/tests \
	dlls/kernelbase \
	dlls/kernelbase/tests \
	dlls/keyboard.drv16 \
	dlls/krnl386.exe16 \
	dlls/ksecdd.sys \
	dlls/ksproxy.ax \
	dlls/ksuser \
	dlls/ktmw32 \
	dlls/l3codeca.acm \
	dlls/l3codecx.ax \
	dlls/light.msstyles \
	dlls/loadperf \
	dlls/localspl \
	dlls/localspl/tests \
	dlls/localui \
	dlls/localui/tests \
	dlls/lz32 \
	dlls/lz32/tests \
	dlls/lzexpand.dll16 \
	dlls/magnification \
	dlls/mapi32 \
	dlls/mapi32/tests \
	dlls/mapistub \
	dlls/mciavi32 \
	dlls/mcicda \
	dlls/mciqtz32 \
	dlls/mciseq \
	dlls/mciwave \
	dlls/mf \
	dlls/mf/tests \
	dlls/mf3216 \
	dlls/mfasfsrcsnk \
	dlls/mferror \
	dlls/mfh264enc \
	dlls/mfmediaengine \
	dlls/mfmediaengine/tests \
	dlls/mfmp4srcsnk \
	dlls/mfplat \
	dlls/mfplat/tests \
	dlls/mfplay \
	dlls/mfplay/tests \
	dlls/mfreadwrite \
	dlls/mfreadwrite/tests \
	dlls/mfsrcsnk \
	dlls/mfsrcsnk/tests \
	dlls/mgmtapi \
	dlls/midimap \
	dlls/mlang \
	dlls/mlang/tests \
	dlls/mmcndmgr \
	dlls/mmcndmgr/tests \
	dlls/mmdevapi \
	dlls/mmdevapi/tests \
	dlls/mmdevldr.vxd \
	dlls/mmsystem.dll16 \
	dlls/monodebg.vxd \
	dlls/mouhid.sys \
	dlls/mountmgr.sys \
	dlls/mouse.drv16 \
	dlls/mp3dmod \
	dlls/mp3dmod/tests \
	dlls/mpr \
	dlls/mpr/tests \
	dlls/mprapi \
	dlls/msacm.dll16 \
	dlls/msacm32.drv \
	dlls/msacm32 \
	dlls/msacm32/tests \
	dlls/msado15 \
	dlls/msado15/tests \
	dlls/msadp32.acm \
	dlls/msasn1 \
	dlls/msasn1/tests \
	dlls/msauddecmft \
	dlls/mscat32 \
	dlls/mscms \
	dlls/mscms/tests \
	dlls/mscoree \
	dlls/mscoree/tests \
	dlls/mscorwks \
	dlls/msctf \
	dlls/msctf/tests \
	dlls/msctfmonitor \
	dlls/msctfp \
	dlls/msdaps \
	dlls/msdasql \
	dlls/msdasql/tests \
	dlls/msdelta \
	dlls/msdmo \
	dlls/msdmo/tests \
	dlls/msdrm \
	dlls/msftedit \
	dlls/msftedit/tests \
	dlls/msg711.acm \
	dlls/msgsm32.acm \
	dlls/mshtml.tlb \
	dlls/mshtml \
	dlls/mshtml/tests \
	dlls/msi \
	dlls/msi/tests \
	dlls/msident \
	dlls/msimg32 \
	dlls/msimsg \
	dlls/msimtf \
	dlls/msisip \
	dlls/msisys.ocx \
	dlls/msls31 \
	dlls/msmpeg2vdec \
	dlls/msnet32 \
	dlls/mspatcha \
	dlls/mspatcha/tests \
	dlls/msports \
	dlls/msrle32 \
	dlls/msrle32/tests \
	dlls/msscript.ocx \
	dlls/msscript.ocx/tests \
	dlls/mssign32 \
	dlls/mssip32 \
	dlls/mstask \
	dlls/mstask/tests \
	dlls/msttsengine \
	dlls/msv1_0 \
	dlls/msvcirt \
	dlls/msvcirt/tests \
	dlls/msvcm80 \
	dlls/msvcm90 \
	dlls/msvcp100 \
	dlls/msvcp100/tests \
	dlls/msvcp110 \
	dlls/msvcp110/tests \
	dlls/msvcp120 \
	dlls/msvcp120/tests \
	dlls/msvcp120_app \
	dlls/msvcp140 \
	dlls/msvcp140/tests \
	dlls/msvcp140_1 \
	dlls/msvcp140_1/tests \
	dlls/msvcp140_2 \
	dlls/msvcp140_2/tests \
	dlls/msvcp140_atomic_wait \
	dlls/msvcp140_atomic_wait/tests \
	dlls/msvcp140_codecvt_ids \
	dlls/msvcp60 \
	dlls/msvcp60/tests \
	dlls/msvcp70 \
	dlls/msvcp71 \
	dlls/msvcp80 \
	dlls/msvcp90 \
	dlls/msvcp90/tests \
	dlls/msvcp_win \
	dlls/msvcr100 \
	dlls/msvcr100/tests \
	dlls/msvcr110 \
	dlls/msvcr110/tests \
	dlls/msvcr120 \
	dlls/msvcr120/tests \
	dlls/msvcr120_app \
	dlls/msvcr70 \
	dlls/msvcr70/tests \
	dlls/msvcr71 \
	dlls/msvcr71/tests \
	dlls/msvcr80 \
	dlls/msvcr80/tests \
	dlls/msvcr90 \
	dlls/msvcr90/tests \
	dlls/msvcrt \
	dlls/msvcrt/tests \
	dlls/msvcrt20 \
	dlls/msvcrt40 \
	dlls/msvcrtd \
	dlls/msvcrtd/tests \
	dlls/msvfw32 \
	dlls/msvfw32/tests \
	dlls/msvidc32 \
	dlls/msvideo.dll16 \
	dlls/msvdsp \
	dlls/msvproc \
	dlls/mswsock \
	dlls/msxml \
	dlls/msxml2 \
	dlls/msxml3 \
	dlls/msxml3/tests \
	dlls/msxml4 \
	dlls/msxml4/tests \
	dlls/msxml6 \
	dlls/msxml6/tests \
	dlls/mtxdm \
	dlls/ncrypt \
	dlls/ncrypt/tests \
	dlls/nddeapi \
	dlls/ndis.sys \
	dlls/ndis.sys/tests \
	dlls/netapi32 \
	dlls/netapi32/tests \
	dlls/netcfgx \
	dlls/netcfgx/tests \
	dlls/netio.sys \
	dlls/netprofm \
	dlls/netprofm/tests \
	dlls/netutils \
	dlls/newdev \
	dlls/ninput \
	dlls/ninput/tests \
	dlls/normaliz \
	dlls/npmshtml \
	dlls/npptools \
	dlls/nsi \
	dlls/nsi/tests \
	dlls/nsiproxy.sys \
	dlls/ntdll \
	dlls/ntdll/tests \
	dlls/ntdsapi \
	dlls/ntdsapi/tests \
	dlls/ntoskrnl.exe \
	dlls/ntoskrnl.exe/tests \
	dlls/ntprint \
	dlls/ntprint/tests \
	dlls/nvcuda \
	dlls/objsel \
	dlls/odbc32 \
	dlls/odbc32/tests \
	dlls/odbcbcp \
	dlls/odbccp32 \
	dlls/odbccp32/tests \
	dlls/odbccu32 \
	dlls/ole2.dll16 \
	dlls/ole2conv.dll16 \
	dlls/ole2disp.dll16 \
	dlls/ole2nls.dll16 \
	dlls/ole2prox.dll16 \
	dlls/ole2thk.dll16 \
	dlls/ole32 \
	dlls/ole32/tests \
	dlls/oleacc \
	dlls/oleacc/tests \
	dlls/oleaut32 \
	dlls/oleaut32/tests \
	dlls/olecli.dll16 \
	dlls/olecli32 \
	dlls/oledb32 \
	dlls/oledb32/tests \
	dlls/oledlg \
	dlls/oledlg/tests \
	dlls/olepro32 \
	dlls/olesvr.dll16 \
	dlls/olesvr32 \
	dlls/olethk32 \
	dlls/opcservices \
	dlls/opcservices/tests \
	dlls/opencl \
	dlls/opengl32 \
	dlls/opengl32/tests \
	dlls/packager \
	dlls/packager/tests \
	dlls/pdh \
	dlls/pdh/tests \
	dlls/photometadatahandler \
	dlls/pidgen \
	dlls/powrprof \
	dlls/printui \
	dlls/prntvpt \
	dlls/prntvpt/tests \
	dlls/profapi \
	dlls/propsys \
	dlls/propsys/tests \
	dlls/protontts \
	dlls/psapi \
	dlls/psapi/tests \
	dlls/pstorec \
	dlls/pstorec/tests \
	dlls/pwrshplugin \
	dlls/qasf \
	dlls/qasf/tests \
	dlls/qcap \
	dlls/qcap/tests \
	dlls/qdvd \
	dlls/qdvd/tests \
	dlls/qedit \
	dlls/qedit/tests \
	dlls/qmgr \
	dlls/qmgr/tests \
	dlls/qmgrprxy \
	dlls/quartz \
	dlls/quartz/tests \
	dlls/query \
	dlls/qwave \
	dlls/qwave/tests \
	dlls/rasapi16.dll16 \
	dlls/rasapi32 \
	dlls/rasapi32/tests \
	dlls/rasdlg \
	dlls/regapi \
	dlls/resampledmo \
	dlls/resutils \
	dlls/riched20 \
	dlls/riched20/tests \
	dlls/riched32 \
	dlls/riched32/tests \
	dlls/rometadata \
	dlls/rometadata/tests \
	dlls/rpcrt4 \
	dlls/rpcrt4/tests \
	dlls/rsabase \
	dlls/rsaenh \
	dlls/rsaenh/tests \
	dlls/rstrtmgr \
	dlls/rtutils \
	dlls/rtworkq \
	dlls/rtworkq/tests \
	dlls/samlib \
	dlls/sane.ds \
	dlls/sapi \
	dlls/sapi/tests \
	dlls/sas \
	dlls/scarddlg \
	dlls/scardsvr \
	dlls/sccbase \
	dlls/schannel \
	dlls/schannel/tests \
	dlls/schedsvc \
	dlls/schedsvc/tests \
	dlls/scrobj \
	dlls/scrobj/tests \
	dlls/scrrun \
	dlls/scrrun/tests \
	dlls/scsiport.sys \
	dlls/sechost \
	dlls/secur32 \
	dlls/secur32/tests \
	dlls/security \
	dlls/sensapi \
	dlls/serialui \
	dlls/serialui/tests \
	dlls/setupapi \
	dlls/setupapi/tests \
	dlls/setupx.dll16 \
	dlls/sfc \
	dlls/sfc_os \
	dlls/sharedgpures.sys \
	dlls/shcore \
	dlls/shcore/tests \
	dlls/shdoclc \
	dlls/shdocvw \
	dlls/shdocvw/tests \
	dlls/shell.dll16 \
	dlls/shell32 \
	dlls/shell32/tests \
	dlls/shfolder \
	dlls/shlwapi \
	dlls/shlwapi/tests \
	dlls/slbcsp \
	dlls/slc \
	dlls/slc/tests \
	dlls/snmpapi \
	dlls/snmpapi/tests \
	dlls/softpub \
	dlls/sound.drv16 \
	dlls/spoolss \
	dlls/spoolss/tests \
	dlls/sppc \
	dlls/srclient \
	dlls/srvcli \
	dlls/srvsvc \
	dlls/sspicli \
	dlls/stdole2.tlb \
	dlls/stdole32.tlb \
	dlls/sti \
	dlls/sti/tests \
	dlls/storage.dll16 \
	dlls/stress.dll16 \
	dlls/strmdll \
	dlls/svrapi \
	dlls/sxs \
	dlls/sxs/tests \
	dlls/system.drv16 \
	dlls/t2embed \
	dlls/t2embed/tests \
	dlls/tapi32 \
	dlls/tapi32/tests \
	dlls/taskschd \
	dlls/taskschd/tests \
	dlls/tbs \
	dlls/tdh \
	dlls/tdi.sys \
	dlls/threadpoolwinrt \
	dlls/threadpoolwinrt/tests \
	dlls/toolhelp.dll16 \
	dlls/traffic \
	dlls/twain.dll16 \
	dlls/twain_32 \
	dlls/twain_32/tests \
	dlls/twinapi.appcore \
	dlls/twinapi.appcore/tests \
	dlls/typelib.dll16 \
	dlls/tzres \
	dlls/ucrtbase \
	dlls/ucrtbase/tests \
	dlls/uianimation \
	dlls/uianimation/tests \
	dlls/uiautomationcore \
	dlls/uiautomationcore/tests \
	dlls/uiribbon \
	dlls/unicows \
	dlls/updspapi \
	dlls/url \
	dlls/urlmon \
	dlls/urlmon/tests \
	dlls/usbd.sys \
	dlls/user.exe16 \
	dlls/user32 \
	dlls/user32/tests \
	dlls/userenv \
	dlls/userenv/tests \
	dlls/usp10 \
	dlls/usp10/tests \
	dlls/utildll \
	dlls/uxtheme \
	dlls/uxtheme/tests \
	dlls/vbscript \
	dlls/vbscript/tests \
	dlls/vcomp \
	dlls/vcomp/tests \
	dlls/vcomp100 \
	dlls/vcomp110 \
	dlls/vcomp110/tests \
	dlls/vcomp120 \
	dlls/vcomp140 \
	dlls/vcomp90 \
	dlls/vcruntime140 \
	dlls/vcruntime140_1 \
	dlls/vdhcp.vxd \
	dlls/vdmdbg \
	dlls/ver.dll16 \
	dlls/version \
	dlls/version/tests \
	dlls/vga \
	dlls/vidreszr \
	dlls/virtdisk \
	dlls/virtdisk/tests \
	dlls/vmm.vxd \
	dlls/vnbt.vxd \
	dlls/vnetbios.vxd \
	dlls/vssapi \
	dlls/vtdapi.vxd \
	dlls/vulkan-1 \
	dlls/vulkan-1/tests \
	dlls/vwin32.vxd \
	dlls/w32skrnl \
	dlls/w32sys.dll16 \
	dlls/wbemdisp \
	dlls/wbemdisp/tests \
	dlls/wbemprox \
	dlls/wbemprox/tests \
	dlls/wdscore \
	dlls/webservices \
	dlls/webservices/tests \
	dlls/websocket \
	dlls/wer \
	dlls/wer/tests \
	dlls/wevtapi \
	dlls/wevtapi/tests \
	dlls/wevtsvc \
	dlls/wiaservc \
	dlls/wiaservc/tests \
	dlls/wimgapi \
	dlls/win32s16.dll16 \
	dlls/win32u \
	dlls/win32u/tests \
	dlls/win87em.dll16 \
	dlls/winaspi.dll16 \
	dlls/windebug.dll16 \
	dlls/windows.applicationmodel \
	dlls/windows.applicationmodel/tests \
	dlls/windows.devices.bluetooth \
	dlls/windows.devices.bluetooth/tests \
	dlls/windows.devices.enumeration \
	dlls/windows.devices.enumeration/tests \
	dlls/windows.devices.usb \
	dlls/windows.devices.usb/tests \
	dlls/windows.gaming.input \
	dlls/windows.gaming.input/tests \
	dlls/windows.gaming.ui.gamebar \
	dlls/windows.gaming.ui.gamebar/tests \
	dlls/windows.globalization \
	dlls/windows.globalization/tests \
	dlls/windows.media.devices \
	dlls/windows.media.devices/tests \
	dlls/windows.media.mediacontrol \
	dlls/windows.media.mediacontrol/tests \
	dlls/windows.media.speech \
	dlls/windows.media.speech/tests \
	dlls/windows.media \
	dlls/windows.media/tests \
	dlls/windows.networking.connectivity \
	dlls/windows.networking.connectivity/tests \
	dlls/windows.networking.hostname \
	dlls/windows.networking.hostname/tests \
	dlls/windows.networking \
	dlls/windows.perception.stub \
	dlls/windows.perception.stub/tests \
	dlls/windows.security.authentication.onlineid \
	dlls/windows.security.authentication.onlineid/tests \
	dlls/windows.security.credentials.ui.userconsentverifier \
	dlls/windows.security.credentials.ui.userconsentverifier/tests \
	dlls/windows.storage.applicationdata \
	dlls/windows.storage.applicationdata/tests \
	dlls/windows.system.profile.systemmanufacturers \
	dlls/windows.system.profile.systemmanufacturers/tests \
	dlls/windows.ui \
	dlls/windows.ui/tests \
	dlls/windows.web \
	dlls/windows.web/tests \
	dlls/windowscodecs \
	dlls/windowscodecs/tests \
	dlls/windowscodecsext \
	dlls/windowscodecsext/tests \
	dlls/winealsa.drv \
	dlls/wineandroid.drv \
	dlls/winebth.sys \
	dlls/winebus.sys \
	dlls/winecoreaudio.drv \
	dlls/winecrt0 \
	dlls/wined3d \
	dlls/winedmo \
	dlls/winegstreamer \
	dlls/winehid.sys \
	dlls/winemac.drv \
	dlls/winemapi \
	dlls/wineoss.drv \
	dlls/wineps.drv \
	dlls/wineps16.drv16 \
	dlls/winepulse.drv \
	dlls/wineusb.sys \
	dlls/winevulkan \
	dlls/winewayland.drv \
	dlls/winex11.drv \
	dlls/winexinput.sys \
	dlls/wing.dll16 \
	dlls/wing32 \
	dlls/wing32/tests \
	dlls/winhttp \
	dlls/winhttp/tests \
	dlls/wininet \
	dlls/wininet/tests \
	dlls/winmm \
	dlls/winmm/tests \
	dlls/winnls.dll16 \
	dlls/winnls32 \
	dlls/winprint \
	dlls/winscard \
	dlls/winscard/tests \
	dlls/winsock.dll16 \
	dlls/winspool.drv \
	dlls/winspool.drv/tests \
	dlls/winsta \
	dlls/wintab.dll16 \
	dlls/wintab32 \
	dlls/wintab32/tests \
	dlls/wintrust \
	dlls/wintrust/tests \
	dlls/wintypes \
	dlls/wintypes/tests \
	dlls/winusb \
	dlls/wlanapi \
	dlls/wlanapi/tests \
	dlls/wlanui \
	dlls/wldap32 \
	dlls/wldap32/tests \
	dlls/wldp \
	dlls/wldp/tests \
	dlls/wmadmod \
	dlls/wmasf \
	dlls/wmi \
	dlls/wmilib.sys \
	dlls/wmiutils \
	dlls/wmiutils/tests \
	dlls/wmp \
	dlls/wmp/tests \
	dlls/wmphoto \
	dlls/wmvcore \
	dlls/wmvcore/tests \
	dlls/wmvdecod \
	dlls/wnaspi32 \
	dlls/wofutil \
	dlls/wow32 \
	dlls/wow64 \
	dlls/wow64cpu \
	dlls/wow64win \
	dlls/wpc \
	dlls/wpc/tests \
	dlls/wpcap \
	dlls/wpcap/tests \
	dlls/ws2_32 \
	dlls/ws2_32/tests \
	dlls/wsdapi \
	dlls/wsdapi/tests \
	dlls/wshom.ocx \
	dlls/wshom.ocx/tests \
	dlls/wsnmp32 \
	dlls/wsnmp32/tests \
	dlls/wsock32 \
	dlls/wtsapi32 \
	dlls/wtsapi32/tests \
	dlls/wuapi \
	dlls/wuaueng \
	dlls/x3daudio1_0 \
	dlls/x3daudio1_1 \
	dlls/x3daudio1_2 \
	dlls/x3daudio1_3 \
	dlls/x3daudio1_4 \
	dlls/x3daudio1_5 \
	dlls/x3daudio1_6 \
	dlls/x3daudio1_7 \
	dlls/xactengine2_0 \
	dlls/xactengine2_4 \
	dlls/xactengine2_7 \
	dlls/xactengine2_9 \
	dlls/xactengine3_0 \
	dlls/xactengine3_1 \
	dlls/xactengine3_2 \
	dlls/xactengine3_3 \
	dlls/xactengine3_4 \
	dlls/xactengine3_5 \
	dlls/xactengine3_6 \
	dlls/xactengine3_7 \
	dlls/xactengine3_7/tests \
	dlls/xapofx1_1 \
	dlls/xapofx1_2 \
	dlls/xapofx1_3 \
	dlls/xapofx1_4 \
	dlls/xapofx1_5 \
	dlls/xaudio2_0 \
	dlls/xaudio2_1 \
	dlls/xaudio2_2 \
	dlls/xaudio2_3 \
	dlls/xaudio2_4 \
	dlls/xaudio2_5 \
	dlls/xaudio2_6 \
	dlls/xaudio2_7 \
	dlls/xaudio2_7/tests \
	dlls/xaudio2_8 \
	dlls/xaudio2_8/tests \
	dlls/xaudio2_9 \
	dlls/xinput1_1 \
	dlls/xinput1_2 \
	dlls/xinput1_3 \
	dlls/xinput1_3/tests \
	dlls/xinput1_4 \
	dlls/xinput9_1_0 \
	dlls/xinputuap \
	dlls/xmllite \
	dlls/xmllite/tests \
	dlls/xolehlp \
	dlls/xpsprint \
	dlls/xpssvcs \
	dlls/xtajit64 \
	fonts \
	include \
	libs/adsiid \
	libs/capstone \
	libs/dmoguids \
	libs/dxerr8 \
	libs/dxerr9 \
	libs/dxguid \
	libs/faudio \
	libs/fluidsynth \
	libs/gsm \
	libs/jpeg \
	libs/jxr \
	libs/lcms2 \
	libs/ldap \
	libs/mfuuid \
	libs/mpg123 \
	libs/musl \
	libs/png \
	libs/strmbase \
	libs/strmiids \
	libs/tiff \
	libs/tomcrypt \
	libs/uuid \
	libs/vkd3d \
	libs/wbemuuid \
	libs/wmcodecdspuuid \
	libs/xml2 \
	libs/xslt \
	libs/zlib \
	loader \
	nls \
	po \
	programs/arp \
	programs/aspnet_regiis \
	programs/attrib \
	programs/belauncher \
	programs/cabarc \
	programs/cacls \
	programs/certutil \
	programs/chcp.com \
	programs/clock \
	programs/cmd \
	programs/cmd/tests \
	programs/conhost \
	programs/conhost/tests \
	programs/control \
	programs/cscript \
	programs/dism \
	programs/dllhost \
	programs/dplaysvr \
	programs/dotnetfx35 \
	programs/dpnsvr \
	programs/dpvsetup \
	programs/dxdiag \
	programs/eject \
	programs/expand \
	programs/explorer \
	programs/explorer/tests \
	programs/extrac32 \
	programs/fc \
	programs/fc/tests \
	programs/find \
	programs/find/tests \
	programs/findstr \
	programs/findstr/tests \
	programs/fsutil \
	programs/fsutil/tests \
	programs/getminidump \
	programs/hh \
	programs/hostname \
	programs/icacls \
	programs/icinfo \
	programs/iexplore \
	programs/ipconfig \
	programs/klist \
	programs/lodctr \
	programs/mofcomp \
	programs/mshta \
	programs/msidb \
	programs/msiexec \
	programs/msinfo32 \
	programs/net \
	programs/net/tests \
	programs/netsh \
	programs/netstat \
	programs/ngen \
	programs/notepad \
	programs/oleview \
	programs/ping \
	programs/plugplay \
	programs/pnputil \
	programs/powershell \
	programs/presentationfontcache \
	programs/progman \
	programs/reg \
	programs/reg/tests \
	programs/regasm \
	programs/regedit \
	programs/regedit/tests \
	programs/regini \
	programs/regsvcs \
	programs/regsvr32 \
	programs/robocopy \
	programs/rpcss \
	programs/rundll.exe16 \
	programs/rundll32 \
	programs/sc \
	programs/sc/tests \
	programs/schtasks \
	programs/schtasks/tests \
	programs/sdbinst \
	programs/secedit \
	programs/servicemodelreg \
	programs/services \
	programs/services/tests \
	programs/setx \
	programs/shutdown \
	programs/sort \
	programs/spoolsv \
	programs/start \
	programs/subst \
	programs/svchost \
	programs/systeminfo \
	programs/tabtip \
	programs/taskkill \
	programs/tasklist \
	programs/tasklist/tests \
	programs/taskmgr \
	programs/termsv \
	programs/uninstaller \
	programs/unlodctr \
	programs/view \
	programs/wevtutil \
	programs/where \
	programs/whoami \
	programs/wineboot \
	programs/winebrowser \
	programs/winecfg \
	programs/wineconsole \
	programs/winedbg \
	programs/winedevice \
	programs/winefile \
	programs/winemenubuilder \
	programs/winemine \
	programs/winemsibuilder \
	programs/winepath \
	programs/winetest \
	programs/winevdm \
	programs/winhelp.exe16 \
	programs/winhlp32 \
	programs/winmgmt \
	programs/winoldap.mod16 \
	programs/winver \
	programs/wmic \
	programs/wmplayer \
	programs/wordpad \
	programs/write \
	programs/wscript \
	programs/wscript/tests \
	programs/wuauserv \
	programs/wusa \
	programs/xcopy \
	programs/xcopy/tests \
	server \
	tools \
	tools/sfnt2fon \
	tools/widl \
	tools/winebuild \
	tools/winedump \
	tools/winegcc \
	tools/winemaker \
	tools/wmc \
	tools/wrc
DISABLED_SUBDIRS =  dlls/capi2032 dlls/gphoto2.ds dlls/protontts dlls/sane.ds dlls/winealsa.drv dlls/wineandroid.drv dlls/winecoreaudio.drv dlls/winegstreamer dlls/winemac.drv dlls/wineoss.drv dlls/winepulse.drv dlls/wineusb.sys dlls/winewayland.drv dlls/winex11.drv dlls/winscard dlls/wpcap fonts po
CONFIGURE_TARGETS =  loader/wine-preloader wine wine64 TAGS tags autom4te.cache compile_commands.json config.log config.status include/config.h include/stamp-h loader/wine
CARBON_LIBS = 
COREFOUNDATION_LIBS = 
DISKARBITRATION_LIBS = 
IOKIT_LIBS = 
METAL_LIBS = 
APPLICATIONSERVICES_LIBS = 
CORESERVICES_LIBS = 
APPKIT_LIBS = 
SECURITY_LIBS = 
SYSTEMCONFIGURATION_LIBS = 
COREAUDIO_LIBS = 
OPENCL_LIBS = 
OBJC = 
OBJCFLAGS = 
ac_ct_OBJC = 
WINELOADER_DEPENDS = 
PE_ARCHS =  x86_64
MINGW_PKG_CONFIG = 
CAPSTONE_PE_CFLAGS = -I$(top_srcdir)/libs/capstone/include
CAPSTONE_PE_LIBS = capstone
FAUDIO_PE_CFLAGS = -I$(top_srcdir)/libs/faudio/include
FAUDIO_PE_LIBS = faudio mfplat mfreadwrite mfuuid propsys
FLUIDSYNTH_PE_CFLAGS = -I$(top_srcdir)/libs/fluidsynth/include
FLUIDSYNTH_PE_LIBS = fluidsynth
GSM_PE_CFLAGS = -I$(top_srcdir)/libs/gsm/inc
GSM_PE_LIBS = gsm
JPEG_PE_CFLAGS = -I$(top_srcdir)/libs/jpeg
JPEG_PE_LIBS = jpeg
JXR_PE_CFLAGS = -I$(top_srcdir)/libs/jxr/jxrgluelib -I$(top_srcdir)/libs/jxr/image/sys
JXR_PE_LIBS = jxr
LCMS2_PE_CFLAGS = -I$(top_srcdir)/libs/lcms2/include
LCMS2_PE_LIBS = lcms2
LDAP_PE_CFLAGS = -I$(top_srcdir)/libs/ldap/include
LDAP_PE_LIBS = ldap
MPG123_PE_CFLAGS = -I$(top_srcdir)/libs/mpg123/src/include
MPG123_PE_LIBS = mpg123
MUSL_PE_CFLAGS = 
MUSL_PE_LIBS = musl
PNG_PE_CFLAGS = -I$(top_srcdir)/libs/png
PNG_PE_LIBS = png $(ZLIB_PE_LIBS)
TIFF_PE_CFLAGS = -I$(top_srcdir)/libs/tiff/libtiff
TIFF_PE_LIBS = tiff $(ZLIB_PE_LIBS)
TOMCRYPT_PE_CFLAGS = -I$(top_srcdir)/libs/tomcrypt/src/headers -DLTC_NO_PROTOTYPES -DLTC_SOURCE
TOMCRYPT_PE_LIBS = tomcrypt
VKD3D_PE_CFLAGS = -I$(top_srcdir)/libs/vkd3d/include
VKD3D_PE_LIBS = vkd3d
XML2_PE_CFLAGS = -I$(top_srcdir)/libs/xml2/include -DLIBXML_STATIC
XML2_PE_LIBS = xml2
XSLT_PE_CFLAGS = -I$(top_srcdir)/libs/xslt/libxslt -I$(top_srcdir)/libs/xslt -DLIBXSLT_STATIC
XSLT_PE_LIBS = xslt
ZLIB_PE_CFLAGS = -I$(top_srcdir)/libs/zlib -DFAR= -DZ_SOLO
ZLIB_PE_LIBS = z
PTHREAD_LIBS = 
XMKMF = 
CPP = gcc -m64 -E
X_CFLAGS = 
X_LIBS = 
WAYLAND_CLIENT_CFLAGS = 
WAYLAND_CLIENT_LIBS = 
WAYLAND_SCANNER = 
XKBCOMMON_CFLAGS = 
XKBCOMMON_LIBS = 
XKBREGISTRY_CFLAGS = 
XKBREGISTRY_LIBS = 
EGL_CFLAGS = 
EGL_LIBS = -lEGL
WAYLAND_EGL_CFLAGS = 
WAYLAND_EGL_LIBS = 
PCAP_LIBS = 
PCSCLITE_LIBS = 
INOTIFY_CFLAGS = 
INOTIFY_LIBS = 
DBUS_CFLAGS = 
DBUS_LIBS = 
GNUTLS_CFLAGS = 
GNUTLS_LIBS = -lgnutls
DRM_CFLAGS = 
DRM_LIBS = 
DRMAMDGPU_CFLAGS = 
DRMAMDGPU_LIBS = 
GMP_CFLAGS = 
GMP_LIBS = -lgmp 
SANE_CFLAGS = 
SANE_LIBS = 
USB_CFLAGS = 
USB_LIBS = 
GPHOTO2_CFLAGS = 
GPHOTO2_LIBS = -lgphoto2
GPHOTO2_PORT_CFLAGS = 
GPHOTO2_PORT_LIBS = 
RESOLV_LIBS = -lresolv
FREETYPE_CFLAGS = 
FREETYPE_LIBS = 
GETTEXTPO_LIBS = 
PULSE_CFLAGS = 
PULSE_LIBS = 
FFMPEG_CFLAGS = 
FFMPEG_LIBS = 
GSTREAMER_CFLAGS = 
GSTREAMER_LIBS = 
ALSA_LIBS = 
OSS4_CFLAGS = 
OSS4_LIBS = 
UDEV_CFLAGS = 
UDEV_LIBS = 
UNWIND_CFLAGS = 
UNWIND_LIBS = 
SDL2_CFLAGS = 
SDL2_LIBS = -lSDL2
CAPI20_CFLAGS = 
CAPI20_LIBS = 
CUPS_CFLAGS = 
CUPS_LIBS = 
FONTCONFIG_CFLAGS = 
FONTCONFIG_LIBS = 
KRB5_CFLAGS = 
KRB5_LIBS = 
GSSAPI_CFLAGS = 
GSSAPI_LIBS = 
PROCSTAT_LIBS = 
NETAPI_CFLAGS = 
NETAPI_LIBS = 
GCRYPT_CFLAGS = 
GCRYPT_LIBS = 
PIPER_CFLAGS = 
PIPER_LIBS = 
MSVCRTFLAGS = -D_WIN32 -fno-builtin -fshort-wchar -Wno-format -mabi=ms
DELAYLOADFLAG = 
WINELOADER_PROGRAMS = wine64 wine64-preloader
RT_LIBS = 
TAGSFLAGS = --langmap='c:+.idl.l.rh,make:(Make*.in)'
LIBOBJS = 
LTLIBOBJS = 

all:
wine:
	@./config.status $@
wine64:
	@./config.status $@
all: wine
	@echo "Wine build complete."
Makefile: config.status
	@./config.status Makefile
config.status: configure
	@./config.status --recheck
include/config.h: include/stamp-h
include/stamp-h: include/config.h.in config.status
	@./config.status include/config.h include/stamp-h
distclean:: clean
	rm -rf autom4te.cache
maintainer-clean::
	rm -f configure include/config.h.in
dlls/ntdll/unix/version.c: dummy
	@version=`(GIT_DIR=.git git describe HEAD 2>/dev/null || echo "wine-$(PACKAGE_VERSION)") | sed -n -e '$$s/\(.*\)/const char wine_build[] = "\1";/p'` && (echo $$version | cmp -s - $@) || echo $$version >$@ || (rm -f $@ && exit 1)
programs/winetest/build.rc: dummy
	@build="STRINGTABLE { 1 \"`GIT_DIR=.git git rev-parse HEAD 2>/dev/null`\" }" && (echo $$build | cmp -s - $@) || echo $$build >$@ || (rm -f $@ && exit 1)
programs/winetest/build.nfo:
	@-$(CC) -v 2>$@
dlls/wineandroid.drv/wine-debug.apk: dlls/wineandroid.drv/build.gradle dlls/wineandroid.drv/AndroidManifest.xml dlls/wineandroid.drv/WineActivity.java dlls/wineandroid.drv/wine.svg
	cd dlls/wineandroid.drv && gradle -q -Psrcdir=. assembleDebug
	mv dlls/wineandroid.drv/build/outputs/apk/wine-debug.apk $@
TAGS etags:
	rm -f TAGS
	(test -d .git && git ls-files || find -L . -name '*.[ch]' -print) | xargs etags -a $(TAGSFLAGS)
tags ctags:
	rm -f tags
	(test -d .git && git ls-files || find -L . -name '*.[ch]' -print) | xargs ctags -a $(TAGSFLAGS)
dummy:
.PHONY: dummy

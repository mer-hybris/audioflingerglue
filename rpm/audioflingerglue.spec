%define strip /bin/true
%define __requires_exclude  ^.*$
%define __find_requires     %{nil}
%global debug_package       %{nil}
%define __provides_exclude_from ^.*$

%define _target_cpu %{device_rpm_architecture_string}

Name:          audioflingerglue
Summary:       Android AudioFlinger glue library
Version:       0.0.1
Release:       1
Group:         System/Libraries
License:       ASL 2.0
BuildRequires: ubu-trusty
BuildRequires: sudo-for-abuild
BuildRequires: droid-bin-src-full
Source0:       %{name}-%{version}.tgz
AutoReqProv:   no

%description
%{summary}

%package       devel
Summary:       audioflingerglue development headers
Group:         System/Libraries
Requires:      audioflingerglue = %{version}-%{release}
BuildArch:     noarch

%description   devel
%{summary}

%prep

%if %{?device_rpm_architecture_string:0}%{!?device_rpm_architecture_string:1}
echo "device_rpm_architecture_string is not defined"
exit -1
%endif

%setup -T -c -n audioflingerglue
sudo chown -R abuild:abuild /home/abuild/src/droid/
mv /home/abuild/src/droid/* .
mkdir -p external
pushd external
tar -zxf %SOURCE0
mv audioflingerglue* audioflingerglue
popd

%build
echo "building for %{device_rpm_architecture_string}"

# default to building whatever target arch defines
LIB_TARGET=libaudioflingerglue

%if "%{device_rpm_architecture_string}" == "aarch64"
    # noop
%else
    ANDROID_ARCH=`grep -h -m 1 "TARGET_ARCH *:=" device/*/*/*.mk | sed -e 's/ *TARGET_ARCH *:= *\([a-zA-Z0-9_]*\) */\1/'`
    if [ "$ANDROID_ARCH" == "arm64" ]; then
        LIB_TARGET=${LIB_TARGET}_32
    fi
%endif

droid-make %{?_smp_mflags} $LIB_TARGET miniafservice

%install

if [ -f out/target/product/*/system/lib64/libaudioflingerglue.so ]; then
DROIDLIB=lib64
else
DROIDLIB=lib
fi

mkdir -p $RPM_BUILD_ROOT/%{_libexecdir}/droid-hybris/system/$DROIDLIB/
mkdir -p $RPM_BUILD_ROOT/%{_libexecdir}/droid-hybris/system/bin/
mkdir -p $RPM_BUILD_ROOT/%{_includedir}/audioflingerglue/
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/audioflingerglue/

cp out/target/product/*/system/$DROIDLIB/libaudioflingerglue.so \
    $RPM_BUILD_ROOT/%{_libexecdir}/droid-hybris/system/$DROIDLIB/

cp out/target/product/*/system/bin/miniafservice \
    $RPM_BUILD_ROOT/%{_libexecdir}/droid-hybris/system/bin/

cp external/audioflingerglue/audioflingerglue.h $RPM_BUILD_ROOT/%{_includedir}/audioflingerglue/
sed -e "s/@TARGET_LIB_ARCH@/$DROIDLIB/" external/audioflingerglue/hybris.c.in > \
    $RPM_BUILD_ROOT/%{_datadir}/audioflingerglue/hybris.c

LIBAFSOLOC=$RPM_BUILD_ROOT/file.list
echo %{_libexecdir}/droid-hybris/system/$DROIDLIB/libaudioflingerglue.so > %{LIBAFSOLOC}

%files -f %{LIBAFSOLOC}
%defattr(-,root,root,-)
%{_libexecdir}/droid-hybris/system/bin/miniafservice

%files devel
%defattr(-,root,root,-)
%{_includedir}/audioflingerglue/*.h
%{_datadir}/audioflingerglue/hybris.c

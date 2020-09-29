%bcond_with wayland

Name:       com.samsung.dali-scene-launcher
Summary:    The DALi Scene Launcher
Version:    1.0.0
Release:    1
Group:      The DALi Scene Launcher
License:    Apache-2.0
URL:        https://github.com/dalihub/dali-scene-launcher.git
Source0:    %{name}-%{version}.tar.gz

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

BuildRequires:  cmake
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(capi-media-player)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  gettext-tools
BuildRequires:  pkgconfig(dali2-core)
BuildRequires:  pkgconfig(dali2-toolkit)
BuildRequires:  pkgconfig(dali2-adaptor)

BuildRequires:  libdli
BuildRequires:  libdli-devel

BuildRequires:  pkgconfig(libtzplatform-config)

%description
Example implementation of DLI scene loading. See README.md for more details.

##############################
# Preparation
##############################
%prep
%setup -q

%define dali_app_ro_dir       %TZ_SYS_RO_APP/com.samsung.dali-scene-launcher/
%define dali_xml_file_dir     %TZ_SYS_RO_PACKAGES
%define dali_icon_dir         %TZ_SYS_RO_ICONS
%define smack_rule_dir        %TZ_SYS_SMACK/accesses2.d/

%define dali_app_icon_dir     %{dali_app_ro_dir}share/icons/
%define dali_app_exe_dir      %{dali_app_ro_dir}bin/
%define dali_app_res_dir      %{dali_app_ro_dir}res/

##############################
# Build
##############################
%build
PREFIX="/usr"
CXXFLAGS+=" -Wall -g -O2"
LDFLAGS+=" -Wl,--rpath=$PREFIX/lib -Wl,--as-needed"

%ifarch %{arm}
CXXFLAGS+=" -D_ARCH_ARM_"
%endif

cd %{_builddir}/%{name}-%{version}/build/tizen

cmake \
%if 0%{?enable_debug}
	-DCMAKE_BUILD_TYPE=Debug \
%endif
	-DCMAKE_INSTALL_PREFIX=%{_prefix} \
	-DDALI_APP_DIR=%{dali_app_exe_dir} \
	-DDALI_APP_RES_DIR=%{dali_app_res_dir} \
	.

make %{?jobs:-j%jobs}

##############################
# Installation
##############################
%install
rm -rf %{buildroot}
cd build/tizen
%make_install

mkdir -p %{buildroot}%{dali_xml_file_dir}
cp -f %{_builddir}/%{name}-%{version}/%{name}.xml %{buildroot}%{dali_xml_file_dir}

mkdir -p %{buildroot}%{dali_icon_dir}
mv %{_builddir}/%{name}-%{version}/share/icons/%{name}.png %{buildroot}%{dali_icon_dir}

%if 0%{?enable_dali_smack_rules} && !%{with wayland}
mkdir -p %{buildroot}%{smack_rule_dir}
cp -f %{_builddir}/%{name}-%{version}/%{name}.rule %{buildroot}%{smack_rule_dir}
%endif

##############################
# Post Install
##############################
%post
/sbin/ldconfig
exit 0

##############################
# Post Uninstall
##############################
%postun
/sbin/ldconfig
exit 0

##############################
# Files in Binary Packages
##############################

%files
%if 0%{?enable_dali_smack_rules}
%manifest %{name}.manifest-smack
%else
%manifest %{name}.manifest
%endif
%defattr(-,root,root,-)
%{dali_app_exe_dir}/dali-scene-launcher
%{dali_app_res_dir}/*
%{dali_xml_file_dir}/%{name}.xml
%{dali_icon_dir}/*
%if 0%{?enable_dali_smack_rules} && !%{with wayland}
%config %{smack_rule_dir}/%{name}.rule
%endif
%license LICENSE

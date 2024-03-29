# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       harbour-freebible

# >> macros
%define __requires_exclude ^libquazip1.*$
# << macros

Summary:    Bible for offline learning
Version:    1.0.1
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/spiritEcosse/bible
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  cmake >= 3.19
BuildRequires:  git
BuildRequires:  qt5-qttest
BuildRequires:  desktop-file-utils
BuildRequires:  perl-IO-Compress
BuildRequires:  perl-Module-Load-Conditional
BuildRequires:  perl-CPAN
BuildRequires:  perl-IPC-Cmd
BuildRequires:  perl-IO-Compress
BuildRequires:  sdk-deploy-rpm
BuildRequires:  expect

%description
The FreeBible application is completely free for learning
and daily reading, and it is designed for everyone.

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export CMAKE_BUILD_TYPE=$(if [[ -z ${CMAKE_BUILD_TYPE+x} ]]; then echo "Debug"; else echo ${CMAKE_BUILD_TYPE}; fi;)
cmake -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTING=OFF -DCODE_COVERAGE=OFF
make
# << build pre

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
DESTDIR=%{buildroot} make install
mkdir -p %{_bindir}
# << install pre
# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_libdir}/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5Config.cmake
%{_libdir}/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5ConfigVersion.cmake
%{_libdir}/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5_SharedTargets*
%{_libdir}/cmake/SqliteOrm/SqliteOrmConfig.cmake
%{_libdir}/cmake/SqliteOrm/SqliteOrmConfigVersion.cmake
%{_libdir}/cmake/SqliteOrm/SqliteOrmTargets.cmake
%exclude %{_libdir}/pkgconfig/quazip1-qt5.pc
%{_libdir}/libquazip1-qt5*
%{_includedir}/sqlite_orm/sqlite_orm.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/JlCompress.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/ioapi.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/minizip_crypt.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quaadler32.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quachecksum32.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quacrc32.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quagzipfile.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quaziodevice.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazip.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazip_global.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazip_qt_compat.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazipdir.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazipfile.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazipfileinfo.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/quazipnewinfo.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/unzip.h
%{_includedir}/QuaZip-Qt5-1.3/quazip/zip.h

%{_bindir}/*
%{_datadir}/%{name}
%{_datadir}/%{name}/translations/
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
# << files


%changelog
* Wed Oct 11 2023 Ihor Shevchenko shevchenkcoigor@gmail.com - 1.0.1-1
- Properly hide nested content when the list shrinks.
- Took care of minor UI issues related to the nonstandard size of bottom blocks on the BooksPage.

* Sun Oct 01 2023 Ihor Shevchenko shevchenkcoigor@gmail.com - 1.0.0-1
- Initial release

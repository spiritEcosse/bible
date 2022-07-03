# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       bible

# >> macros
# << macros

Summary:    bible
Version:    0.1
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Source100:  bible.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   perl-CPAN
Requires:   perl-IPC-Cmd
Requires:   perl-IO-Compress
Requires:   sqlite
Requires:   qt5-qttest
Requires:   sdk-deploy-rpm
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(sqlite3)
BuildRequires:  cmake >= 3.5
BuildRequires:  git
BuildRequires:  qt5-qttest
BuildRequires:  sqlite-devel
BuildRequires:  desktop-file-utils
BuildRequires:  perl-IO-Compress

%description
Short description of my Sailfish OS Application


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTING=ON -DCODE_COVERAGE=ON
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
/usr/lib/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5Config.cmake
/usr/lib/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5ConfigVersion.cmake
/usr/lib/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5_SharedTargets-debug.cmake
/usr/lib/cmake/QuaZip-Qt5-1.3/QuaZip-Qt5_SharedTargets.cmake
/usr/lib/cmake/SqliteOrm/SqliteOrmConfig.cmake
/usr/lib/cmake/SqliteOrm/SqliteOrmConfigVersion.cmake
/usr/lib/cmake/SqliteOrm/SqliteOrmTargets.cmake
/usr/lib/libquazip1-qt5d.so
/usr/lib/libquazip1-qt5d.so.1.3.0
/usr/lib/libquazip1-qt5d.so.1.3
/usr/lib/pkgconfig/quazip1-qt5.pc
/usr/include/sqlite_orm/sqlite_orm.h
/usr/include/QuaZip-Qt5-1.3/quazip/JlCompress.h
/usr/include/QuaZip-Qt5-1.3/quazip/ioapi.h
/usr/include/QuaZip-Qt5-1.3/quazip/minizip_crypt.h
/usr/include/QuaZip-Qt5-1.3/quazip/quaadler32.h
/usr/include/QuaZip-Qt5-1.3/quazip/quachecksum32.h
/usr/include/QuaZip-Qt5-1.3/quazip/quacrc32.h
/usr/include/QuaZip-Qt5-1.3/quazip/quagzipfile.h
/usr/include/QuaZip-Qt5-1.3/quazip/quaziodevice.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazip.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazip_global.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazip_qt_compat.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazipdir.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazipfile.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazipfileinfo.h
/usr/include/QuaZip-Qt5-1.3/quazip/quazipnewinfo.h
/usr/include/QuaZip-Qt5-1.3/quazip/unzip.h
/usr/include/QuaZip-Qt5-1.3/quazip/zip.h
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/%{name}/translations/
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
# << files

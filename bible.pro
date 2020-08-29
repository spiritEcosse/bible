# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

TEMPLATE = subdirs

SUBDIRS = \
    src \
    tests

CONFIG += ordered

linux-g++ {
    message(Linux)
}

DISTFILES = \
    docker-compose.yml \
    conf.pri

TEMPLATE = subdirs
SUBDIRS += integration

DISTFILES += \
    docker-compose.yml

copydata.commands = $(COPY_DIR) $$PWD/docker-compose.yml $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

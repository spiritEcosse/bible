src_core.subdir = $$PWD/core
src_core.target = sub-core

src_entities.subdir = $$PWD/entities
src_entities.target = sub-entities
src_entities.depends = src_core

contains(CONFIG, entities) {
    SUBDIRS += \
        src_core \
        src_entities
}

CONFIG += ordered

contains(CONFIG, entities) {
    include($$PWD/Application/src/core/core.pri)
    include($$PWD/Application/src/entities/entities.pri)
}

contains(CONFIG, network) {
    include($$PWD/Application/src/network/network.pri)
}

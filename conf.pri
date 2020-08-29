contains(CONFIG, entities) {
    include($$PWD/Application/src/core/core.pri)
}

contains(CONFIG, network) {
    include($$PWD/Application/src/network/network.pri)
}

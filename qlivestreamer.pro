TEMPLATE = subdirs

SUBDIRS += \
    src

isEmpty(PREFIX) {
    PREFIX=/usr
    message("PREFIX has not been set, using default: $${PREFIX}")
}

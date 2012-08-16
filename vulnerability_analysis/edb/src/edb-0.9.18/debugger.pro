TEMPLATE	= subdirs
CONFIG		+= ordered
SUBDIRS		+= src plugins

# test for usable Qt version
!equals(QT_MAJOR_VERSION, 4)|lessThan(QT_MINOR_VERSION, 5) {
	error('edb requires Qt version 4.5 or greater')
}

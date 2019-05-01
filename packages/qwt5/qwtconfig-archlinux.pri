######################################################################
# Install paths
######################################################################

unix {
    INSTALLBASE    = /usr
}

win32 {
    INSTALLBASE    = C:/Qwt
}

target.path    = $$INSTALLBASE/lib
headers.path   = $$INSTALLBASE/include/qwt5
doc.path       = $$INSTALLBASE/share

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt     # Also for Qtopia Core!
CONFIG           += warn_on
CONFIG           += thread

######################################################################
# release/debug mode
# The designer plugin is always built in release mode.
# If want to change this, you have to edit designer/designer.pro.
######################################################################

CONFIG           += release     # release/debug
RELEASE_SUFFIX   = 5

######################################################################
# Build the static/shared libraries.
# If QwtDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

CONFIG           += QwtDll

######################################################################
# QwtPlot enables all classes, that are needed to use the QwtPlot
# widget.
######################################################################

CONFIG       += QwtPlot

######################################################################
# QwtWidgets enables all classes, that are needed to use the all other
# widgets (sliders, dials, ...), beside QwtPlot.
######################################################################

CONFIG     += QwtWidgets

######################################################################
# If you want to display svg images on the plot canvas, enable the
# line below. Note that Qwt needs the svg+xml, when enabling
# QwtSVGItem.
######################################################################

CONFIG     += QwtSVGItem

######################################################################
# If you have a commercial license you can use the MathML renderer
# of the Qt solutions package to enable MathML support in Qwt.
# So if you want this, copy qtmmlwidget.h + qtmmlwidget.cpp to
# textengines/mathml and enable the line below.
######################################################################

#CONFIG     += QwtMathML

######################################################################
# If you want to build the Qwt designer plugin,
# enable the line below.
# Otherwise you have to build it from the designer directory.
######################################################################

CONFIG     += QwtDesigner

######################################################################
# If you want to auto build the examples, enable the line below
# Otherwise you have to build them from the examples directory.
######################################################################

#CONFIG     += QwtExamples

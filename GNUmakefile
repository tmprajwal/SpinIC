# $Id: GNUmakefile,v 1.6 2010-01-11 16:30:44 gcosmo Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := spinIC
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
#/path/to/geant4.9.6-install/share/Geant4-9.6.1/geant4make
endif

.PHONY: all
all: lib bin

#### G4ANALYSIS_USE := true

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk

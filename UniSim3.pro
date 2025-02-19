# Universal Simulator main project file

TEMPLATE = subdirs

#
# Essential modules
#

SUBDIRS += unisim
unisim.subdir = src/apps/unisim

SUBDIRS += base
base.subdir = src/plugins/base
unisim.depends = base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base
unisim.depends = boxes

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base
unisim.depends = command

#
# Simulation model modules
#

SUBDIRS += vg
vg.subdir = src/plugins/vg
vg.depends = base
unisim.depends = vg

#SUBDIRS += aphid
#aphid.subdir = src/plugins/aphid
#aphid.depends = base
#unisim.depends = aphid

#SUBDIRS += cmbp
#cmbp.subdir = src/plugins/cmbp
#cmbp.depends = base
#unisim.depends = cmbp

#SUBDIRS += PestTox
#PestTox.subdir = src/plugins/PestTox
#PestTox.depends = base
#unisim.depends = PestTox

#SUBDIRS += saccharina
#saccharina.subdir = src/plugins/saccharina
#saccharina.depends = base
#unisim.depends = saccharina#

###SUBDIRS += coffee
###coffee.subdir = src/plugins/coffee
###coffee.depends = base
###unisim.depends = coffee
###
###SUBDIRS += MusselBed
###MusselBed.subdir = src/plugins/MusselBed
###MusselBed.depends = base
###unisim.depends = MusselBed

#
# InfoGrow modules
#

#SUBDIRS += iglib
#iglib.subdir = src/lib/iglib
#iglib.depends = base

#SUBDIRS += igclient
#igclient.subdir = src/apps/igclient
#igclient.depends = iglib
#igclient.depends = base
#igclient.depends = boxes
#igclient.depends = command
#igclient.depends = vg

#
# Unit tests
#

#SUBDIRS += test
#test.subdir = src/apps/test
#test.depends = base

#SUBDIRS += test_boxes
#test_boxes.subdir = src/plugins/test_boxes
#test_boxes.depends = base

#SUBDIRS += test_boxes2
#test_boxes2.subdir = src/plugins/test_boxes2
#test_boxes2.depends = base

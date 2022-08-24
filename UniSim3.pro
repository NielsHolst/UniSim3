# Universal Simulator main project file
#

TEMPLATE = subdirs

#
# Modules required for source code release
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

SUBDIRS += student
student.subdir = src/plugins/student
student.depends = base
unisim.depends = student

#
# Modules required for end-user release
#

SUBDIRS += aphid
aphid.subdir = src/plugins/aphid
aphid.depends = base
unisim.depends = aphid

#SUBDIRS += cmbp
#cmbp.subdir = src/plugins/cmbp
#cmbp.depends = base
#unisim.depends = cmbp

#SUBDIRS += coffee
#coffee.subdir = src/plugins/coffee
#coffee.depends = base
#unisim.depends = coffee

#SUBDIRS += MusselBed
#MusselBed.subdir = src/plugins/MusselBed
#MusselBed.depends = base
#unisim.depends = MusselBed

#SUBDIRS += PestTox
#PestTox.subdir = src/plugins/PestTox
#PestTox.depends = base
#unisim.depends = PestTox

#SUBDIRS += saccharina
#saccharina.subdir = src/plugins/saccharina
#saccharina.depends = base
#unisim.depends = saccharina

#SUBDIRS += vg
#vg.subdir = src/plugins/vg
#vg.depends = base
#unisim.depends = vg

#
# Modules under (re-)construction
#

#SUBDIRS += apis
#apis.subdir = src/plugins/apis
#apis.depends = base
#unisim.depends = apis

#SUBDIRS += ave
#ave.subdir = src/plugins/ave
#ave.depends = base
#unisim.depends = ave

#SUBDIRS += bmsb
#bmsb.subdir = src/plugins/bmsb
#bmsb.depends = base
#unisim.depends = bmsb

#SUBDIRS += conductance
#conductance.subdir = src/plugins/conductance
#conductance.depends = base
#unisim.depends = conductance

#SUBDIRS += cowpea
#cowpea.subdir = src/plugins/cowpea
#cowpea.depends = base
#unisim.depends = cowpea

#SUBDIRS += io
#io.subdir = src/plugins/io
#io.depends = base
#unisim.depends = io

#SUBDIRS += pinacate
#pinacate.subdir = src/plugins/pinacate
#pinacate.depends = base
#unisim.depends = pinacate

#SUBDIRS += pollnet
#pollnet.subdir = src/plugins/pollnet
#pollnet.depends = base
#unisim.depends = pollnet

#SUBDIRS += potato
#potato.subdir = src/plugins/potato
#potato.depends = base
#unisim.depends = potato

#SUBDIRS += resist
#resist.subdir = src/plugins/resist
#resist.depends = base
#unisim.depends = resist

#SUBDIRS += rvf
#rvf.subdir = src/plugins/rvf
#rvf.depends = base
#unisim.depends = rvf

#SUBDIRS += test
#test.subdir = src/plugins/test
#test.depends = base
#unisim.depends = test

#SUBDIRS += xpest
#xpest.subdir = src/plugins/xpest
#xpest.depends = base
#unisim.depends = xpest




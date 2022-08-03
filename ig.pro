TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base

SUBDIRS += iglib
iglib.subdir = src/lib/iglib
iglib.depends = base

SUBDIRS += vg
vg.subdir = src/plugins/vg
vg.depends = base

SUBDIRS += igclient
igclient.subdir = src/apps/igclient
igclient.depends = iglib
igclient.depends = base
igclient.depends = boxes
igclient.depends = command
igclient.depends = vg

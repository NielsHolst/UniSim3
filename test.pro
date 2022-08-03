TEMPLATE = subdirs

SUBDIRS += test
test.subdir = src/apps/test

SUBDIRS += base
base.subdir = src/plugins/base
test.depends = base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base
test.depends = boxes

#SUBDIRS += command
#command.subdir = src/plugins/command
#command.depends = base
#test.depends = command

SUBDIRS += test_boxes
test_boxes.subdir = src/plugins/test_boxes
test_boxes.depends = base

SUBDIRS += test_boxes2
test_boxes2.subdir = src/plugins/test_boxes2
test_boxes2.depends = base

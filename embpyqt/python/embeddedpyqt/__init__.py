# -*- coding: utf-8 -*-
import sys
import os
import sip
from PyQt4.QtCore import QMetaObject
from PyQt4.QtGui import qApp

from embeddedpyqt.miniconsole import Console
from embeddedpyqt.proxy import class_from_metaobject, wrap_instance


global_namespace = {}
base_namespace = {}
embpyqt = None


def class_by_name(name):
	ptr = embpyqt.metaObjectByName(name)
	mo = sip.wrapinstance(int(ptr), QMetaObject)
	return class_from_metaobject(mo)


def execute(command, global_context=False):
	if global_context:
		namespace = global_namespace
	else:
		namespace = dict(base_namespace)
	try:
		exec unicode(command) in namespace
	except Exception, e:
		if sys.excepthook != sys.__excepthook__:
			raise
		print 
		print "-"*40
		print e
		import traceback
		traceback.print_exc()


def init():
	global embpyqt
	embpyqt = wrap_instance(qApp.property("embedded_pyqt").toPyObject())
	embpyqt.executionRequested.connect(execute)
		
	global_namespace.update(
	  embpyqt=embpyqt,
	  embpyqt_console=wrap_instance(Console(execute))
	)

	# add classes to global_namespace
	for class_name in embpyqt.availableClasses():
		cls = class_by_name(class_name)
		global_namespace[str(class_name)] = cls


	# add existing instances to global_namespace
	for obj_name in embpyqt.availableObjects():
		obj = embpyqt.objectByName(obj_name)
		global_namespace[str(obj_name)] = obj
	
	#base_namespace.update(global_namespace)

		
	# if the env var DUMP_STUBS is set create a stub file 
	if os.environ.get("DUMP_STUBS", None):
		from embeddedpyqt.stubgen import StubCreator
		stubs = StubCreator()
		for cls in global_namespace.values():
			stubs.add_class(cls)
		target = "/tmp/embpyqtstubs.py"
		open(target, "w").write(str(stubs))
		print target, "written"

	# add global_namespace variables to globals
	for k, v in global_namespace.items():
		globals()[k] = v


init()
__all__ = global_namespace.keys()

# -*- coding: utf-8 -*-
import keyword
from PyQt4.QtCore import QMetaMethod, QObject
from embeddedpyqt.proxy import class_from_metaobject




class CodeGenerator(object):
	
	
	def __init__(self):
		self.output = []
		self.level = 0

	
	def indent(self, count=1):
		self.level += count
	
		
	def dedent(self, count=1):
		self.level -= count
	
	
	def begin(self, code=""):
		if code:
			self.emit(code)
		self.indent()

	
	def end(self):
		if self.output and self.output[-1] and self.output[-1][-1] == ":":
			self.emit("pass")
		self.dedent()


	def emit(self, code):
		indention = "  " * self.level
		self.output.append(indention + code)


	def prepend_header(self, code):
		if code not in self.output:
			self.output.insert(0, code)

		
	def newline(self, count=1):
		self.output.extend([""]*count)


	def __str__(self):
		return "\n".join(["# coding: utf-8 -*-", "# created stub"] + self.output)
	


class StubCreator(object):

	
	def __init__(self):
		self.codegen = CodeGenerator()
		self.codegen.newline()

	def add_class(self, cls):
		if isinstance(cls, QObject):
			pcls = class_from_metaobject(cls.staticMetaObject)
		else:
			pcls = cls
		self.codegen.newline()
		self.codegen.begin("class %s(object):" % pcls._metaobject.className())
		self.codegen.newline()
		qt_types = set()
		# -- methods --
		for method_name, methods in pcls._methods.items():
			if method_name.startswith("_") or method_name in ("deleteLater",):
				continue # skip private methods
			for signature, method in methods.items():
				param_names = map(unicode, method.param_names)
				param_types = map(unicode, method.param_types)
				for i, (pn, pt) in enumerate(zip(param_names, param_types)):
					pt = self.fix_type(pt)
					if pt.startswith("Q"):
						qt_types.add(pt)
					param_types[i] = pt
					if not pn:
						pn = "%s_%s" % (pt, i)
						param_names[i] = pn
				args = ["self"] + param_names
				signature = ", ".join(args)
				if method.method_type == QMetaMethod.Signal:
					self.codegen.emit("%s = pyqtSignal(%s)" % (method_name, ", ".join(param_types)))
				else:
					self.codegen.begin("def %s(%s):" % (self.fix_name(method_name), signature))
					for pname, ptype in zip(param_names, param_types):
						self.codegen.emit("assert isinstance(%s, %s)" % (pname, ptype))
					
					rtype = unicode(method.return_type)
					if rtype not in ("void", ""):
						rtype = self.fix_type(rtype)
						if rtype.startswith("Q"):
							qt_types.add(rtype)
						self.codegen.emit("return %s()" % rtype)
					self.codegen.end()
				self.codegen.newline()
				break
		# -- properties --
		for property_name, prop in pcls._properties.items():
			if property_name in ("destroyed",):
				continue
			ptype = self.fix_type(prop.type)
			if ptype.startswith("Q"):
				qt_types.add(ptype)

			getter = pcls._methods.get(prop.qt_name, None)
			setter = pcls._methods.get("set%s" % property_name, None)
			if getter and setter:
				self.codegen.emit("%s = property(fget=%s, fset=set%s)" % (property_name, prop.qt_name, property_name))
			elif getter:
				self.codegen.emit("%s = property(fget=%s)" % (property_name, prop.qt_name))
			elif setter:
				self.codegen.emit("%s = property(fset=set%s)" % (property_name, property_name))
			else:
				self.codegen.emit("%s = property()" % property_name)

		self.codegen.newline()
		self.codegen.begin("def __assert_property_types(self):")
		self.codegen.emit("# gives source code analyzers hints about assumed type of property")
		for property_name, prop in pcls._properties.items():
			if property_name in ("destroyed",):
				continue
			ptype = self.fix_type(prop.type)
			self.codegen.emit("assert isinstance(self.%s, %s)" % (property_name, ptype))
		self.codegen.end()
		self.codegen.end()
		self.codegen.newline()
		self.codegen.newline()
		
		# -- imports ---
		import PyQt4.QtCore, PyQt4.QtGui
		qt_modules = (PyQt4.QtCore, PyQt4.QtGui)
		self.codegen.prepend_header("from PyQt4.QtCore import Qt, pyqtSignal")
		for qtt in qt_types:
			for mod in qt_modules:
				if hasattr(mod, qtt):
					self.codegen.prepend_header("from %s import %s" % (mod.__name__, qtt))
					break
		
		
	def fix_name(self, n):
		if keyword.iskeyword(n):
			return "%s_" % n
		else:
			return n

		
	def fix_type(self, t):
		t = t.replace("&", "").replace("*", "").replace("::", ".")
		if t == "QString":
			t = "unicode"
		return t

	
	def __str__(self):
		return str(self.codegen)


	
if __name__ == "__main__":
	from PyQt4.QtGui import QApplication, QPushButton
	stubs = StubCreator()
	stubs.add_class(QApplication)
	stubs.add_class(QPushButton)
	print stubs

# -*- coding: utf-8 -*-
import sys

from PyQt4.QtCore import QMetaMethod, pyqtSlot, QObject, QVariant, QString, QStringList


def wrap_value(value):
    if isinstance(value, QObject):
        return wrap_instance(value)
    elif isinstance(value, QString):
        return unicode(value)
    elif isinstance(value, QStringList):
        return [unicode(v) for v in value]
    elif isinstance(value, dict):
        d = {}
        for k, v in value.items():
            d[unicode(k)] = wrap_value(v)
        return d
    elif isinstance(value, list):
        return [wrap_value(v) for v in value]
    else:
        return value

        



class QtMethodCall(QObject):
    

    def __init__(self, obj, methods):
        QObject.__init__(self)
        self.obj = obj
        self.methods = methods
        
        
    def __repr__(self):
        return "<instance method %r of %r>" % (self.methods.values()[0].name, self.obj)

    
    def _matching_method_for_signature(self, arg_types):
        found = None
        for i, (signature, method) in enumerate(self.methods.items()):
            found = method
            break # XXX: add real code here
        if not found:
            raise TypeError("No matching methods with given signature found")
        return found


    def __call__(self, *args, **kwargs):
        if kwargs:
            raise TypeError("Keyword arguments are not supported currently")
        new_args = []
        for a in args:
            if isinstance(a, PyQtClass):
                new_args.append(a._qt)
            else:
                new_args.append(a)
        args = new_args
        arg_types = [type(a) for a in args]
        method = self._matching_method_for_signature(arg_types)
        call = getattr(self.obj._qt, method.name)
        result = call(*args)
        return wrap_value(result)


    @pyqtSlot()
    def _parameterless_callback(self):
        return self()




class QtMethod(object):


    def __init__(self, meta_method):
        self._mm = meta_method
        name, _sep, params = self._mm.signature().partition("(")
        self.name = name
        self.signature = params[:-1]
        self.param_names = self._mm.parameterNames()
        self.param_types = self._mm.parameterTypes()
        self.return_type = self._mm.typeName()
        self.method_type = self._mm.methodType()

    
    def __repr__(self):
        return "%s %s(%s)" % (
              self.return_type or "void", self.name, self.signature)

              


class QtMethodSignal(object):
    

    def __init__(self, obj, signal, signature=None):
        self.obj = obj
        self.signal = signal
        self.signature = signature

        
    def __repr__(self):
        return "<Signal %r of %r>" % (self.signal, self.obj)


    def connect(self, callback):
        signal = getattr(self.obj._qt, self.signal.name)
        if isinstance(callback, QtMethodCall) and not self.signature:
            # cannot guess empty signature  with __call__(*args,**kwargs)
            signal.connect(callback._parameterless_callback)
            return
    
        if self.signature:
            signal[self.signature].connect(callback)
        else:
            signal.connect(callback)


    def __getitem__(self, *args):
        return self.__class__(self.obj, self.signal, signature=args)




class QtMethodDispatcher(object):
    

    def __init__(self, name, methods):
        self.name = name
        self.methods = methods

        
    def __call__(self, *args, **kwargs):
        raise TypeError("%r is not a static or class method" % self.name)

        
    def __repr__(self):
        return "<QtMethod %r>" % self.name


    def __get__(self, obj, cls):
        if obj is None:
            return self
        signal = None
        for method in self.methods.values():
            if method.method_type == QMetaMethod.Signal:
                signal = QtMethodSignal(obj, method)
                break
        # calls to the same method will be cached inside the object
        if signal:
            setattr(obj, self.name, signal)
            return signal
        else:
            call = QtMethodCall(obj, self.methods)
            setattr(obj, self.name, call)
            return call




class QtProperty(object):


    def __init__(self, meta_property):
        self._mp = meta_property
        name = self._mp.name()
        self.qt_name = name
        self.name = name[0].upper() + name[1:]
        self.type = self._mp.typeName()

        
    def __get__(self, obj, cls):
        if obj is None:
            return self
        if isinstance(obj, PyQtClass):
            obj = obj._qt
        value = self._mp.read(obj)
        return wrap_value(value)

    
    def __set__(self, obj, value):
        if obj is None:
            return
        if isinstance(obj, PyQtClass):
            obj = obj._qt
        if isinstance(value, PyQtClass):
            value = value._qt
        self._mp.write(obj, value)




class PyQtClass(object):
    

    def __init__(self, *args, **kwargs):
        instance = kwargs.pop("_wrap_instance", None)
        if not instance:
            mo = self.__class__.__dict__["_metaobject"]
            instance = mo.newInstance(*args)
        elif args:
            raise TypeError("Could not handle args %r for %s" % (args, self))
        self._qt = instance
        if kwargs:
            for name, value in kwargs.items():
                self.__set__(self, self.__class__, value)

        
    def __del__(self):
        pass #print self, "deleted"


    def __repr__(self):
        mo = self.__class__.__dict__["_metaobject"]
        return "<%s object %r>" % (mo.className(), unicode(self._qt.objectName()))

    
    def __iter__(self):
        for child in self._qt.children():
            yield wrap_instance(child)


    def __getattr__(self, name):
        # dynamic properties cannot be wrapped with QtProperty descriptors
        pnames = [unicode(n) for n in self._qt.dynamicPropertyNames()]
        qname = name[0].lower() + name[1:]
        if qname in pnames:
            value = self._qt.property(qname).toPyObject()
            return wrap_value(value)

        # access child objects as attribute
        for child in self._qt.children():
            if child.objectName() == name:
                return wrap_instance(child)
        # unknown
        raise AttributeError("%s has not attribute %r" % (self, name))


    def __setitem__(self, name, value):
        # set dynamic property
        if isinstance(value, PyQtClass):
            value = value._qt
        self._qt.setProperty(name, QVariant(value))


def create_proxy_class(meta_object, bases=(PyQtClass,), attrs=None):
    mo = meta_object
    properties = {}
    for pi in range(mo.propertyCount()):
        meta_property = mo.property(pi)
        p = QtProperty(meta_property)
        properties[p.name] = p
    methods = {}
    for mi in range(mo.methodCount()):
        meta_method = mo.method(mi)
        m = QtMethod(meta_method)
        methods.setdefault(m.name, {})[m.signature] = m
    if attrs is None:
        attrs = {}
    for k, v in properties.items():
        attrs[k] = v
    attrs["_properties"] = properties
    for k, v in methods.items():
        attrs[k] = QtMethodDispatcher(k, v)
    attrs["_methods"] = methods
    attrs["_metaobject"] = meta_object
    class_name = mo.className()
    return type(class_name, bases, attrs)



# cache classes
_qt_classes = {}


def class_from_metaobject(meta_object):
    class_name = meta_object.className()
    cls = _qt_classes.get(class_name, None)
    if not cls:
        cls = create_proxy_class(meta_object)
        _qt_classes[class_name] = cls
    return cls


def wrap_instance(instance):
    cls = class_from_metaobject(instance.metaObject())
    return cls(_wrap_instance=instance)




if __name__ == "__main__":
    from PyQt4.QtCore import QObject
    from PyQt4.QtGui import QApplication, QPushButton
    _a = QApplication(sys.argv)
    a = wrap_instance(_a)
    
    def on_click():
        print "clicked!"
        ap.quit()


    _b = QPushButton(None)
    b = wrap_instance(_b)
    b.Text = "Click to close!"
    b.clicked.connect(on_click)
    
    cls = class_from_metaobject(QObject.staticMetaObject)
    print cls
    t = cls()
    t.ObjectName = "foo bar"
    print t

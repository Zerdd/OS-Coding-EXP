cc      = g++
cflags  = -ldl -g
soflags = -fpic -shared
sodeps  = IPrintPlugin.h IPrintPlugin.cpp
deph    = pluginCounter.h pluginController.h
depcpp  = pluginCounter.cpp pluginController.cpp

2_4 : 2_4.cpp $(depcpp) libHC.so libHW.so
	$(cc) -o 2_4 2_4.cpp $(depcpp) $(deph) $(cflags)

libHC.so : Hello_China.cpp
	$(cc) -o libHC.so Hello_China.cpp $(sodeps) $(soflags)
	cp libHC.so plugin/

libHW.so : Hello_World.cpp
	$(cc) -o libHW.so Hello_World.cpp $(sodeps) $(soflags)
	cp libHW.so plugin/

clean:
	rm *.so
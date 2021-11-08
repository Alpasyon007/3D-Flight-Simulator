TEMPLATE = app

#Executable Name
TARGET = Assignment
CONFIG = debug

#Destination
DESTDIR = .
OBJECTS_DIR = ./build/

HEADERS	+= 	../common/Shader.h	    	\	
		../common/Vector.h		        \	
		../common/Matrix.h		        \
		../common/Mesh.h		        \
        ../common/Texture.h             \		
        ../common/SphericalCameraManipulator.h   \
		../common/Map.h					\
		../common/Object.h				\

#Sources
SOURCES += 	main.cpp			        \
  		../common/Shader.cpp		    \
		../common/Vector.cpp		    \
		../common/Matrix.cpp		    \
		../common/Mesh.cpp		        \
        ../common/Texture.cpp           \
		../common/Map.cpp				\
        ../common/SphericalCameraManipulator.cpp \
		../common/Object.cpp			\

QMAKE_CXXFLAGS_DEBUG += -fPIC

INCLUDEPATH += 	./ 				    \
		        ../common/ 			\
	
#Library Libraries
LIBS +=	-lGLEW			    	    	        \
		-lglut			        		        \
        -lGL             	                  	\ 
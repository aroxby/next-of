SRCS = main.cpp
OBJS = main.o

TARGET = next-of.exe

ifneq ($(TOOLCHAIN), MSVC)
	CPP = g++
	LD = g++
	RM = rm
	CPPOUT = -c -o
	LDOUT = -o
	RM_FLAGS += -rf
else
	CPP = cl
	LD = cl
	RM = del
	RM_FLAGS += /s /q
	CPPFLAGS += /EHsc
	CPPOUT = /c /Fo:
	LDOUT = /link /out:
endif

all: $(TARGET)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) $^ $(CPPOUT)$@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LDOUT)$@

.PHONY: clean
clean:
	$(RM) $(RM_FLAGS) $(TARGET) $(OBJS)

.PHONY: test
test: $(TARGET)
	./$(TARGET) "C:\\Windows\\system32\\cmd.exe /k dir" "C:\\msys64\\mingw64\\bin\\python.exe -v"

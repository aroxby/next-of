SRC_DIR = .
SRCS = $(shell find $(SRC_DIR) -name *.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))

TARGET = next-of

CPP=g++
LD=g++

all: $(TARGET)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) $^ -c -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)

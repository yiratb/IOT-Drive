TARGET = none

CC = g++
CFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -Werror -fPIC -I $(INC) -Wl,-rpath,.
LDFLAGS = -export-dynamic -lrt -ldl -L. -pthread -lm $(addprefix -l,$(patsubst %.o, %, $(DEPENDENCIES_OBJ))) -Wl,-rpath,.

FOLDER = projects/final_project/framework
WORKSPACE = ~/git/$(FOLDER)
SRC = $(WORKSPACE)/src/
INC = $(WORKSPACE)/inc/
TST = $(WORKSPACE)/test/

ifneq ($(TARGET),none)
DEPENDENCIES = $(shell $(CC) -MM $(SRC)$(TARGET).cpp $(CFLAGS))
DEPENDENCIES_SRC = $(shell echo $(subst $(TARGET).o: ,,$(patsubst %.hpp, %.cpp,$(subst inc,src,$(DEPENDENCIES)))))
DEPENDENCIES_OBJ = $(shell basename -a $(patsubst %.cpp, %.o,$(DEPENDENCIES_SRC)))
endif

DEBUG = 1
SUBLIB = 0

.PHONY: clean cleano cleanall release debug all test

ifeq ($(SUBLIB), 0)
$(TARGET).out: lib$(TARGET).so $(TARGET)_test.o
	@$(CC) $(TARGET)_test.o -o $@ $(LDFLAGS)
	@echo $@ Compiled Successfuly!
endif

lib$(TARGET).so: $(DEPENDENCIES_OBJ)
	@$(CC) -shared -o $@ $< $(LDFLAGS)
	@echo $@ Library compiled
						
$(TARGET).o: $(SRC)$(TARGET).cpp
ifeq ($(DEBUG), 1)
	@$(CC) -c $(SRC)$(TARGET).cpp -g $(CFLAGS) 
else
	@$(CC) -c $(SRC)$(TARGET).cpp -DNDEBUG -O3 $(CFLAGS) 
endif
	@echo $@ Object File Created!
	
$(TARGET)_test.o: $(TST)$(TARGET)_test.cpp
	@$(CC) -c $(TST)$(TARGET)_test.cpp -g $(CFLAGS) 

%.o: 
	@make -s TARGET=$(patsubst %.o,%,$@) SUBLIB=1

cleanall:
	@rm *.o *.so *.out
	@echo All files Removed Successfuly!
	
clean:
	@rm *.o *.so 
	@echo Object files and libraries Removed Successfuly!

cleano:
	@rm *.o 
	@echo All Object files Removed Successfuly!

release: 
	@make -s TARGET=$(TARGET) DEBUG=0

debug: $(TARGET).out

all: release lib$(TARGET).so
	@make -s cleano

	



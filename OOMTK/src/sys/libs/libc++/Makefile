###############################################################################
######### lightOS Makfile                                            ##########
###############################################################################
-include ../../Makefile.config
SRC= ios.cpp
OBJ= $(SRC:%.cpp=%.o)
INCLUDE= -I include $(PDCLIB_INCLUDE) $(LIBLIBGHTOS_INCLUDE)

dep:
	@$(CPP) $(INCLUDE) -MM $(SRC) > $(DEP)

-include $(DEP)

all:
	@make $(OBJ)
	@echo " AR     libc++.a"
	@ar rc libc++.a *.o

$(OBJ):
	@echo " C++    libc++/$(@:%.o=%.cpp)"
	@$(CPP) $(CPP_FLAGS) $(INCLUDE) -c $(@:%.o=%.cpp)

clean:
	@rm -f $(DEP)
	@rm -f libc++.a
	@rm -f *.o

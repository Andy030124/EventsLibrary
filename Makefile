ALL_MAKEFILES = $(shell find makes/ -type f -iname *make)
include makes/Macros.make
include makes/Config.make

# $1 obj dile
# $2 src file
define COMPILE_COMMAND
$(call GENERATION_COMMAND,Generate $1 from $2,$(call COMPILE,$(CXX),$(FLAGS) -DSELF='$2' -DSELF_STR='"$2"' -DSELF_OBJ='$1' -DSELF_OBJ_STR='"$1"',$1,$2,$(OBJDIR),$(SRCDIR)))
endef

define LINK_COMMAND
$(call CREATION_COMMAND,Linking $(APP) from $(OBJFILES),$(CXX) $(FLAGS) $(OBJFILES) $(LDFLAGS) -o $(APP))
endef

# $1 dir
define MKDIR_COMMAND
$(call CREATION_COMMAND,Creating Directory: $1,mkdir -p $1)
endef

# $1 from
# $2 to
define CP_COMMAND
$(call CREATION_COMMAND,Copy Includes Files: $1 to $2,cp -r $1 $2)
endef

# $1 from
# $2 to
define PACK_COMMAND
$(call CREATION_COMMAND,Packed Files: $1 to $2,tar -cf $2 $1)
endef

all: $(OBJDIR) $(OBJDIRS) $(BUILD_DIR) $(APP)
	$(call LOG,Application $(APP) is Updated!)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(call COMPILE_COMMAND,$@,$<)
	$(call INFO,Source File [$<] is out as Object File [$@])

$(APP): $(OBJFILES)
	$(call LINK_COMMAND)
	$(call LOG,Application Bin [$@] is new generated!)

$(OBJDIRS) $(OBJDIR) $(BUILD_DIR):
	$(call MKDIR_COMMAND,$@)
	$(call INFO,Directory [$@] is new created!)

-include $(OBJFILES:.o=.d)
include Phony.make

pack: $(OBJDIR) $(OBJDIRS) $(BUILD_DIR)
	$(call MKDIR_COMMAND,lib)
	$(call CP_COMMAND,$(shell find ./src/* -type d), ./lib/)
	$(call CP_COMMAND,./src/*.hpp, ./lib/)
	$(call PACK_COMMAND,lib/*,mlibs.tar.xz)
	$(call INFO,Files are packed!)

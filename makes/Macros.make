ifdef GNUMAKE_VERSION
  DONTCARE := -
endif

# $1 obj file
# $2 obj dir
# $3 obj ext
# $4 src dir
# $5 src ext
define OBJ2SRC
$(patsubst $2%,$4%,$(1:$3=$5))
endef 

# $1 Compiler
# $2 Flags
# $3 Source
# $4 Source Dir
# $5 Output Dir
define GEN_OBJ
	$1 -c $2 $3 -o $(patsubst $4%,$5%,$(3:.cpp=.o))
endef

# $1 Compiler
# $2 Flags
# $3 Source
# $4 Output Dir
define GEN_DEP
@$1 -MM $2 $3 | sed 's/ / $(DONTCARE)/g' > $4/$*.d
endef

# $1 ObjFile
define DEP_CURATE
	@mv -f $(1:.o=.d) $(1:.o=.d).tmp
	@sed -e 's|.*:|$1:|' < $(1:.o=.d).tmp > $(1:.o=.d)
	@sed -e 's/.*://' -e 's/\\$$//' < $(1:.o=.d).tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(1:.o=.d)
	@rm -f $(1:.o=.d).tmp
endef

# $1: dir
# $2: extension files
define FIND_FILES
$(shell find $1 -type f -iname *$2)
endef

# $1: dir
define FIND_SUBDIRS
$(shell find $1/ -type d)
endef

### CONFIGS ###
# $1 compiler
# $2 flags
### FILES ###
# $3 obj file
# $4 src file
### DIRECTORIES ###
# $5 obj dir
# $6 src dir
define COMPILE
	$(call GEN_OBJ,$1,$2,$4,$6,$5)
	$(call GEN_DEP,$1,$2,$4,$5)
	$(call DEP_CURATE,$3)
endef

include makes/Colors.make

# $1 message
define PRINT
@echo "$1"
endef

# $1 message
define LOG
$(call PRINT,\t-$(COLOR_LIGHT_BLUE) $1 $(COLOR_NORMAL))
endef

# $1 message
define INFO
$(call PRINT,\t-\t-$(COLOR_CYAN) $1 $(COLOR_NORMAL)\n)
endef

# $1 message
define SUCCESS
$(call PRINT,\t-$(COLOR_GREEN) $1 $(COLOR_NORMAL)\n)
endef

# $1 message
# $2 command
# $3 message color
define DOCOMMAND
$(call PRINT,$3-| $1 |-$(COLOR_NORMAL))
@$2
endef

# $1 message
# $2 command
define CREATION_COMMAND
$(call DOCOMMAND,$1,$2,$(COLOR_LIGHT_GREEN))
endef

# $1 message
# $2 command
define GENERATION_COMMAND
$(call DOCOMMAND,$1,$2,$(COLOR_LIGHT_YELLOW))
endef

# $1 message
# $2 command
define REMOVE_COMMAND
$(call DOCOMMAND,$1,$2,$(COLOR_LIGHT_RED))
endef

# $1 message
# $2 command
define OTHER_COMMAND
$(call DOCOMMAND,$1,$2,$(COLOR_LIGHT_CYAN))
endef



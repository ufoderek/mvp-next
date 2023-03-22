# Customizable parts of Makefile
CPP				:=	g++

OPT_FLAG		:=	-O2 -march=native -fomit-frame-pointer -pipe -fvisibility-inlines-hidden #-flto
MVP_FLAG		:=	-DSC_INCLUDE_DYNAMIC_PROCESSES
FUCKGCC_FLAG	:=	-fpermissive -Wno-write-strings -w
RELEASE_FLAG	:=	$(OPT_FLAG) $(MVP_FLAG) $(FUCKGCC_FLAG) $(COMMON_FLAG) -DRELEASE
DEBUG_FLAG		:=	$(OPT_FLAG) $(MVP_FLAG) $(FUCKGCC_FLAG) $(COMMON_FLAG) -DINST_ASSERT -DCPU_ASSERT

STRIP			:=	strip
STRIP_FLAG		:=	-s

SC_INC			:=	-I/home/ufoderek/toolchain/systemc-2.2.0-flto-v2/include -I/home/ufoderek/toolchain/tlm-2.0.1/include/tlm
SC_LIB			:=	-L/home/ufoderek/toolchain/systemc-2.2.0-flto-v2/lib-linux64
BOOST_INC       :=  -I/home/ufoderek/toolchain/boost-1.48.0/include
BOOST_LIB       :=  -L/home/ufoderek/toolchain/boost-1.48.0/lib
INC				:=	$(SC_INC) $(BOOST_INC)
LIB				:=	$(SC_LIB) $(BOOST_LIB) -lsystemc

platform		:=
target			:=
sub_dirs		:=	
CPP_FLAG		:=

ifeq "$(MAKECMDGOALS)" "pb"
platform		:=	pb
target			:=	mvp_realviewpb_armv7a
sub_dirs		+=	armv7a armv7a_inst realviewpb_top hw sys tlm_bus armv7a_tlb 
CPPFLAG			+=	$(RELEASE_FLAG)
endif

ifeq "$(MAKECMDGOALS)" "pbd"
platform		:=	pbd
target			:=	mvp_realviewpb_armv7a_debug
sub_dirs		+=	armv7a armv7a_inst realviewpb_top hw sys tlm_bus armv7a_tlb
CPPFLAG			+=	$(DEBUG_FLAG)
endif

ifeq "$(MAKECMDGOALS)" "cmp"
platform		:=	cmp
target			:=	mvp_casmp
sub_dirs		+=	armv7a armv7a_inst casmp_top hw sys tlm_bus
CPPFLAG			+=	$(RELEASE_FLAG)
endif

ifeq "$(MAKECMDGOALS)" "cmpd"
platform		:=	cmpd
target			:=	mvp_casmp_debug
sub_dirs		+=	armv7a armv7a_inst casmp_top hw sys tlm_bus
CPPFLAG			+=	$(DEBUG_FLAG)
endif

ifeq "$(MAKECMDGOALS)" "vb"
platform		:=	vb
target			:=	mvp_meteorvb_armv7a
sub_dirs		+=	armv7a armv7a_inst meteorvb_top hw sys tlm_bus
CPPFLAG			+=	$(RELEASE_FLAG)
endif

ifeq "$(MAKECMDGOALS)" "vbd"
platform		:=	vbd
target			:=	mvp_meteorvb_armv7a_debug
sub_dirs		+=	armv7a armv7a_inst meteorvb_top hw sys tlm_bus
CPPFLAG			+=	$(DEBUG_FLAG)
endif



# PHONY

.PHONY: odi

odi:
	make ODI=xxx

.PHONY: clean_odi

clean_odi:
	make clean ODI=xxx

.PHONY: hdi

hdi:
	make HDI=xxx

.PHONY: clean_hdi

clean_hdi:
	make clean HDI=xxx

.PHONY: debug

debug:
	make DEBUG=xxx

.PHONY: print_env

print_env:
	@ echo "topdir  = ${TOPDIR}"
	@ echo "srcbasedir  = ${SRCBASEDIR}"
	@ echo "srcdir  = $(SRCDIR)"
	@ echo "cpuarch = ${CPUARCH}"
	@ echo "ccc     = $(CCC)"
	@ echo "gcc     = $(GCC)"
	@ echo "cppc    = $(CPPC)"
	@ echo "cc      = $(CC)"
	@ echo "icpc    = $(ICPC)"
	@ echo "gflags  = $(GFLAGS)"
	@ echo "cflags  = $(CFLAGS)"
	@ echo "ccflags = $(CCFLAGS)"
	@ echo "cpflags = $(CPFLAGS)"
	@ echo "cpstdflag = $(CPSTDFLAG)"
	@ echo "  arm:  aflags = $(AFLAGS)"
	@ echo "       asflags = $(ASFLAGS)"
	@ echo "native = $(NATIVE)"
	@ echo "objbasedir = $(OBJBASEDIR)"
	@ echo "objdir = $(OBJBASEDIR)"
	@ echo "ofs = $(OFS)"
	@ echo "bfs = $(BFS)"
	@ echo "afs = $(AFS)"
	@ echo "all = $(ALL)"
	@ echo "CINCL = $(CINCL)"
	@ echo "vpath = $(vpath)"
	@ echo "VPATH = $(VPATH)"
	@ echo "LIBS = $(LIBS)"
	@ echo "WITH_MOSEK = $(WITH_MOSEK)"
	@ echo "MSKINCL = $(MSKINCL)"
	@ echo "MSKLIBDIR = $(MSKLIBDIR)"
	@ echo "ALL = $(ALL)"
	@ echo "OSKERNEL = ${OSKERNEL}"



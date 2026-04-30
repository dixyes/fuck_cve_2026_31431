OBJ_DIR := $(PWD)
KERNELDIR := /lib/modules/${KERNELVER}/build
KERNELSRC := /lib/modules/${KERNELVER}/source

include ${KERNELDIR}/.config

obj-m += fuck-cve-2026-31431.o

fuck-cve-2026-31431-objs += fuck_cve_2026_31431.o

all: $(MTGPU_BINARY_O) $(CONFTEST_H) $(MTGPU_ION_O)
	make -C $(KERNELDIR)   M=$(OBJ_DIR) modules
	@echo "make all end"

clean:
	make  -C $(KERNELDIR)   M=$(OBJ_DIR) clean
	@echo "make clean end"

# 编译器与目标设置
CC                    := gcc
BIN                   := posix_demo

# 构建目录与路径设置
BUILD_DIR             := ./build
BUILD_DIR_ABS         := $(abspath $(BUILD_DIR))
SRC_DIR               := .  # 源文件根目录
MAKEFILE_DIR          := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# FreeRTOS 路径设置
FREERTOS_DIR_REL      := ../FreeRTOS
FREERTOS_DIR          := $(abspath $(FREERTOS_DIR_REL))

KERNEL_DIR            := $(FREERTOS_DIR)/Source

ifeq ($(BUILD_DIR_ABS),/)
  $(error BUILD_DIR_ABS resolved to root! Check BUILD_DIR setting.)
endif

ifeq ($(FREERTOS_DIR),/)
  $(error FREERTOS_DIR resolved to root! Check FREERTOS_DIR_REL setting.)
endif

# 包含目录设置
INCLUDE_DIRS          := -I$(SRC_DIR)
INCLUDE_DIRS          += -I./Trace_Recorder_Configuration
INCLUDE_DIRS          += -I$(KERNEL_DIR)/include
INCLUDE_DIRS          += -I$(KERNEL_DIR)/portable/ThirdParty/GCC/Posix
INCLUDE_DIRS          += -I$(KERNEL_DIR)/portable/ThirdParty/GCC/Posix/utils
INCLUDE_DIRS          += -I$(FREERTOS_DIR)/Demo/Common/include

# 源文件设置
SOURCE_FILES          := $(wildcard *.c)
SOURCE_FILES          += $(wildcard $(KERNEL_DIR)/*.c)
SOURCE_FILES          += $(KERNEL_DIR)/portable/MemMang/heap_3.c
SOURCE_FILES          += $(KERNEL_DIR)/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
SOURCE_FILES          += $(KERNEL_DIR)/portable/ThirdParty/GCC/Posix/port.c
SOURCE_FILES          += $(wildcard $(FREERTOS_DIR)/Demo/Common/Minimal/*.c)

# 编译与链接选项
CFLAGS                := -ggdb3 -O3
LDFLAGS               := -ggdb3 -pthread
CPPFLAGS              := $(INCLUDE_DIRS) -DBUILD_DIR=\"$(BUILD_DIR_ABS)\" -D_WINDOWS_

ifeq ($(TRACE_ON_ENTER),1)
  CPPFLAGS              += -DTRACE_ON_ENTER=1
else
  CPPFLAGS              += -DTRACE_ON_ENTER=0
endif

ifeq ($(COVERAGE_TEST),1)
  CPPFLAGS              += -DprojCOVERAGE_TEST=1
  CPPFLAGS              += -DprojENABLE_TRACING=0
  CFLAGS                += -Werror
else
  ifeq ($(NO_TRACING),1)
    CPPFLAGS              += -DprojENABLE_TRACING=0
  else
    CPPFLAGS              += -DprojENABLE_TRACING=1
    # Trace Library.
    SOURCE_FILES          += ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/kernelports/FreeRTOS/trcKernelPort.c
    SOURCE_FILES          += $(wildcard ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/*.c )
  endif
  CPPFLAGS              += -DprojCOVERAGE_TEST=0
endif

ifdef PROFILE
  CFLAGS              +=   -pg  -O0
  LDFLAGS             +=   -pg  -O0
else
  CFLAGS              +=   -O3
  LDFLAGS             +=   -O3
endif

ifdef SANITIZE_ADDRESS
  CFLAGS              +=   -fsanitize=address -fsanitize=alignment
  LDFLAGS             +=   -fsanitize=address -fsanitize=alignment
endif

ifdef SANITIZE_LEAK
  LDFLAGS             +=   -fsanitize=leak
endif

ifeq ($(USER_DEMO),BLINKY_DEMO)
  CPPFLAGS            +=   -DUSER_DEMO=0
endif

ifeq ($(USER_DEMO),FULL_DEMO)
  CPPFLAGS            +=   -DUSER_DEMO=1
endif

ifeq ($(USER_DEMO),RANK)
  CPPFLAGS            +=   -DUSER_DEMO=2
endif

ifeq ($(USER_DEMO),ROUND_ROBIN)
  CPPFLAGS            +=   -DUSER_DEMO=3
endif


# 对象文件路径生成规则（保留源文件的子目录结构）
OBJ_FILES := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

# 替换路径中的相对前缀
OBJ_FILES := $(foreach file,$(OBJ_FILES),$(subst ../,,$(file)))
DEP_FILES := $(OBJ_FILES:.o=.d)

# 构建规则
all: $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $^ $(LDFLAGS) -o $@

# 编译规则（自动创建目录结构）
$(BUILD_DIR)/%.o: %.c Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

# 包含依赖文件
-include $(DEP_FILES)

# 清理规则
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

# 性能分析规则
GPROF_OPTIONS := --directory-path=$(INCLUDE_DIRS)
profile:
	gprof -a -p --all-lines $(GPROF_OPTIONS) $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/gmon.out > $(BUILD_DIR)/prof_flat.txt
	gprof -a --graph $(GPROF_OPTIONS) $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/gmon.out > $(BUILD_DIR)/prof_call_graph.txt
#Generated by VisualGDB project wizard. 
#Note: VisualGDB will automatically update this file when you add new sources to the project.
#All other changes you make in this file will be preserved.
#Visit http://visualgdb.com/makefiles for more details

#VisualGDB: AutoSourceFiles		#<--- remove this line to disable auto-updating of SOURCEFILES and EXTERNAL_LIBS

TARGETNAME := Glove.elf
#TARGETTYPE can be APP, STATIC or SHARED
TARGETTYPE := APP

to_lowercase = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

CONFIG ?= DEBUG

CONFIGURATION_FLAGS_FILE := $(call to_lowercase,$(CONFIG)).mak

include $(CONFIGURATION_FLAGS_FILE)
include $(ADDITIONAL_MAKE_FILES)

ifeq ($(BINARYDIR),)
error:
	$(error Invalid configuration, please check your inputs)
endif

SOURCEFILES := ../Configuration/Src/adc.c ../Configuration/Src/dma.c ../Configuration/Src/gpio.c ../Configuration/Src/i2c.c ../Configuration/Src/main.c ../Configuration/Src/spi.c ../Configuration/Src/stm32f4xx_it.c ../Configuration/Src/tim.c ../Configuration/Src/usart.c ../Configuration/Src/usb_otg.c ../Drivers/STM32F4xxxx-HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c ../Drivers/STM32F4xxxx-HAL/startup_stm32f4xx.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_msp_template.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c ../Features/Battery.c ../Features/Button.c ../Features/Calibration.c ../Features/Filters.c ../Features/IMU.c ../Features/Interrupt.c ../Features/LED.c ../Features/LSM330DLC.c ../Features/newlib.c ../Features/OTP.c ../Features/Spatial.c ../Features/SPICallbacks.c ../Features/Sqrt.c Glove.c
EXTERNAL_LIBS := 
EXTERNAL_LIBS_COPIED := $(foreach lib, $(EXTERNAL_LIBS),$(BINARYDIR)/$(notdir $(lib)))

CFLAGS += $(COMMONFLAGS)
CXXFLAGS += $(COMMONFLAGS)
ASFLAGS += $(COMMONFLAGS)
LDFLAGS += $(COMMONFLAGS)

CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
CXXFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
CXXFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
ASFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))

CXXFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))
CFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))
LDFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))

LDFLAGS += $(addprefix -L,$(LIBRARY_DIRS))

ifeq ($(GENERATE_MAP_FILE),1)
LDFLAGS += -Wl,-Map=$(BINARYDIR)/$(basename $(TARGETNAME)).map
endif

LIBRARY_LDFLAGS = $(addprefix -l,$(LIBRARY_NAMES))

ifeq ($(IS_LINUX_PROJECT),1)
	RPATH_PREFIX := -Wl,--rpath='$$ORIGIN/../
	LIBRARY_LDFLAGS += $(EXTERNAL_LIBS)
	LIBRARY_LDFLAGS += -Wl,--rpath='$$ORIGIN'
	LIBRARY_LDFLAGS += $(addsuffix ',$(addprefix $(RPATH_PREFIX),$(dir $(EXTERNAL_LIBS))))
	
	ifeq ($(TARGETTYPE),SHARED)
		CFLAGS += -fPIC
		CXXFLAGS += -fPIC
		ASFLAGS += -fPIC
		LIBRARY_LDFLAGS += -Wl,-soname,$(TARGETNAME)
	endif
	
	ifneq ($(LINUX_PACKAGES),)
		PACKAGE_CFLAGS := $(foreach pkg,$(LINUX_PACKAGES),$(shell pkg-config --cflags $(pkg)))
		PACKAGE_LDFLAGS := $(foreach pkg,$(LINUX_PACKAGES),$(shell pkg-config --libs $(pkg)))
		CFLAGS += $(PACKAGE_CFLAGS)
		CXXFLAGS += $(PACKAGE_CFLAGS)
		LIBRARY_LDFLAGS += $(PACKAGE_LDFLAGS)
	endif	
else
	LIBRARY_LDFLAGS += $(EXTERNAL_LIBS)
endif

LIBRARY_LDFLAGS += $(ADDITIONAL_LINKER_INPUTS)

all_make_files := $(firstword $(MAKEFILE_LIST)) $(CONFIGURATION_FLAGS_FILE) $(ADDITIONAL_MAKE_FILES)

ifeq ($(STARTUPFILES),)
	all_source_files := $(SOURCEFILES)
else
	all_source_files := $(STARTUPFILES) $(filter-out $(STARTUPFILES),$(SOURCEFILES))
endif

source_obj1 := $(all_source_files:.cpp=.o)
source_obj2 := $(source_obj1:.c=.o)
source_obj3 := $(source_obj2:.s=.o)
source_obj4 := $(source_obj3:.S=.o)
source_obj5 := $(source_obj4:.cc=.o)
source_objs := $(source_obj5:.cxx=.o)

all_objs := $(addprefix $(BINARYDIR)/, $(notdir $(source_objs)))

PRIMARY_OUTPUTS :=

ifeq ($(GENERATE_BIN_FILE),1)
PRIMARY_OUTPUTS += $(BINARYDIR)/$(basename $(TARGETNAME)).bin
endif

ifeq ($(GENERATE_IHEX_FILE),1)
PRIMARY_OUTPUTS += $(BINARYDIR)/$(basename $(TARGETNAME)).ihex
endif

ifeq ($(PRIMARY_OUTPUTS),)
PRIMARY_OUTPUTS := $(BINARYDIR)/$(TARGETNAME)
endif

all: $(PRIMARY_OUTPUTS)

$(BINARYDIR)/$(basename $(TARGETNAME)).bin: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) -O binary $< $@

$(BINARYDIR)/$(basename $(TARGETNAME)).ihex: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) -O ihex $< $@

ifeq ($(TARGETTYPE),APP)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTERNAL_LIBS)
	$(LD) -o $@ $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP)
endif

ifeq ($(TARGETTYPE),SHARED)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTERNAL_LIBS)
	$(LD) -shared -o $@ $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP)
endif
	
ifeq ($(TARGETTYPE),STATIC)
$(BINARYDIR)/$(TARGETNAME): $(all_objs)
	$(AR) -r $@ $^
endif

-include $(all_objs:.o=.dep)

clean:
ifeq ($(USE_DEL_TO_CLEAN),1)
	del /S /Q $(BINARYDIR)
else
	rm -rf $(BINARYDIR)
endif

$(BINARYDIR):
	mkdir $(BINARYDIR)

#VisualGDB: FileSpecificTemplates		#<--- VisualGDB will use the following lines to define rules for source files in subdirectories
$(BINARYDIR)/%.o : %.cpp $(all_make_files) |$(BINARYDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.S $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(ASFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.s $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(ASFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.cc $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.cxx $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

#VisualGDB: GeneratedRules				#<--- All lines below are auto-generated


$(BINARYDIR)/adc.o : ../Configuration/Src/adc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/dma.o : ../Configuration/Src/dma.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/gpio.o : ../Configuration/Src/gpio.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/i2c.o : ../Configuration/Src/i2c.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/main.o : ../Configuration/Src/main.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/spi.o : ../Configuration/Src/spi.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_it.o : ../Configuration/Src/stm32f4xx_it.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/tim.o : ../Configuration/Src/tim.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/usart.o : ../Configuration/Src/usart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/usb_otg.o : ../Configuration/Src/usb_otg.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/system_stm32f4xx.o : ../Drivers/STM32F4xxxx-HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/startup_stm32f4xx.o : ../Drivers/STM32F4xxxx-HAL/startup_stm32f4xx.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_adc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_adc_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_can.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_cortex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_crc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_cryp.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_cryp_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_dac.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_dac_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_dcmi.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_dma.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_dma2d.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_dma_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_eth.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_flash.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_flash_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_flash_ramfunc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_gpio.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_hash.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_hash_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_hcd.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_i2c.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_i2c_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_i2s.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_i2s_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_irda.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_iwdg.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_ltdc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_msp_template.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_msp_template.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_nand.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_nor.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_pccard.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_pcd.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_pcd_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_pwr.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_pwr_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_rcc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_rcc_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_rng.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_rtc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_rtc_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_sai.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_sd.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_sdram.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_smartcard.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_spi.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_sram.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_tim.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_tim_ex.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_uart.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_usart.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_hal_wwdg.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_ll_fmc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_ll_fsmc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_ll_sdmmc.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f4xx_ll_usb.o : ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Battery.o : ../Features/Battery.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Button.o : ../Features/Button.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Calibration.o : ../Features/Calibration.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Filters.o : ../Features/Filters.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/IMU.o : ../Features/IMU.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Interrupt.o : ../Features/Interrupt.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/LED.o : ../Features/LED.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/LSM330DLC.o : ../Features/LSM330DLC.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/newlib.o : ../Features/newlib.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/OTP.o : ../Features/OTP.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Spatial.o : ../Features/Spatial.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/SPICallbacks.o : ../Features/SPICallbacks.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/Sqrt.o : ../Features/Sqrt.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


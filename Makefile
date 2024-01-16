#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!
#
##############################################################################################
#
# On command line:
#
# make all = Create project
#
# make clean = Clean project files.
#
# To rebuild project do "make clean" and "make all".
#
# Included originally in the yagarto projects. Original Author : Michael Fischer
# Modified to suit our purposes by Hussam Al-Hertani
# Use at your own risk!!!!!
##############################################################################################
# Start of default section
#
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
#AS   = $(TRGT)gcc -x assembler-with-cpp
AS   = $(TRGT)as
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary -S
MCU  = cortex-m3

# List all default C defines here, like -D_DEBUG=1
# for STM32F103RCT6 micro
DDEFS = -DSTM32F10X_HD
#para el micro STM32F103C8
# DDEFS = -DSTM32F10X_MD

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR = ./src
CORELIBDIR = ./cmsis_core
STARTUPDIR = ./startup_src
LINKER = ./startup_src

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

#
# Define project
PROJECT = Template_F103

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

## Source Files
SRC  = ./src/main.c
SRC += ./src/usart.c
SRC += ./src/tim.c
SRC += ./src/it.c
SRC += ./src/flash_program.c
SRC += ./src/adc.c
SRC += ./src/gpio.c
SRC += ./src/dma.c
SRC += ./src/dac.c
SRC += ./src/dac_mux.c
# SRC += ./src/utils.c

SRC += ./src/test_functions.c
SRC += ./src/pwm.c
SRC += ./src/hard.c
SRC += ./src/dmx_transceiver.c
SRC += ./src/dsp.c
SRC += ./src/i2c.c

SRC += ./src/manager.c
SRC += ./src/filters_and_offsets.c
SRC += ./src/comm.c

# linked modules
SRC += ./src/ssd1306_display.c
SRC += ./src/ssd1306_gfx.c
SRC += ./src/screen.c
SRC += ./src/dmx_menu.c
SRC += ./src/display_utils.c
SRC += ./src/dmx1_mode.c
SRC += ./src/dmx2_mode.c
SRC += ./src/master_slave_mode.c
SRC += ./src/manual_mode.c
SRC += ./src/reset_mode.c
SRC += ./src/main_menu.c
SRC += ./src/hardware_mode.c
SRC += ./src/colors_functions.c
SRC += ./src/fixed_menu.c
SRC += ./src/colors_menu.c
SRC += ./src/slave_menu.c
SRC += ./src/master_slave_menu.c
SRC += ./src/master_menu.c
SRC += ./src/manual_menu.c
SRC += ./src/options_menu.c
SRC += ./src/current_menu.c
SRC += ./src/limits_menu.c
SRC += ./src/channels_menu.c
SRC += ./src/temp_menu.c
SRC += ./src/version_menu.c
SRC += ./src/encoder_menu.c
SRC += ./src/temperatures.c







## Core Support
SRC += ./startup_src/syscalls.c
SRC += ./startup_src/startup_clocks.c
SRC += ./cmsis_core/core_cm3.c

# List ASM source files here
ASRC = ./startup_src/startup_stm32f10x_hd.s

# List all user directories here
UINCDIR = $(CORELIBDIR) \
	  $(STARTUPDIR) \
          ./inc

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

# Define optimisation level here
# O1 optimiza size no significativo
# O2 size mas fuerte
# Os (size mas fuerte que O2)
# O3 el mas fuerte, seguro despues no corre
# O0 (no optimiza casi nada, mejor para debug)
OPT = -O0

#
# End of user defines
##############################################################################################
#
# Define linker script file here
#
LDSCRIPT = $(LINKER)/stm32_flash.ld
FULL_PRJ = $(PROJECT)_rom

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

ADEFS   = $(DADEFS) $(UADEFS)

LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU)

ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb  -a=$(<:.s=.lst)

# SIN INFO DEL DEBUGGER
#CPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -mthumb   -fomit-frame-pointer -Wall -Wstrict-prototypes -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DEFS)

# CON INFO PARA DEBUGGER
#CPFLAGS = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DEFS)

# CON INFO PARA DEBUGGER + STRIP CODE
CPFLAGS = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -fdata-sections -ffunction-sections -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DDEFS)

# SIN DEAD CODE, hace el STRIP
#LDFLAGS = $(MCFLAGS) -mthumb --specs=nano.specs -Wl,--gc-sections -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)

# SIN DEAD CODE, hace el STRIP + FLOAT = SOFT
LDFLAGS = $(MCFLAGS) -mthumb -lm --specs=nano.specs -Wl,--gc-sections -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)

# CON DEAD CODE
#LDFLAGS = $(MCFLAGS) -mthumb --specs=nano.specs -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)
#LDFLAGS = $(MCFLAGS) -mthumb -T$(LDSCRIPT) -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)

#
# makefile rules
#

assemblersources = $(ASRC)
sources = $(SRC)
OBJS  = $(ASRC:.s=.o) $(SRC:.c=.o)

objects = $(sources:.c=.o)
assobjects = $(assemblersources:.s=.o)

all: $(assobjects) $(objects) $(FULL_PRJ).elf $(FULL_PRJ).bin
	arm-none-eabi-size $(FULL_PRJ).elf
	gtags -q

$(assobjects): %.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

$(objects): %.o: %.c
	$(CC) -c $(CPFLAGS) -I. $(INCDIR) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN)  $< $@

flash:
	sudo openocd -f stm32f1_flash.cfg

flash_geehy:
	sudo openocd -f stm32f1_flash_geehy.cfg

gdb:
	sudo openocd -f stm32f1_gdb.cfg

reset:
	sudo openocd -f stm32f1_reset.cfg

reset_geehy:
	sudo openocd -f stm32f1_reset_geehy.cfg

clean:
	rm -f $(OBJS)
	rm -f $(FULL_PRJ).elf
	rm -f $(FULL_PRJ).map
	rm -f $(FULL_PRJ).hex
	rm -f $(FULL_PRJ).bin
	rm -f $(SRC:.c=.lst)
	rm -f $(SRC:.c=.su)
	rm -f $(ASRC:.s=.lst)
	rm -f *.o
	rm -f *.out
	rm -f *.gcov
	rm -f *.gcda
	rm -f *.gcno
	rm -f tests_gtk


tests_comm:
	# first module objects to test
	gcc -c --coverage src/comm.c -I. $(INCDIR) $(DDEFS)
	# second auxiliary helper modules
	gcc -c src/tests_ok.c -I $(INCDIR)
	gcc -c src/tests_mock_usart.c -I $(INCDIR)
	gcc --coverage src/tests_comm.c comm.o tests_ok.o tests_mock_usart.o -I $(INCDIR) $(DDEFS)
	# test execution
	./a.out
	# process coverage
	gcov comm.c -m


tests_pwm:
	# first module objects to test and coverage
	gcc -c --coverage src/pwm.c -I. $(INCDIR)
	# second auxiliary helper modules
	gcc -c src/tests_ok.c -I $(INCDIR)
	# compile the test and link with modules
	gcc --coverage src/tests_pwm.c pwm.o tests_ok.o
	# test execution
	./a.out
	# process coverage
	gcov pwm.c -m


tests_dmx_transceiver:
	# first module objects to test and coverage
	gcc -c --coverage src/dmx_transceiver.c -I. $(INCDIR) $(DDEFS)
	# second auxiliary helper modules
	gcc -c src/tests_ok.c -I $(INCDIR)
	gcc -c src/tests_vector_utils.c -I $(INCDIR)
	# compile the test and link with modules
	gcc --coverage src/tests_dmx_transceiver.c dmx_transceiver.o tests_ok.o tests_vector_utils.o
	# test execution
	./a.out
	# process coverage
	gcov dmx_transceiver.c -m


tests_pwm_post_map:
	# first compile common modules (modules to test and dependencies)
	gcc -c src/pwm.c -I. $(INCDIR)
	gcc -c src/filters_and_offsets.c -I. $(INCDIR)
	gcc -c src/dsp.c -I. $(INCDIR)
	# second auxiliary helper modules
	gcc -c src/tests_ok.c -I $(INCDIR)
	gcc -c src/tests_vector_utils.c -I $(INCDIR)
	gcc src/tests_pwm_post_map.c pwm.o filters_and_offsets.o dsp.o tests_ok.o tests_vector_utils.o
	./a.out


tests_oled_screen:
	# first compile common modules (modules to test and dependencies)
	gcc -c src/screen.c -I. $(INCDIR)
	gcc -c src/ssd1306_display.c -I. $(INCDIR)
	gcc -c src/ssd1306_gfx.c -I. $(INCDIR)
	# the module that implements tests_lcd_application.h functions
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_oled_app.c -o tests_oled_app.o
	# then the gtk lib modules
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_glade_oled.c -o tests_glade_oled.o
	# link everything
	gcc tests_glade_oled.o tests_oled_app.o screen.o ssd1306_display.o ssd1306_gfx.o `pkg-config --libs gtk+-3.0` -o tests_gtk
	# run global tags
	gtags -q
	# run the simulation
	# ./tests_gtk


tests_oled_manual_menu:
	# first compile common modules (modules to test and dependencies)
	gcc -c src/screen.c -I. $(INCDIR)
	gcc -c src/ssd1306_display.c -I. $(INCDIR)
	gcc -c src/ssd1306_gfx.c -I. $(INCDIR)
	gcc -c src/manual_menu.c -I. $(INCDIR)
	gcc -c src/options_menu.c -I. $(INCDIR)
	gcc -c src/display_utils.c -I. $(INCDIR)
	# the module that implements tests_lcd_application.h functions
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_oled_manual_menu.c -o tests_oled_manual_menu.o
	# then the gtk lib modules
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_glade_oled.c -o tests_glade_oled.o
	# link everything
	gcc tests_glade_oled.o tests_oled_manual_menu.o manual_menu.o options_menu.o display_utils.o screen.o ssd1306_display.o ssd1306_gfx.o `pkg-config --libs gtk+-3.0` -o tests_gtk
	# run global tags
	gtags -q
	# run the simulation
	# ./tests_gtk


tests_oled_dmx_menu:
	# first compile common modules (modules to test and dependencies)
	gcc -c src/screen.c -I. $(INCDIR)
	gcc -c src/ssd1306_display.c -I. $(INCDIR)
	gcc -c src/ssd1306_gfx.c -I. $(INCDIR)
	gcc -c src/dmx_menu.c -I. $(INCDIR)
	# gcc -c src/options_menu.c -I. $(INCDIR)
	gcc -c src/display_utils.c -I. $(INCDIR)
	# the module that implements tests_lcd_application.h functions
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_oled_dmx_menu.c -o tests_oled_dmx_menu.o
	# then the gtk lib modules
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_glade_oled.c -o tests_glade_oled.o
	# link everything
	gcc tests_glade_oled.o tests_oled_dmx_menu.o dmx_menu.o options_menu.o display_utils.o screen.o ssd1306_display.o ssd1306_gfx.o `pkg-config --libs gtk+-3.0` -o tests_gtk
	# run global tags
	gtags -q
	# run the simulation
	# ./tests_gtk


tests_oled_cct_dmx_menu:
	# first compile common modules (modules to test and dependencies)
	gcc -c src/screen.c -I. $(INCDIR)
	gcc -c src/ssd1306_display.c -I. $(INCDIR)
	gcc -c src/ssd1306_gfx.c -I. $(INCDIR)
	gcc -c src/cct_dmx_menu.c -I. $(INCDIR)
	gcc -c src/display_utils.c -I. $(INCDIR)
	# the module that implements tests_lcd_application.h functions
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_oled_cct_dmx_menu.c -o tests_oled_cct_dmx_menu.o
	# then the gtk lib modules
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_glade_oled.c -o tests_glade_oled.o
	# link everything
	gcc tests_glade_oled.o tests_oled_cct_dmx_menu.o cct_dmx_menu.o display_utils.o screen.o ssd1306_display.o ssd1306_gfx.o `pkg-config --libs gtk+-3.0` -o tests_gtk
	# run global tags
	gtags -q
	# run the simulation
	# ./tests_gtk


tests_oled_cct_manual_cct_menu:
	# first compile common modules (modules to test and dependencies)
	gcc -c src/screen.c -I. $(INCDIR)
	gcc -c src/ssd1306_display.c -I. $(INCDIR)
	gcc -c src/ssd1306_gfx.c -I. $(INCDIR)
	gcc -c src/cct_utils.c -I. $(INCDIR)
	gcc -c src/cct_manual_cct_menu.c -I. $(INCDIR)
	gcc -c src/display_utils.c -I. $(INCDIR)
	# the module that implements tests_lcd_application.h functions
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_oled_cct_manual_cct_menu.c -o tests_oled_cct_manual_cct_menu.o
	# then the gtk lib modules
	gcc -c `pkg-config --cflags gtk+-3.0` src/tests_glade_oled.c -o tests_glade_oled.o
	# link everything
	gcc tests_glade_oled.o tests_oled_cct_manual_cct_menu.o cct_utils.o cct_manual_cct_menu.o display_utils.o screen.o ssd1306_display.o ssd1306_gfx.o `pkg-config --libs gtk+-3.0` -o tests_gtk
	# run global tags
	gtags -q
	# run the simulation
	# ./tests_gtk



# *** EOF ***

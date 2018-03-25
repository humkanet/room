XC="C:/Program Files (x86)/Microchip/xc8/v1.45/bin"
CC=$(XC)/xc8.exe

NAME     = room
MCU      = 16lf15325

FOSC     = 1000000UL

OBJ_DIR  = ./obj

CFLAGS=--chip=$(MCU) -D_XTAL_FREQ=$(FOSC) -D_LFINTOSC_FREQ=32000UL -Q --debugger=none \
	--opt=asm,space --dep=none --objdir=$(OBJ_DIR)

all: $(OBJ_DIR) $(NAME).cof

$(NAME).cof: main.p1 fnv1.p1 clock.p1 nvm.p1 i2c.p1 spi.p1 cc1101.p1 shtc1.p1 ldo.p1 vbat.p1
	$(CC) $(CFLAGS) --output=mcof,inhx32 -O$@ $^

%.p1: %.c %.h
	$(CC) $(CFLAGS) --pass1 -C -O$@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(NAME).hex $(NAME).cof *.p1 *.pre *.as *.cmf *.hxl *.obj *.sdb *.sym

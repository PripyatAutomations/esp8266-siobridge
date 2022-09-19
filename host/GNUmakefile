include ../config.mk

bin: genconf ansi_chart


genconf_objs += genconf.o
ansi_chart_objs += ansi_chart.o

all: world

world: ${bin}

clean:
	${RM} ${bin} *.o

install:
	@echo "No installed needed for host tools, skipping!"

genconf: ${genconf_objs}
	${HOST_CC} ${HOST_LDFLAGS} -o $@ $^

ansi_chart: ${ansi_chart_obsj}
	${HOST_CC} ${HOST_LDFLAGS} -o $@ $^

%.o:%.c
	${HOST_CC} ${HOST_CFLAGS} -o $@ $^

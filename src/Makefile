MAKE_DIRS	=	bitfields \
				datatypes \
                gpio \
                gpio_v2 \
                hw_1.5 \
				interrupts \
				interrupts_v2 \
				interrupts_v3 \
				padding

all: $(MAKE_DIRS)
	@for d in $(MAKE_DIRS); do \
		$(MAKE) -C $$d; \
	done


clean: $(MAKE_DIRS)
	@for d in $(MAKE_DIRS); do \
		$(MAKE) -C $$d clean; \
	done

# vim:set ts=4 tw=79 syntax=make et:

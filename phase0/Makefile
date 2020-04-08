
default:
	@echo "Please specify a target"

umps:
	$(MAKE) -f umpsmake0
	
umps2:
	$(MAKE) -f umpsmake0

uarm:
	$(MAKE) -f uarmmake0

clean:
	$(MAKE) -f uarmmake0 clean
	$(MAKE) -f umpsmake0 clean

.PHONY: umps umps2 uarm clean default
